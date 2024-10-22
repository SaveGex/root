#pragma once
#include "root.h"

using namespace std;

class Linear_Equation : public Root {
private:
    string equation = "(x^2 - 4) / (x + 2) = 0";
    string result = "";
public:
    FractionEquation parseEquation() override;
    void solveEquation() override;
    string require_answer() { return result; }
    ~Linear_Equation() {}
};
