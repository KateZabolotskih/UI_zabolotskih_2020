#include <new>
#include <cmath>
#include <cstring>
#include "include/IVector.h"
#include "IVectorImpl.cpp"

static ReturnCode validateVectors(const IVector * vec1, const IVector * vec2, double accuracy = 0) {
    if (!vec1 || !vec2) {
        return ReturnCode::RC_NULL_PTR;
    }
    if (vec1->getDim() != vec2->getDim()) {
        return ReturnCode::RC_WRONG_DIM;
    }
    size_t dim = vec1->getDim();
    for (size_t i = 0; i < dim; ++i) {
        if (std::isnan(vec1->getCoord(i)) || std::isnan(vec2->getCoord(i))) {
            return ReturnCode::RC_NAN;
        }
    }
    if (std::isnan(accuracy)){
        return ReturnCode::RC_NAN;
    }
    if (accuracy < 0) {
        return ReturnCode::RC_INVALID_PARAMS;
    }
    return ReturnCode::RC_SUCCESS;
}

static ReturnCode validateVector(const IVector * vec) {
    if (!vec) {
        return ReturnCode::RC_NULL_PTR;
    }
    if (vec->getDim() == 0){
        return ReturnCode::RC_ZERO_DIM;
    }
    double dim = vec->getDim();
    for (int i = 0; i < dim; i++){
        if(std::isinf(vec->getCoord(i)) || std::isnan(vec->getCoord(i))){
            return ReturnCode::RC_NAN;
        }
    }
    return ReturnCode::RC_SUCCESS;
}

IVector * IVector::createVector(size_t dim, double * data, ILogger * logger) {
    if (!dim) {
        LOG(logger, ReturnCode::RC_ZERO_DIM);
        return nullptr;
    }
    if (!data) {
        LOG(logger, ReturnCode::RC_NULL_PTR);
        return nullptr;
    }
    for (int i = 0; i < dim; ++i) {
        if (std::isinf(data[i]) || std::isnan(data[i])) {
            LOG(logger, ReturnCode::RC_NAN);
            return nullptr;
        }
    }
    double * copy = new(std::nothrow) double[dim];
    if (!copy) {
        LOG(logger, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    std::memcpy(copy, data, dim * sizeof(double));
    IVector * result = (IVector *)new(std::nothrow) IVectorImpl(dim, copy);
    if (!result) {
        LOG(logger, ReturnCode::RC_NO_MEM);
        delete [] copy;
        return nullptr;
    }
    return result;
}

IVector * IVector::add(const IVector * vec1, const IVector * vec2, ILogger * logger) {
    auto r_code = validateVectors(vec1, vec2);
    if (r_code != ReturnCode::RC_SUCCESS) {
        LOG(logger, r_code);
        return nullptr;
    }

    size_t dim = vec1->getDim();
    auto * data = new(std::nothrow) double[dim];
    if (!data) {
        LOG(logger, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    for (int i = 0; i < dim; ++i) {
        data[i] = vec1->getCoord(i) + vec2->getCoord(i);
    }
    IVector * res_vec = createVector(dim, data, logger);
    if (validateVector(res_vec) != ReturnCode::RC_SUCCESS) {
        LOG(logger, validateVector(res_vec));
        delete res_vec;
        delete [] data;
        return nullptr;
    }
    delete [] data;
    return res_vec;
}

IVector * IVector::sub(const IVector * minuend, const IVector * subtrahend, ILogger * logger) {
    auto r_code = validateVectors(minuend, subtrahend);
    if (r_code != ReturnCode::RC_SUCCESS) {
        return nullptr;
    }
    size_t dim = minuend->getDim();
    auto * data = new(std::nothrow) double[dim];
    if (!data) {
        LOG(logger, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    for (int i = 0; i < dim; ++i) {
        data[i] = minuend->getCoord(i) - subtrahend->getCoord(i);
    }
    IVector * res_vec = createVector(dim, data, logger);
    if (validateVector(res_vec) != ReturnCode::RC_SUCCESS){
        LOG(logger, validateVector(res_vec));
        delete res_vec;
        delete [] data;
        return nullptr;
    }
    delete [] data;
    return res_vec;
}

IVector * IVector::mul(const IVector * multiplier, double scale, ILogger * logger) {
    auto r_code = validateVector(multiplier);
    if (r_code != ReturnCode::RC_SUCCESS) {
        LOG(logger, r_code);
        return nullptr;
    }
    if (std::isnan(scale) || std::isinf(scale)) {
        LOG(logger, ReturnCode::RC_NAN);
        return nullptr;
    }
    size_t dim = multiplier->getDim();
    auto * data = new(std::nothrow) double[dim];
    if (!data) {
        LOG(logger, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    for (int i = 0; i < dim; ++i) {
        data[i] = multiplier->getCoord(i) * scale;
    }
    IVector * res_vec = createVector(dim, data, logger);
    if (validateVector(res_vec) != ReturnCode::RC_SUCCESS){
        LOG(logger, validateVector(res_vec));
        delete res_vec;
        delete [] data;
        return nullptr;
    }
    delete [] data;
    return res_vec;
}

double IVector::mul(const IVector * multiplier1, const IVector * multiplier2, ILogger * logger) {
    auto r_code = validateVectors(multiplier1, multiplier2);
    if (r_code != ReturnCode::RC_SUCCESS) {
        LOG(logger, r_code);
        return std::nan("1");
    }
    double res = 0;
    for (size_t i = 0; i < multiplier1->getDim(); i++) {
        res += multiplier1->getCoord(i) * multiplier2->getCoord(i);
    }
    return res;
}

ReturnCode IVector::equals(const IVector * vec1, const IVector * vec2, Norm norm, double accuracy, bool & result, ILogger * logger) {
    auto r_code = validateVectors(vec1, vec2, accuracy);
    if (r_code != ReturnCode::RC_SUCCESS) {
        LOG(logger, r_code);
        result = false;
        return r_code;
    }
    result = false;

    IVector * diff = sub(vec1, vec2, logger);
    if (diff == nullptr)
        return ReturnCode::RC_NULL_PTR;

    double normValue = std::fabs(diff->norm(norm));
    if (std::isnan(normValue)) {
        return ReturnCode::RC_NAN;
    }
    if (normValue < accuracy) {
        result = true;
    }

    delete diff;
    return ReturnCode::RC_SUCCESS;
}

IVector::~IVector() {}