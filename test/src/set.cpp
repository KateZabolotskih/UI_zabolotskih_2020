#include "../include/test.h"
#define FILE_NAME "Log_set.txt"

void getParams(std::vector<IVector *> & vec_s, double & accuracy, IVector::Norm & norm, ILogger * logger) {
    accuracy = 1e-5;
    norm = IVector::Norm::NORM_1;

    const size_t dim1 = 4, dim2 = 6;
    double data1[dim1] = {1.5,        4.67, 2.754, 5.566};
    double data2[dim1] = {1.5 + 1e-8, 4.67, 2.754, 5.566};
    double data3[dim1] = {5.1,        6.78, 8.187, 6.326};

    double data4[dim2] = {-2.1, 2.56, -2.689, 4.806, 0.111, 1.5};
    double data5[dim2] = { 1.1, 5.89,  2.689, 0.0,   0.431, 1.5};

    IVector * vec1 = IVector::createVector(dim1, data1, logger);
    IVector * vec2 = IVector::createVector(dim1, data2, logger);
    IVector * vec3 = IVector::createVector(dim1, data3, logger);
    IVector * vec4 = IVector::createVector(dim2, data4, logger);
    IVector * vec5 = IVector::createVector(dim2, data5, logger);

    std::vector<IVector *> vec = {vec1, vec2, vec3, vec4, vec5};
    vec_s = vec;
}

void getTwoSets(ISet * set_1, ISet * set_2, double & accuracy, IVector::Norm & norm, ILogger * logger) {
    const size_t dim = 4;
    double data1_1[dim] = {1.5,   4.67,  2.754, 5.566};
    double data1_2[dim] = {7.12, -3.23,  1.11, -9.221};
    double data1_3[dim] = {5.1,   6.78,  8.187, 6.326};

    double data2_1[dim] = {1.123, 5.443, 7.653, 1.99 };
    double data2_2[dim] = {3.22,  2.432, 6.211, 9.34 };
    double data2_3[dim] = {5.1,   6.78,  8.187, 6.326};

    std::vector<IVector *> vecs_1, vecs_2;
    vecs_1.push_back(IVector::createVector(dim, data1_1, logger));
    vecs_1.push_back(IVector::createVector(dim, data1_2, logger));
    vecs_1.push_back(IVector::createVector(dim, data1_3, logger));

    vecs_2.push_back(IVector::createVector(dim, data2_1, logger));
    vecs_2.push_back(IVector::createVector(dim, data2_2, logger));
    vecs_2.push_back(IVector::createVector(dim, data2_3, logger));

    for (auto vec : vecs_1) {
        set_1->insert(vec, norm, accuracy);
    }
    for (auto vec : vecs_2) {
        set_2->insert(vec, norm, accuracy);
    }

    for (auto vec : vecs_1) {
        delete vec;
    }
    for (auto vec : vecs_2) {
        delete vec;
    }
}

void clear_vecs(std::vector<IVector *> & vec_s) {
    for (auto vec : vec_s) {
        delete vec;
    }
}

ReturnCode _insert_test(ILogger * logger) {
    std::vector<IVector *> vec_s;
    IVector::Norm norm;
    double accuracy;

    getParams(vec_s, accuracy, norm, logger);

    ISet * set = ISet::createSet(logger);

    auto r_code = set->insert(vec_s[0], norm, accuracy);
    if (r_code != ReturnCode::RC_SUCCESS || set->getDim() != vec_s[0]->getDim()) {
        return ReturnCode::RC_UNKNOWN;
    }

    r_code = set->insert(vec_s[1], norm, accuracy);
    if (r_code != ReturnCode::RC_SUCCESS || set->getSize() != 1) {
        return ReturnCode::RC_UNKNOWN;
    }

    r_code = set->insert(vec_s[3], norm, accuracy);
    if (r_code == ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }

    r_code = set->insert(nullptr, norm, accuracy);
    if (r_code == ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }

    delete set;
    clear_vecs(vec_s);
    vec_s.clear();

    return ReturnCode::RC_SUCCESS;
}

