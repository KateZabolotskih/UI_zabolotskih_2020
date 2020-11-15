#ifndef ISET_H
#define ISET_H

#include "ILogger.h"
#include "IVector.h"
#include "../../../Downloads/UI_labs_2020_testing_prj-main/UI_labs_2020_testing_prj-main/headers/ReturnCode.h"
#include "../../../Downloads/UI_labs_2020_testing_prj-main/UI_labs_2020_testing_prj-main/headers/Export.h"
#include <cstddef>

class DECLSPEC ISet {
public:
	static ISet * createSet(ILogger * logger = nullptr);
	static ISet * _union(ISet const * set1, ISet const * set2, IVector::Norm norm, double accuracy, ILogger * logger = nullptr);
	static ISet * difference(ISet const * minuend, ISet const * subtrahend, IVector::Norm norm, double accuracy, ILogger * logger = nullptr);
	static ISet * symmetricDifference(ISet const * set1, ISet const * set2, IVector::Norm norm, double accuracy, ILogger * logger = nullptr);
	static ISet * intersection(ISet const * set1, ISet const * set2, IVector::Norm norm, double accuracy, ILogger * logger = nullptr);

	virtual ReturnCode insert(IVector const * vector, IVector::Norm norm, double accuracy) = 0;
	virtual ReturnCode erase(IVector const * vector, IVector::Norm norm, double accuracy)  = 0;
	virtual ReturnCode erase(size_t ind) 												   = 0;
	virtual void clear() 																   = 0;

	virtual ReturnCode find(IVector const * vector, IVector::Norm norm, double accuracy, size_t & ind) 	const = 0;
	virtual ReturnCode get(IVector *& dst, size_t ind) 													const = 0;
	virtual size_t getDim() 																			const = 0;
	virtual size_t getSize() 																			const = 0;
	virtual ISet * clone() 																				const = 0;

	ISet() = default;
	virtual ~ISet() = 0;

private:
	ISet(ISet const&) 			 = delete;
	ISet& operator=(ISet const&) = delete;
};

#endif /* ISET_H */
