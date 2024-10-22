#pragma once
#include <boost/regex.hpp>
#include <muParser.h>
#include <string>
#include <vector>

#include "format"
using namespace std;

struct FractionEquation {
	string numerator;
	string denominator;
};

class Root{
private:

public:

	virtual FractionEquation parseEquation() = 0;
    // function for found the root of equation
	virtual void solveEquation() = 0;
	virtual string require_answer() = 0;
    virtual ~Root() {}
};

