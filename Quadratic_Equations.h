#pragma once

#include "root.h"
#include <utility>

struct QuadraticCoefficients {
    double a;
    double b;
    double c;
};

class Quadratic_Equations : public Root {
public:
    explicit Quadratic_Equations(std::string equation = "x^2 - 3*x + 2 = 0")
        : equation_(std::move(equation)) {}
    QuadraticCoefficients parseEquation() const;
    void solveEquation() override;
    void change_equation(std::string expression);

private:
    std::string equation_;
};
