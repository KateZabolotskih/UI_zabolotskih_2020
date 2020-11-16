#include "../include/test.h"
#define FILE_NAME "Log_vector.txt"

ReturnCode _add_test(ILogger * logger) {
    size_t dim = 4;
    double data1[4] =               { 1.5,    4.67,  2.754,  5.566 };
    double data2[4] =               { 3.6,    2.11,  5.443,  0.76  };
    double data_res_add[4] =        { 5.1,    6.78,  8.187,  6.326 };
    IVector * vec1 = IVector::createVector(dim, data1, logger);
    IVector * vec2 = IVector::createVector(dim, data2, logger);
    IVector * vec_res_add = IVector::createVector(dim, data_res_add, logger);
    IVector * result = IVector::add(vec1, vec2, logger);

    bool res_bool = true;
    IVector::equals(result, vec_res_add, IVector::Norm::NORM_1, 0.0001, res_bool, logger);
    if (!res_bool) {
        return ReturnCode::RC_SUCCESS;
    } else {
        return ReturnCode::RC_UNKNOWN;
    }
}
ReturnCode _sub_test(ILogger * logger) {
    size_t dim = 4;
    double data1[4] =               { 1.5,    4.67,  2.754,  5.566 };
    double data2[4] =               { 3.6,    2.11,  5.443,  0.76  };
    double data_res_sub[4] =        {-2.1,    2.56, -2.689,  4.806 };
    IVector * vec1 = IVector::createVector(dim, data1, logger);
    IVector * vec2 = IVector::createVector(dim, data2, logger);
    IVector * vec_res_sub = IVector::createVector(dim, data_res_sub, logger);
    IVector * result = IVector::sub(vec1, vec2, logger);

    bool res_bool = true;
    IVector::equals(result, vec_res_sub, IVector::Norm::NORM_1, 0.0001, res_bool, logger);
    if (!res_bool) {
        return ReturnCode::RC_UNKNOWN;
    } else {
        return ReturnCode::RC_SUCCESS;
    }
}
ReturnCode _mul_onScale_test(ILogger * logger) {
    size_t dim = 4;
    double scale = 2.5;
    double data1[4] =               { 1.5,    4.67,  2.754,  5.566 };
    double data_res_mul_scale[4] =  {   4,  11.675,  6.885,  13.915};
    IVector * vec1 = IVector::createVector(dim, data1, logger);
    IVector * vec_res_mul_scale = IVector::createVector(dim, data_res_mul_scale, logger);
    IVector * result = IVector::mul(vec1, scale, logger);

    bool res_bool = true;
    IVector::equals(result, vec_res_mul_scale, IVector::Norm::NORM_1, 0.0001, res_bool, logger);
    if (!res_bool) {
        return ReturnCode::RC_SUCCESS;
    } else {
        return ReturnCode::RC_UNKNOWN;
    }
}
ReturnCode _mul_test(ILogger * logger) {
    size_t dim = 4;
    double data1[4] =               { 1.5,    4.67,  2.754,  5.566 };
    double data2[4] =               { 3.6,    2.11,  5.443,  0.76  };
    double sum = 0;

    for (int i = 0; i < dim; i++) {
        sum += data1[i] * data2[i];
    }

    IVector * vec1 = IVector::createVector(dim, data1, logger);
    IVector * vec2 = IVector::createVector(dim, data2, logger);
    double result = IVector::mul(vec1, vec2, logger);

    if (result == sum) {
        return ReturnCode::RC_SUCCESS;
    } else {
        return ReturnCode::RC_UNKNOWN;
    }
}

ReturnCode extra_test() {
    double data[3] = {1, 2, 3};
    IVector * vec1 = IVector::createVector(3, data, nullptr);
    IVector * res1 = IVector::mul(vec1, 2, nullptr);
    if (res1->getCoord(0) != 2 ||
        res1->getCoord(1) != 4 ||
        res1->getCoord(2) != 6) {
        return ReturnCode::RC_ELEM_NOT_FOUND;
    }
    return ReturnCode::RC_SUCCESS;
}

void vector_testing_run() {
    int client = 1;
    ILogger * logger = ILogger::createLogger(&client);
    logger->setLogFile(FILE_NAME);
    int flag = 0;
    if (_add_test(logger) != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "vector addition testing failed" << std::endl << std::flush;
    }
    if (_sub_test(logger) != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "vector subtraction testing failed" << std::endl << std::flush;
    }
    if (_mul_onScale_test(logger) != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "vector multiplication on scale testing failed" << std::endl << std::flush;
    }
    if (_mul_test(logger) != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "vector multiplication testing failed" << std::endl << std::flush;
    }
    if (flag == 0) {
        std::cout << "IVector testing passed successfully" << std::endl << std::flush;
    } else {
        std::cout << "IVector testing failed" << std::endl << std::flush;
    }
    extra_test();
    logger->releaseLogger(&client);
}


