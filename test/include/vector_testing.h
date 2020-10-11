#ifndef UI_ZABOLOTSKIH_2020_IVECTOR_H
#define UI_ZABOLOTSKIH_2020_IVECTOR_H
#include "../../src/vector/include/IVector.h"
#include "../../src/util/ReturnCode.h"
#include "../../src/logger/include/ILogger.h"
#include<iostream>

ReturnCode _add_test(IVector * vec1, IVector * vec2, IVector * res, ILogger * logger);
ReturnCode _sub_test(IVector * minuend, IVector * subtrahend, IVector * res, ILogger * logger);
ReturnCode _mul_test(IVector * multiplier, double scale, IVector * res, ILogger * logger);
ReturnCode _mul_test(IVector * multiplier1, IVector * multiplier2, double res, ILogger * logger);

void vector_testing_run(ILogger * logger) {
    int flag = 0;
    size_t dim = 4;
    double scale = 2.5;
    double data1[4] =               { 1.5,    4.67,  2.754,  5.566 };
    double data2[4] =               { 3.6,    2.11,  5.443,  0.76  };
    double data_res_add[4] =        { 5.1,    6.78,  8.187,  6.326 };
    double data_res_sub[4] =        {-2.1,    2.56, -2.689,  4.806 };
    double data_res_mul_scale[4] =  {   4,  11.675,  6.885,  13.915};
    double data_res_mul = 34.473882;
    IVector * vec1 = IVector::createVector(dim, data1, logger);
    IVector * vec2 = IVector::createVector(dim, data2, logger);
    IVector * vec_res_add = IVector::createVector(dim, data_res_add, logger);
    IVector * vec_res_sub = IVector::createVector(dim, data_res_sub, logger);
    IVector * vec_res_mul_scale = IVector::createVector(dim, data_res_mul_scale, logger);

    if (_add_test(vec1, vec2, vec_res_add, logger) != ReturnCode::RC_SUCCESS) {
        std::cout << "vector addition testing failed";
        flag = 1;
    }
    if (_sub_test(vec1, vec2, vec_res_sub, logger) != ReturnCode::RC_SUCCESS) {
        std::cout << "vector subtraction testing failed";
        flag = 1;
    }
    if (_mul_test(vec1, scale, vec_res_mul_scale, logger) != ReturnCode::RC_SUCCESS){
        std::cout << "vector multiplication on scale testing failed";
        flag = 1;
    }
    if (_mul_test(vec1, vec2, data_res_mul, logger) != ReturnCode::RC_SUCCESS) {
        std::cout << "vector multiplication testing failed";
        flag = 1;
    }

    if (flag == 0) {
        std::cout << "IVector testing passed successfully";
    } else {
        std::cout << "IVector testing failed";
    }
}
#endif //UI_ZABOLOTSKIH_2020_IVECTOR_H
