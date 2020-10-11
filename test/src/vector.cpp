#include "../include/vector_testing.h"

bool equal(IVector * vec1, IVector * vec2) {
    size_t dim1, dim2;
    dim1 = vec1->getDim();
    dim2 = vec2->getDim();
    if (dim1!= dim2) {
        return false;
    }
    for (int i = 0; i < dim1 ; i++) {
        for (int j = 0; j < dim2; j++) {
            if (vec1->getCoord(i) != vec1->getCoord(j))
                return false;
        }
    }
    return true;
}

ReturnCode _add_test(IVector * vec1, IVector * vec2, IVector * res, ILogger * logger) {
    IVector * result = IVector::add(vec1, vec2, logger);

    if (equal(result, res)) {
        return ReturnCode::RC_SUCCESS;
    } else {
        return ReturnCode::RC_UNKNOWN;
    }
}
ReturnCode _sub_test(IVector * minuend, IVector * subtrahend, IVector * res, ILogger * logger) {
    IVector * result = IVector::sub(minuend, subtrahend, logger);

    if (equal(result, res)) {
        return ReturnCode::RC_SUCCESS;
    } else {
        return ReturnCode::RC_UNKNOWN;
    }
}
ReturnCode _mul_test(IVector * multiplier, double scale, IVector * res, ILogger * logger) {
    IVector * result = IVector::mul(multiplier, scale, logger);

    if (equal(result, res)) {
        return ReturnCode::RC_SUCCESS;
    } else {
        return ReturnCode::RC_UNKNOWN;
    }
}
ReturnCode _mul_test(IVector * multiplier1, IVector * multiplier2, double res, ILogger * logger) {
    double result = IVector::mul(multiplier1, multiplier2, logger);

    if (result == res) {
        return ReturnCode::RC_SUCCESS;
    } else {
        return ReturnCode::RC_UNKNOWN;
    }
}


