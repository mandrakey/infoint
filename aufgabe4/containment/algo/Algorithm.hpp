#ifndef ALGORITHM_HPP_
#define ALGORITHM_HPP_

class Query;

class Algorithm
{
public:
	virtual ~Algorithm() {}

	virtual bool matches(const Query& q1, const Query& q2) = 0;
};

#endif /* ALGORITHM_HPP_ */