ReturnCode _erase_byVec_test(ILogger * logger) {
    std::vector<IVector *> vec_s;
    IVector::Norm norm;
    double accuracy;

    getParams(vec_s, accuracy, norm, logger);
    ISet * set = ISet::createSet(logger);

    set->insert(vec_s[3], norm, accuracy);
    set->insert(vec_s[4], norm, accuracy);

    auto r_code = set->erase(vec_s[3], norm, accuracy);
    if (r_code != ReturnCode::RC_SUCCESS || set->getSize() != 1) {
        return ReturnCode::RC_UNKNOWN;
    }

    r_code = set->erase(vec_s[1], norm, accuracy);
    if (r_code == ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }

    r_code = set->erase(vec_s[4], norm, accuracy);
    if (r_code != ReturnCode::RC_SUCCESS || set->getSize() != 0) {
        return ReturnCode::RC_UNKNOWN;
    }

    delete set;
    clear_vecs(vec_s);
    vec_s.clear();

    return ReturnCode::RC_SUCCESS;
}

ReturnCode _erase_byInd_test(ILogger * logger) {
    std::vector<IVector *> vec_s;
    IVector::Norm norm;
    double accuracy;

    getParams(vec_s, accuracy, norm, logger);
    ISet * set = ISet::createSet(logger);

    set->insert(vec_s[3], norm, accuracy);
    set->insert(vec_s[4], norm, accuracy);

    size_t index;
    ReturnCode r_code = set->find(vec_s[3], norm, accuracy, index);
    if (r_code == ReturnCode::RC_SUCCESS) {
        r_code = set->erase(index);
        if (r_code != ReturnCode::RC_SUCCESS || set->getSize() != 1) {
            return ReturnCode::RC_UNKNOWN;
        }
    } else {
        return ReturnCode::RC_UNKNOWN;
    }

    r_code = set->erase(10);
    if (r_code == ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }

    r_code = set->find(vec_s[4], norm, accuracy, index);
    if (r_code == ReturnCode::RC_SUCCESS) {
        r_code = set->erase(index);
        if (r_code != ReturnCode::RC_SUCCESS || set->getSize() != 0) {
            return ReturnCode::RC_UNKNOWN;
        }
    } else {
        return ReturnCode::RC_UNKNOWN;
    }

    delete set;
    clear_vecs(vec_s);
    vec_s.clear();
    return ReturnCode::RC_SUCCESS;
}

ReturnCode _find_test(ILogger * logger) {
    std::vector<IVector *> vec_s;
    IVector::Norm norm;
    double accuracy;

    getParams(vec_s, accuracy, norm, logger);
    ISet * set = ISet::createSet(logger);

    set->insert(vec_s[3], norm, accuracy);
    set->insert(vec_s[4], norm, accuracy);

    size_t index;
    ReturnCode r_code = set->find(vec_s[3], norm, accuracy, index);
    if (r_code != ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }

    r_code = set->find(vec_s[1], norm, accuracy, index);
    if (r_code == ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }
    r_code = set->find(nullptr, norm, accuracy, index);
    if (r_code == ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }

    r_code = set->find(vec_s[4], norm, accuracy, index);
    if (r_code != ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }
    delete set;
    clear_vecs(vec_s);
    vec_s.clear();
    return ReturnCode::RC_SUCCESS;
}

ReturnCode _union_test(ILogger * logger) {
    std::vector<IVector *> vec_s;
    IVector::Norm norm;
    double accuracy;

    getParams(vec_s, accuracy, norm, logger);

    ISet * set_1 = ISet::createSet(logger);
    ISet * set_2 = ISet::createSet(logger);

    getTwoSets(set_1, set_2, accuracy, norm, logger);

    ISet * uni_set = ISet::_union(set_1, set_2, norm, accuracy);
    if (uni_set->getSize() != 5) {
        return ReturnCode::RC_UNKNOWN;
    }
    size_t ind;
    if (uni_set->find(vec_s[0], norm, accuracy, ind) != ReturnCode::RC_SUCCESS ||
        uni_set->find(vec_s[4], norm, accuracy, ind) == ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }
    uni_set->insert(vec_s[1], norm, accuracy);
    if (uni_set->getSize() != 5) {
        return ReturnCode::RC_UNKNOWN;
    }
    delete set_1;
    delete set_2;
    delete uni_set;
    clear_vecs(vec_s);
    vec_s.clear();
    return ReturnCode::RC_SUCCESS;
}

