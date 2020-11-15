#include "include/ISet.h"
#include <stdlib.h>
#include <cmath>
#include <vector>

static ReturnCode validateVector(const IVector * vec) {
    if (!vec) {
        return ReturnCode::RC_NULL_PTR;
    }
    if (vec->getDim() == 0){
        return ReturnCode::RC_ZERO_DIM;
    }
    for (int i = 0; i < vec->getDim(); i++){
        if(std::isinf(vec->getCoord(i)) || std::isnan(vec->getCoord(i))){
            return ReturnCode::RC_NAN;
        }
    }
    return ReturnCode::RC_SUCCESS;
}

namespace {
    class ISetImpl : public ISet {
    private:
        size_t _dim {0};
        std::vector<IVector *> _data;
        ILogger * _logger {nullptr};

    public:
        ISetImpl();

        ReturnCode insert(IVector const * vector, IVector::Norm norm, double accuracy)  override;
        ReturnCode erase(IVector const * vector, IVector::Norm norm, double accuracy) 	override;
        ReturnCode erase(size_t index) 													override;
        void clear() 																	override;
        ReturnCode find(IVector const * vector, IVector::Norm norm, double accuracy, size_t & ind) const override;
        ReturnCode get(IVector *& dst, size_t ind) 													const override;
        size_t getDim() 																			const override;
        size_t getSize() 																			const override;
        ISet * clone() 																				const override;

        ~ISetImpl()                                                                                       override;
    };
}

ISetImpl::ISetImpl() : _dim(0) {
    _logger = ILogger::createLogger(this);
}

ReturnCode ISetImpl::insert(IVector const * vector, IVector::Norm norm, double accuracy) {
    ReturnCode r_code = validateVector(vector);
    if (r_code != ReturnCode::RC_SUCCESS) {
        LOG(_logger, r_code);
        return r_code;
    }

    if (accuracy < 0 || std::isnan(accuracy)) {
        LOG(_logger, ReturnCode::RC_INVALID_PARAMS);
        return ReturnCode::RC_INVALID_PARAMS;
    }

    if (_data.empty()) {
        _data.push_back(vector->clone());
        _dim = vector->getDim();
        return ReturnCode::RC_SUCCESS;
    } else {
        if (_dim != vector->getDim()) {
            return ReturnCode::RC_WRONG_DIM;
        }
    }

    for (auto cur_vec : _data) {
        bool equal = false;
        IVector::equals(cur_vec, vector, norm, accuracy, equal, nullptr);
        if (equal) {
            return ReturnCode::RC_SUCCESS;
        }
    }

    _data.push_back(vector->clone());
    return ReturnCode::RC_SUCCESS;
}

ReturnCode ISetImpl::erase(IVector const * vector, IVector::Norm norm, double accuracy) {
    ReturnCode r_code = validateVector(vector);
    if (r_code != ReturnCode::RC_SUCCESS) {
        LOG(_logger, r_code)
        return r_code;
    }
    if (vector->getDim() != _dim) {
        return ReturnCode::RC_WRONG_DIM;
    }
    if (accuracy < 0 || std::isnan(accuracy)) {
        LOG(_logger, ReturnCode::RC_INVALID_PARAMS)
        return ReturnCode::RC_INVALID_PARAMS;
    }
    if (_data.empty()) {
        LOG(_logger, ReturnCode::RC_ELEM_NOT_FOUND)
        return ReturnCode::RC_ELEM_NOT_FOUND;
    }

    bool found = false;
    for (auto cur_vec : _data) {
        bool equal = false;
        IVector::equals(cur_vec, vector, norm, accuracy, equal, _logger);
        if (equal) {
            found = true;
            size_t cur_vec_ind;
            find(vector, norm, accuracy, cur_vec_ind);
            erase(cur_vec_ind);
            break;
        }
    }
    if (!found) {
        return ReturnCode::RC_ELEM_NOT_FOUND;
    } else {
        return ReturnCode::RC_SUCCESS;
    }
}

ReturnCode ISetImpl::erase(size_t index) {
    if (index >= _data.size() || index < 0) {
        LOG(_logger, ReturnCode::RC_INVALID_PARAMS);
        return ReturnCode::RC_INVALID_PARAMS;
    }

    delete _data[index];
    _data.erase(_data.begin() + index);

    if (_data.empty()) {
        _data.clear();
        _dim = 0;
    }

    return ReturnCode::RC_SUCCESS;
}

ReturnCode ISetImpl::get(IVector*& dst, size_t ind) const {
    if (ind >= _data.size()) {
        LOG(_logger, ReturnCode::RC_INVALID_PARAMS);
        return ReturnCode::RC_INVALID_PARAMS;
    }

    dst = _data[ind]->clone();
    return ReturnCode::RC_SUCCESS;
}

ReturnCode ISetImpl::find(IVector const* vector, IVector::Norm norm, double accuracy, size_t& ind) const {
    ReturnCode r_code = validateVector(vector);
    if (r_code != ReturnCode::RC_SUCCESS) {
        LOG(_logger, r_code);
        return r_code;
    }
    if (_dim != vector->getDim()) {
        LOG(_logger, ReturnCode::RC_WRONG_DIM);
        return ReturnCode::RC_WRONG_DIM;
    }
    if (std::isnan(accuracy) || accuracy < 0) {
        LOG(_logger, ReturnCode::RC_INVALID_PARAMS);
        return ReturnCode::RC_INVALID_PARAMS;
    }

    if (_data.size() == 0 || _dim == 0) {
        return ReturnCode::RC_ELEM_NOT_FOUND;
    }

    size_t capacity = _data.size();
    IVector * cur_vec;

    for (size_t cur_vec_ind = 0; cur_vec_ind < capacity; cur_vec_ind++) {
        cur_vec = _data[cur_vec_ind];
        bool equal = false;
        IVector::equals(cur_vec, vector, norm, accuracy, equal, nullptr);
        if (equal) {
            ind = cur_vec_ind;
            return ReturnCode::RC_SUCCESS;
        }
    }
    return ReturnCode::RC_ELEM_NOT_FOUND;
}

size_t ISetImpl::getDim() const {
    return _dim;
}

size_t ISetImpl::getSize() const {
    return _data.size();
}

ISet* ISetImpl::clone() const {
    ISetImpl* new_set = new(std::nothrow) ISetImpl();
    if (new_set == nullptr) {
        LOG(_logger, ReturnCode::RC_NO_MEM)
        return nullptr;
    }

    new_set->_dim = _dim;
    for (auto cur_vec : _data) {
        new_set->_data.push_back(cur_vec->clone());
    }

    return new_set;
}

void ISetImpl::clear() {
    for(auto cur_vec : _data) {
        delete cur_vec;
    }
    _data.clear();
    _dim = 0;
}

ISetImpl::~ISetImpl() {
    for(auto cur_vec : _data) {
        delete cur_vec;
    }
    _data.clear();
    _dim = 0;

    if (_logger != nullptr) {
        _logger->releaseLogger(this);
    }
}