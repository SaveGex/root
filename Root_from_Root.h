#pragma once
#include "root.h"

class Root_from_Root : public Root{
private:
	string result = "";
	vector<std::string> examples = {
		string("2.3 * x - 5 = 7 - 0.1 * x"),
		string("2 * x - 12 = 6 - x"),
		string("3 * (x - 5) = 18"),
		string("x + 5 = 7")
	};
	size_t vector_size = examples.size();
	string equation ;
	size_t break_counter = 0;
public:

	FractionEquation parseEquation() override;
	void solveEquation() override;
	string require_answer() override { return result; }
	~Root_from_Root(){}
};

