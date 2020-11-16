#include <cmath>
#include "include/IVector.h"

namespace {
    class IVectorImpl : IVector {
        ILogger * _logger {nullptr};
        double * _data {nullptr};
        size_t _dim;
    public:
        IVectorImpl(size_t dim, double * data);

        size_t getDim()                                 const override;
        double getCoord(size_t index)                   const override;
        ReturnCode setCoord(size_t index, double value) const override;
        double norm(Norm norm)                          const override;
        IVector * clone()                               const override;

        ~IVectorImpl()                                        override;
    };
}

IVectorImpl::IVectorImpl(size_t dim, double * data) : _dim(dim), _data(data) {
    _logger = ILogger::createLogger(this);
}

size_t IVectorImpl::getDim() const {
    return _dim;
}

double IVectorImpl::getCoord(size_t index) const {
    if (index >= _dim || index < 0) {
        LOG(_logger, ReturnCode::RC_OUT_OF_BOUNDS);
        return std::nan("1");
    }
    return _data[index];
}

ReturnCode IVectorImpl::setCoord(size_t index, double value) const {
    if (index >= _dim || index < 0) {
        LOG(_logger, ReturnCode::RC_OUT_OF_BOUNDS);
        return ReturnCode::RC_INVALID_PARAMS;
    }
    if (std::isnan(value) || std::isinf(value)) {
        LOG(_logger, ReturnCode::RC_NAN);
        return ReturnCode::RC_NAN;
    }
    _data[index] = value;
    return ReturnCode::RC_SUCCESS;
}

double IVectorImpl::norm(Norm norm) const {
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

IVector * IVectorImpl::clone() const {
    return createVector(_dim, _data, _logger);
}

IVectorImpl::~IVectorImpl() {
    if (_data != nullptr) {
        delete[] _data;
        _data = nullptr;
    }
    if(_logger != nullptr) {
        _logger->releaseLogger(this);
    }
}