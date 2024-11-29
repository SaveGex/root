#pragma once
#include "root.h"

class Quadratic_Equations : public Root {
private:
    string equation = "1 * x^2 + -3 * x + 2 = 0";
    string result = "";

public:
    Quadratic_Equations(string equation) : equation{equation}{}
    Quadratic_Equations() {}
    ~Quadratic_Equations() {}
    FractionEquation parseEquation() override;
    void solveEquation() override;
    string require_answer() override { return result; }
    void change_equation(string expression);
};