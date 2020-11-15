#include "include/ISet.h"
#include "ISetImpl.cpp"

static ReturnCode validateSets(ISet const * set1, ISet const * set2, double accuracy) {
    if (set1 == nullptr || set2 == nullptr) {
        return ReturnCode::RC_NULL_PTR;
    }
    if (set1->getDim() == 0 || set2->getDim() == 0) {
        return ReturnCode::RC_ZERO_DIM;
    }
    if (set1->getDim() != set2->getDim()) {
        return ReturnCode::RC_WRONG_DIM;
    }
    if (std::isnan(accuracy) || accuracy < 0) {
        return ReturnCode::RC_INVALID_PARAMS;
    }

    return ReturnCode::RC_SUCCESS;
}

ISet * ISet::createSet(ILogger * logger) {
    ISet * set = new(std::nothrow) ISetImpl();
    if (set == nullptr) {
        LOG(logger, ReturnCode::RC_NO_MEM);
        return nullptr;
    }
    return set;
}

ISet * ISet::_union(ISet const * set1, ISet const * set2, IVector::Norm norm, double accuracy, ILogger * logger) {
    ReturnCode r_code = validateSets(set1, set2, accuracy);
    if (r_code != ReturnCode::RC_SUCCESS) {
        LOG(logger, r_code);
        return nullptr;
    }

    ISet * union_set = set1->clone();
    IVector * cur_vec = nullptr;

    for (size_t ind = 0; ind < set2->getSize(); ind++) {
        r_code = set2->get(cur_vec, ind);
        if (validateVector(cur_vec) == ReturnCode::RC_SUCCESS) {
            union_set->insert(cur_vec, norm, accuracy);
            delete cur_vec;
            cur_vec = nullptr;
        }
    }
    return union_set;
}

ISet * ISet::difference(ISet const * minuend, ISet const * subtrahend, IVector::Norm norm, double accuracy, ILogger * logger) {
    ReturnCode r_code = validateSets(minuend, subtrahend, accuracy);
    if (r_code != ReturnCode::RC_SUCCESS) {
        LOG(logger, r_code);
        return nullptr;
    }

    ISet * diff_set = minuend->clone();
    IVector * cur_vec = nullptr;

    for (size_t ind = 0; ind < subtrahend->getSize(); ind++) {
        r_code = subtrahend->get(cur_vec, ind);
        if (validateVector(cur_vec) == ReturnCode::RC_SUCCESS) {
            diff_set->erase(cur_vec, norm, accuracy);
            delete cur_vec;
            cur_vec = nullptr;
        }
    }
    return diff_set;
}

ISet * ISet::symmetricDifference(ISet const * set1, ISet const * set2, IVector::Norm norm, double accuracy, ILogger * logger) {
    ReturnCode r_code = validateSets(set1, set2, accuracy);
    if (r_code != ReturnCode::RC_SUCCESS) {
        LOG(logger, r_code);
        return nullptr;
    }

    ISet * union_set = _union(set1, set2, norm, accuracy, logger);
    ISet * inter_set = intersection(set1, set2, norm, accuracy, logger);

    ISet * result_set = difference(union_set, inter_set, norm, accuracy, logger);

    delete union_set;
    delete inter_set;

    return result_set;
}

ISet * ISet::intersection(ISet const * set1, ISet const * set2, IVector::Norm norm, double accuracy, ILogger * logger) {
    ReturnCode r_code = validateSets(set1, set2, accuracy);
    if (r_code != ReturnCode::RC_SUCCESS) {
        LOG(logger, r_code);
        return nullptr;
    }

    ISet * inter_set = set1->clone();
    IVector * cur_vec = nullptr;

    for (size_t ind = 0; ind < set1->getSize(); ind++) {
        r_code = set1->get(cur_vec, ind);
        if (validateVector(cur_vec) == ReturnCode::RC_SUCCESS) {
            if (set2->find(cur_vec, norm, accuracy, ind) != ReturnCode::RC_SUCCESS) {
                inter_set->erase(ind);
            }
            delete cur_vec;
            cur_vec = nullptr;
        }
    }
    return inter_set;
}

ISet::~ISet() {}