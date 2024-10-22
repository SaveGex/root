#pragma once
#include "root.h"

class Quadratic_Equations : public Root {
private:
    string equation = "1 * x^2 + -3 * x + 2 = 0";
    string result = "";
public:
    FractionEquation parseEquation() override;
    void solveEquation() override;
    string require_answer() override { return result; }
    ~Quadratic_Equations() {}
};