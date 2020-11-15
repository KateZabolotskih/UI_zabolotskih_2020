#include "include/ICompact.h"
#include <cmath>
#include <new>
#include <algorithm>
#include <assert.h>
namespace {
    enum SEQUENCE {INVERSE = -1, EXPLICIT = 1};
    class ICompactImpl : public ICompact {
    private:
        size_t _dim {0};
        IVector * _begin {nullptr};
        IVector * _end {nullptr};
        ILogger * _logger {nullptr};
        double const _accuracy;

    public:
        class IteratorImpl : public ICompact::Iterator {
        private:
            IVector const * _begin {nullptr};
            IVector const * _end {nullptr};
            IVector const * _step {nullptr};
            IVector * _cur_point {nullptr};
            std::vector<size_t> _direction;
            SEQUENCE _orientation;
            ILogger * _logger {nullptr};
        public:
            ReturnCode setDirection(std::vector<size_t> const & direction)  override;
            ReturnCode doStep()                                             override;
            IVector * getPoint()                                      const override;

            IteratorImpl(IVector const * beg, IVector const * end, IVector const * step, std::vector<size_t> const & dir, SEQUENCE orientation);
            ~IteratorImpl();
        };

        Iterator * begin(IVector const * step)                            override;
        Iterator * end(IVector const * step)                              override;
        ICompact * clone()                                          const override;
        IVector * getBegin()                                        const override;
        IVector * getEnd()                                          const override;
        ReturnCode contains(IVector const * vec, bool & result)     const override;
        ReturnCode isSubset(ICompact const * anotherCopm, bool & result)   const override;
        ReturnCode intersects(ICompact const * anotherCopm, bool & result) const override;
        size_t getDim() const override;

        ICompactImpl(IVector const * begin, IVector const * end, double accuracy);
        ~ICompactImpl();
    };
}

ICompactImpl::IteratorImpl::IteratorImpl(IVector const * begin,
                                         IVector const * end,
                                         IVector const * step,
                                         std::vector<size_t> const & direction,
                                         SEQUENCE orientation) :
        _begin(begin),
        _end(end),
        _step(step),
        _direction(direction),
        _orientation(orientation) {
    if (_orientation == EXPLICIT) {
        _cur_point = begin->clone();
    } else {
        _cur_point = end->clone();
    }
    _logger = ILogger::createLogger(this);
}

ICompactImpl::IteratorImpl::~IteratorImpl() {
    if (_begin != nullptr) {
        delete _begin;
        _begin = nullptr;
    }
    if (_end != nullptr) {
        delete _end;
        _end = nullptr;
    }
    if (_cur_point!= nullptr) {
        delete _cur_point;
        _cur_point = nullptr;
    }
    if (_step != nullptr) {
        delete _step;
        _step = nullptr;
    }
    if (_logger != nullptr) {
        _logger->releaseLogger(this);
    }
}

ReturnCode ICompactImpl::IteratorImpl::setDirection(std::vector<size_t> const & direction) {
    if (direction.size() != _begin->getDim()) {
        LOG(_logger, ReturnCode::RC_WRONG_DIM);
    }
    std::vector<bool> bool_vec(direction.size(), false);
    for (auto dir : direction) {
        bool_vec[dir] = true;
    }
    bool have_all_dirs = true;
    for (auto exist_axis : bool_vec) {
        if (!exist_axis) {
            have_all_dirs = false;
            break;
        }
    }
    if (!have_all_dirs) {
        LOG(_logger, ReturnCode::RC_INVALID_PARAMS);
    }
    _direction = direction;
    return ReturnCode::RC_SUCCESS;
}

ReturnCode ICompactImpl::IteratorImpl::doStep() {
    size_t cur_axis = 0;
    double new_value;
    for (size_t i = 0; i < _begin->getDim(); i++) {
        size_t d = _direction[i];
        new_value = _cur_point->getCoord(d) + _orientation * _step->getCoord(d);
        if (_orientation == INVERSE && new_value < _begin->getCoord(d)) {
            _cur_point->setCoord(d, _end->getCoord(d));
        }
        if (_orientation == EXPLICIT && new_value > _end->getCoord(d)) {
            _cur_point->setCoord(d, _begin->getCoord(d));
        }
        else {
            _cur_point->setCoord(d, new_value);
            cur_axis = d;
            break;
        }
    }

    if (cur_axis >= _begin->getDim()) {
        LOG(_logger, ReturnCode::RC_OUT_OF_BOUNDS);
        delete _cur_point;
        if (_orientation == EXPLICIT) {
            _cur_point = _end->clone();
        } else {
            _cur_point = _begin->clone();
        }
        return ReturnCode::RC_OUT_OF_BOUNDS;
    }
    return ReturnCode::RC_SUCCESS;
}

IVector * ICompactImpl::IteratorImpl::getPoint() const {
    return _cur_point->clone();
}

static ReturnCode checkStep(IVector const * step, size_t dim, ILogger * logger) {
    if (step->getDim() != dim) {
        LOG(logger, ReturnCode::RC_WRONG_DIM);
        return ReturnCode::RC_WRONG_DIM;
    }

    for (size_t i = 0; i < dim; i++) {
        if (step->getCoord(i) <= 0.0) {
            LOG(logger, ReturnCode::RC_INVALID_PARAMS);
            return ReturnCode::RC_INVALID_PARAMS;
        }
    }

    return ReturnCode::RC_SUCCESS;
}

