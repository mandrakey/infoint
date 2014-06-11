#ifndef ALGORITHM_HPP_
#define ALGORITHM_HPP_

#include "containment.hpp"
#include "model/Query.hpp"
#include <memory>
using std::unique_ptr;

class Algorithm
{
public:
	virtual ~Algorithm() {}

    virtual bool matches(const QueryPtr q1, const QueryPtr q2) = 0;
};

#endif /* ALGORITHM_HPP_ */
