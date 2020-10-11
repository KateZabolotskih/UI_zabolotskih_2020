#include "../include/test.h"

bool equal(IVector * vec1, IVector * vec2) {
    size_t dim1, dim2;
    dim1 = vec1->getDim();
    dim2 = vec2->getDim();
    if (dim1!= dim2) {
        return false;
    }
    for (int i = 0, j = 0; i < dim1, j < dim2 ; i++, j++) {
        if (vec1->getCoord(i) != vec1->getCoord(j))
            return false;
    }
    return true;
}

ReturnCode _add_test() {
    size_t dim = 4;
    double data1[4] =               { 1.5,    4.67,  2.754,  5.566 };
    double data2[4] =               { 3.6,    2.11,  5.443,  0.76  };
    double data_res_add[4] =        { 5.1,    6.78,  8.187,  6.326 };
    IVector * vec1 = IVector::createVector(dim, data1, nullptr);
    IVector * vec2 = IVector::createVector(dim, data2, nullptr);
    IVector * vec_res_add = IVector::createVector(dim, data_res_add, nullptr);

    IVector * result = IVector::add(vec1, vec2, nullptr);

    if (equal(result, vec_res_add)) {
        return ReturnCode::RC_SUCCESS;
    } else {
        return ReturnCode::RC_UNKNOWN;
    }
}
ReturnCode _sub_test() {
    size_t dim = 4;
    double data1[4] =               { 1.5,    4.67,  2.754,  5.566 };
    double data2[4] =               { 3.6,    2.11,  5.443,  0.76  };
    double data_res_sub[4] =        {-2.1,    2.56, -2.689,  4.806 };
    IVector * vec1 = IVector::createVector(dim, data1, nullptr);
    IVector * vec2 = IVector::createVector(dim, data2, nullptr);
    IVector * vec_res_sub = IVector::createVector(dim, data_res_sub, nullptr);
    IVector * result = IVector::sub(vec1, vec2, nullptr);

    if (equal(result, vec_res_sub)) {
        return ReturnCode::RC_SUCCESS;
    } else {
        return ReturnCode::RC_UNKNOWN;
    }
}
ReturnCode _mul_onScale_test() {
    size_t dim = 4;
    double scale = 2.5;
    double data1[4] =               { 1.5,    4.67,  2.754,  5.566 };
    double data_res_mul_scale[4] =  {   4,  11.675,  6.885,  13.915};
    IVector * vec1 = IVector::createVector(dim, data1, nullptr);
    IVector * vec_res_mul_scale = IVector::createVector(dim, data_res_mul_scale, nullptr);
    IVector * result = IVector::mul(vec1, scale, nullptr);

    if (equal(result, vec_res_mul_scale)) {
        return ReturnCode::RC_SUCCESS;
    } else {
        return ReturnCode::RC_UNKNOWN;
    }
}
ReturnCode _mul_test() {
    size_t dim = 4;
    double data1[4] =               { 1.5,    4.67,  2.754,  5.566 };
    double data2[4] =               { 3.6,    2.11,  5.443,  0.76  };
    double sum = 0;

    for (int i = 0, j = 0; i < dim, j < dim; i++, j++) {
        sum += data1[i] * data2[j];
    }

    IVector * vec1 = IVector::createVector(dim, data1, nullptr);
    IVector * vec2 = IVector::createVector(dim, data2, nullptr);
    double result = IVector::mul(vec1, vec2, nullptr);

    if (result == sum) {
        return ReturnCode::RC_SUCCESS;
    } else {
        return ReturnCode::RC_UNKNOWN;
    }
}

void vector_testing_run() {
   int flag = 0;
   if (_add_test() != ReturnCode::RC_SUCCESS) {
       flag = 1;
       std::cout << "vector addition testing failed" << std::endl;
   }
   if (_sub_test() != ReturnCode::RC_SUCCESS) {
       flag = 1;
       std::cout << "vector subtraction testing failed" << std::endl;
   }
   if (_mul_onScale_test() != ReturnCode::RC_SUCCESS) {
       flag = 1;
       std::cout << "vector multiplication on scale testing failed" << std::endl;
   }
   if (_mul_test() != ReturnCode::RC_SUCCESS) {
       flag = 1;
       std::cout << "vector multiplication testing failed" << std::endl;
   }
    if (flag == 0) {
        std::cout << "IVector testing passed successfully" << std::endl;
    } else {
        std::cout << "IVector testing failed" << std::endl;
    }
}