ReturnCode _diff_test(ILogger * logger) {
    std::vector<IVector *> vec_s;
    IVector::Norm norm;
    double accuracy;
    size_t ind;

    getParams(vec_s, accuracy, norm, logger);
    ISet * set_extra = ISet::createSet(logger);
    set_extra->insert(vec_s[3], norm, accuracy);
    set_extra->insert(vec_s[4], norm, accuracy);

    ISet * set_1 = ISet::createSet(logger);
    ISet * set_2 = ISet::createSet(logger);

    getTwoSets(set_1, set_2, accuracy, norm, logger);

    ISet * diff_set = ISet::difference(set_1, set_2, norm, accuracy);
    if (diff_set->getSize() != 2) {
        return ReturnCode::RC_UNKNOWN;
    }
    ISet * diff_set_extra = ISet::difference(set_1, set_extra, norm, accuracy);
    if (diff_set_extra != nullptr) {
        return ReturnCode::RC_UNKNOWN;
    }
    if ((diff_set->find(vec_s[0], norm, accuracy, ind)) != ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }
    if ((diff_set->find(vec_s[2], norm, accuracy, ind)) == ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }

    delete set_1;
    delete set_2;
    delete diff_set;
    delete diff_set_extra;
    clear_vecs(vec_s);
    vec_s.clear();
    return ReturnCode::RC_SUCCESS;
}

ReturnCode _intersection_test(ILogger * logger) {
    const size_t dim = 4;
    double data_1[dim] = {1.123, 5.443, 7.653, 1.99 };
    double data_2[dim] = {3.22,  2.432, 6.211, 9.34 };
    IVector * vec_1 = IVector::createVector(dim, data_1);
    IVector * vec_2 = IVector::createVector(dim, data_2);

    std::vector<IVector *> vec_s;
    IVector::Norm norm;
    double accuracy;
    size_t ind;

    getParams(vec_s, accuracy, norm, logger);
    ISet * set_extra = ISet::createSet(logger);
    set_extra->insert(vec_s[3], norm, accuracy);
    set_extra->insert(vec_s[4], norm, accuracy);

    ISet * set_1 = ISet::createSet(logger);
    ISet * set_2 = ISet::createSet(logger);

    getTwoSets(set_1, set_2, accuracy, norm, logger);

    ISet * inter_set = ISet::intersection(set_1, set_2, norm, accuracy);
    if (inter_set->getSize() != 1) {
        return ReturnCode::RC_UNKNOWN;
    }
    ISet * inter_set_extra = ISet::intersection(set_1, set_extra, norm, accuracy);
    if (inter_set_extra != nullptr) {
        return ReturnCode::RC_UNKNOWN;
    }
    if ((inter_set->find(vec_s[0], norm, accuracy, ind)) == ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }
    if ((inter_set->find(vec_1, norm, accuracy, ind)) == ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }
    if ((inter_set->find(vec_2, norm, accuracy, ind)) == ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }
    if ((inter_set->find(vec_s[2], norm, accuracy, ind)) != ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }

    delete set_1;
    delete set_2;
    delete inter_set;
    delete inter_set_extra;
    clear_vecs(vec_s);
    vec_s.clear();
    return ReturnCode::RC_SUCCESS;
}