static ICompact::Iterator * createIterator(IVector const * _begin, IVector const * _end, IVector const * _step, SEQUENCE orientation, ILogger * logger) {
    if (_begin == nullptr || _end == nullptr || _step == nullptr) {
        LOG(logger, ReturnCode::RC_NULL_PTR);
        return nullptr;
    }

    IVector const * begin = _begin->clone();
    IVector const * end   = _end->clone();
    if (begin == nullptr) {
        LOG(logger, ReturnCode::RC_NULL_PTR);
        delete begin;
        return nullptr;
    }
    if (end == nullptr) {
        LOG(logger, ReturnCode::RC_NULL_PTR);
        delete end;
        return nullptr;
    }

    IVector * step;
    if (checkStep(_step, _begin->getDim(), logger) == ReturnCode::RC_SUCCESS) {
        step = _step->clone();
        if (step == nullptr) {
            delete begin;
            delete end;
            return nullptr;
        }
    } else {
        return nullptr;
    }

    std::vector<size_t> direction(begin->getDim());
    for (size_t cur_axis = 0; cur_axis < begin->getDim(); cur_axis++) {
        direction[cur_axis] = cur_axis;
    }
    ICompact::Iterator * iterator = new(std::nothrow) ICompactImpl::IteratorImpl(begin, end, step, direction, orientation);
    if (iterator == nullptr) {
        delete begin;
        delete end;
        delete step;
    }
    return iterator;
}


ICompact::Iterator * ICompactImpl::begin(IVector const * step) {
    return createIterator(_begin, _end, step, EXPLICIT, _logger);
}

ICompact::Iterator * ICompactImpl::end(IVector const * step) {
    return createIterator(_begin, _end, step, INVERSE, _logger);
}

ICompact* ICompactImpl::clone() const {
    return ICompact::createCompact(_begin, _end, _accuracy, _logger);
}

IVector * ICompactImpl::getBegin() const {
    return _begin->clone();
}

IVector * ICompactImpl::getEnd() const {
    return _end->clone();
}

ReturnCode ICompactImpl::contains(IVector const * vec, bool & result) const {
    if (vec == nullptr){
        LOG(_logger, ReturnCode::RC_NULL_PTR);
        return ReturnCode::RC_NULL_PTR;
    }
    if (vec->getDim() != _dim) {
        LOG(_logger, ReturnCode::RC_WRONG_DIM);
        return ReturnCode::RC_WRONG_DIM;
    }
    for (int i = 0; i < _dim; i++) {
        if (vec->getCoord(i) < _begin->getCoord(i) || vec->getCoord(i) > _end->getCoord(i)) {
            result = false;
            return ReturnCode::RC_SUCCESS;
        }
    }
    result = true;
    return ReturnCode::RC_SUCCESS;
}
static ReturnCode checkComp(ICompact const * anotherCopm, size_t dim, ILogger * logger) {
    if (anotherCopm == nullptr) {
        LOG(logger, ReturnCode::RC_NULL_PTR);
        return ReturnCode::RC_NULL_PTR;
    }

    if (anotherCopm->getDim() != dim) {
        LOG(logger, ReturnCode::RC_WRONG_DIM);
        return ReturnCode::RC_WRONG_DIM;
    }
    return ReturnCode::RC_SUCCESS;
}
ReturnCode ICompactImpl::isSubset(ICompact const * anotherCopm, bool & result) const {
    ReturnCode rc = checkComp(anotherCopm, _dim, _logger);
    if (rc != ReturnCode::RC_SUCCESS) {
        return rc;
    }
    bool contains_begin, contains_end;
    if ((rc = anotherCopm->contains(_begin, contains_begin)) != ReturnCode::RC_SUCCESS ||
            (rc = anotherCopm->contains(_end, contains_end)) != ReturnCode::RC_SUCCESS) {
        return rc;
    }
    if (contains_end == true && contains_begin == true) {
        result = true;
    } else {
        result = false;
    }
    return ReturnCode::RC_SUCCESS;
}

ReturnCode ICompactImpl::intersects(ICompact const * anotherCopm, bool & result) const {
    ReturnCode rc = checkComp(anotherCopm, _dim, _logger);
    if (rc != ReturnCode::RC_SUCCESS) {
        return rc;
    }
    auto * anotherComp_begin = anotherCopm->getBegin();
    auto * anotherComp_end = anotherCopm->getEnd();

    result = true;
    for (size_t i = 0; i < _dim; ++i) {
        double max_begin = std::max(anotherComp_begin->getCoord(i), _begin->getCoord(i));
        double min_end = std::min(anotherComp_end->getCoord(i), _end->getCoord(i));
        if (max_begin > min_end) {
            result = false;
        }
    }
    delete anotherComp_begin;
    delete anotherComp_end;
    return ReturnCode::RC_SUCCESS;
}

size_t ICompactImpl::getDim() const {
    return _dim;
}

ICompactImpl::ICompactImpl(IVector const * begin, IVector const * end, double accuracy) :
        _dim(begin->getDim()),
        _begin(begin->clone()),
        _end(end->clone()),
        _accuracy(accuracy) {
    _logger = ILogger::createLogger(this);
}

ICompactImpl::~ICompactImpl() {
    _dim = 0;
    if (_begin != nullptr) {
        delete _begin;
        _begin = nullptr;
    }
    if (_end != nullptr) {
        delete _end;
        _end = nullptr;
    }
    if (_logger != nullptr) {
        _logger->releaseLogger(this);
    }
}