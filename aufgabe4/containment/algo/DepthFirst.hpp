#ifndef DEPTHFIRST_HPP_
#define DEPTHFIRST_HPP_

#include "Algorithm.hpp"

class DepthFirst : public Algorithm
{
public:
	virtual bool matches(const Query& q1, const Query& q2);
};

#endif /* DEPTHFIRST_HPP_ */