ReturnCode _symmDiff_test(ILogger * logger) {
    const size_t dim = 4;
    double data_1[dim] = {1.123, 5.443, 7.653, 1.99 };
    double data_2[dim] = {3.22,  2.432, 6.211, 9.34 };
    IVector * vec_1 = IVector::createVector(dim, data_1);
    IVector * vec_2 = IVector::createVector(dim, data_2);

    std::vector<IVector *> vec_s;
    IVector::Norm norm;
    double accuracy;
    size_t ind;

    getParams(vec_s, accuracy, norm, logger);
    ISet * set_extra = ISet::createSet(logger);
    set_extra->insert(vec_s[3], norm, accuracy);
    set_extra->insert(vec_s[4], norm, accuracy);

    ISet * set_1 = ISet::createSet(logger);
    ISet * set_2 = ISet::createSet(logger);

    getTwoSets(set_1, set_2, accuracy, norm, logger);

    ISet * summDiff_set = ISet::symmetricDifference(set_1, set_2, norm, accuracy);
    if (summDiff_set->getSize() != 4) {
        return ReturnCode::RC_UNKNOWN;
    }
    ISet * summDiff_set_extra = ISet::symmetricDifference(set_1, set_extra, norm, accuracy);
    if (summDiff_set_extra != nullptr) {
        return ReturnCode::RC_UNKNOWN;
    }
    if ((summDiff_set->find(vec_s[0], norm, accuracy, ind)) != ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }
    if ((summDiff_set->find(vec_1, norm, accuracy, ind)) != ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }
    if ((summDiff_set->find(vec_2, norm, accuracy, ind)) != ReturnCode::RC_SUCCESS) {
        return ReturnCode::RC_UNKNOWN;
    }

    delete set_1;
    delete set_2;
    delete summDiff_set;
    delete summDiff_set_extra;
    clear_vecs(vec_s);
    vec_s.clear();
    delete vec_1;
    delete vec_2;
    return ReturnCode::RC_SUCCESS;
}

ReturnCode _clone_test(ILogger * logger) {
    double accuracy = 1e-5;
    IVector::Norm norm = IVector::Norm::NORM_1;
    const size_t dim = 4;
    double data1[dim] = {1.5,   4.67,  2.754, 5.566};
    double data2[dim] = {7.12, -3.23,  1.11, -9.221};
    double data3[dim] = {5.1,   6.78,  8.187, 6.326};
    std::vector<IVector *> vecs;
    vecs.push_back(IVector::createVector(dim, data1, logger));
    vecs.push_back(IVector::createVector(dim, data2, logger));
    vecs.push_back(IVector::createVector(dim, data3, logger));
    ISet * set_1 = ISet::createSet(logger);
    for (auto vec : vecs) {
        set_1->insert(vec, norm, accuracy);
    }

    ISet * set_2 = set_1->clone();

    for (auto vec : vecs) {
        size_t ind;
        ReturnCode r_code = set_2->find(vec, norm, accuracy, ind);
        if (r_code != ReturnCode::RC_SUCCESS) {
            return ReturnCode::RC_UNKNOWN;
        }
    }
    delete set_1;
    delete set_2;
    clear_vecs(vecs);
    vecs.clear();
    return ReturnCode::RC_SUCCESS;
}


void set_testing_run() {
    int client = 2;
    ILogger * logger = ILogger::createLogger(&client);
    logger->setLogFile(FILE_NAME);
    int flag = 0;
    if (_insert_test(logger) != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "set insertion test failed" << std::endl;
    }
    if (_erase_byVec_test(logger) != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "set erase vector test failed" << std::endl;
    }
    if ( _erase_byInd_test(logger) != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "set erase vector by index test failed" << std::endl;
    }
    if (_find_test(logger) != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "set find test failed" << std::endl;
    }
    if (_union_test(logger) != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "set union test failed" << std::endl;
    }
    if (_diff_test(logger) != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "set difference test failed" << std::endl;
    }
    if (_symmDiff_test(logger) != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "set symmetric difference test failed" << std::endl;
    }
    if (_insert_test(logger) != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "set symmetric difference test failed" << std::endl;
    }
    if (_clone_test(logger) != ReturnCode::RC_SUCCESS) {
        flag = 1;
        std::cout << "set symmetric difference test failed" << std::endl;
    }
    if (flag == 0) {
        std::cout << "ISet testing passed successfully" << std::endl;
    } else {
        std::cout << "ISet testing failed" << std::endl;
    }
    logger->releaseLogger(&client);
}