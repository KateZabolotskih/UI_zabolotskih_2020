#include "../logger/include/ILogger.h"
#include "include/ISet.h"
#include <vector>
#include <cmath>

namespace {
    /* declaration */
    class SetImpl : public ISet {
    private:
        size_t m_dim {0};
        std::vector<IVector*> m_data;
        ILogger* m_logger {nullptr};

    public:
        SetImpl();
        ~SetImpl()                                                                                  override;

        ReturnCode insert(IVector const* vector, IVector::Norm norm, double tolerance) 	            override;
        ReturnCode erase(IVector const* vector, IVector::Norm norm, double tolerance) 	            override;
        ReturnCode erase(size_t index) 													            override;
        void clear() 																	            override;

        ReturnCode find(IVector const* vector, IVector::Norm norm, double tolerance, size_t& ind) 	const override;
        ReturnCode get(IVector*& dst, size_t ind) 													const override;
        size_t getDim() 																			const override;
        size_t getSize() 																			const override;
        ISet* clone() 																				const override;
    };
}

static void log(ILogger * logger, ReturnCode code) {
    if (!logger){
        return;
    }
    switch (code) {
        case ReturnCode::RC_WRONG_DIM:
            logger->log("ISet: dimension mismatch", ReturnCode::RC_WRONG_DIM);
            break;
        case ReturnCode::RC_ZERO_DIM:
            logger->log("ISet: zero dimension set", ReturnCode::RC_ZERO_DIM);
        case ReturnCode::RC_NO_MEM:
            logger->log("ISet: can't allocate memory", ReturnCode::RC_NO_MEM);
            break;
        case ReturnCode::RC_NULL_PTR:
            logger->log("ISet: can't work with nullptr", ReturnCode::RC_NULL_PTR);
            break;
        case ReturnCode::RC_INVALID_PARAMS:
            logger->log("ISet: invalid params", ReturnCode::RC_INVALID_PARAMS);
            break;
        case ReturnCode::RC_NAN:
            logger->log("ISet: value is missing", ReturnCode::RC_NAN);
            break;
        case ReturnCode::RC_OUT_OF_BOUNDS:
            logger->log("ISet: run out the bounds", ReturnCode::RC_OUT_OF_BOUNDS);
    }
}
