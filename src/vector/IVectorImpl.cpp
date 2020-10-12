#include <cmath>
#include "include/IVector.h"

namespace {
    class VectorImpl : IVector {
        ILogger * _logger {nullptr};
        double * _data {nullptr};
        size_t _dim {0};
    public:
        VectorImpl(size_t dim, double * data);
        ~VectorImpl()                                         override;
        IVector * clone()                               const override;
        double norm(Norm norm)                          const override;
        size_t getDim()                                 const override;
        double getCoord(size_t index)                   const override;
        ReturnCode setCoord(size_t index, double value) const override;
        bool operator==(const IVector * vec);
    };
}

VectorImpl::VectorImpl(size_t dim, double * data) : _dim(dim), _data(data) {
    _logger = ILogger::createLogger(this);
}

VectorImpl::~VectorImpl() {
    if (_data != nullptr) {
        delete[] _data;
        _data = nullptr;
    }
    if(_logger != nullptr) {
        _logger->releaseLogger(this);
    }
}

size_t VectorImpl::getDim() const {
    return _dim;
}

double VectorImpl::getCoord(size_t index) const {
    if (index >= _dim || index < 0) {
        _log(_logger, ReturnCode::RC_OUT_OF_BOUNDS);
        return std::nan("1");
    }
    return _data[index];
}

ReturnCode VectorImpl::setCoord(size_t index, double value) const {
    if (index >= _dim || index < 0) {
        _log(_logger, ReturnCode::RC_OUT_OF_BOUNDS);
        return ReturnCode::RC_INVALID_PARAMS;
    }
    if (std::isnan(value) || std::isinf(value)) {
        _log(_logger, ReturnCode::RC_NAN);
        return ReturnCode::RC_NAN;
    }
    _data[index] = value;
    return ReturnCode::RC_SUCCESS;
}

double VectorImpl::norm(Norm norm) const {
    double res = 0;
    switch (norm) {
        case Norm::NORM_1:
            for (size_t i = 0; i < _dim; ++i) {
                res += std::fabs(_data[i]);
            }
            break;
        case Norm::NORM_2:
            for (size_t i = 0; i < _dim; ++i) {
                res += _data[i] * _data[i];
            }
            res = std::sqrt(res);
            break;
        case Norm::NORM_INF:
            res = std::fabs(_data[0]);
            for (size_t i = 1; i < _dim; ++i) {
                if (res < std::fabs(_data[i]))
                    res = std::fabs(_data[i]);
            }
            break;
    }
    return res;
}

IVector * VectorImpl::clone() const {
    return createVector(_dim, _data, _logger);
}
