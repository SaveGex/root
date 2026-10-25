#pragma once

#include "root.h"
#include <utility>

// Historical name: solves a fraction equal to zero, not only linear equations.
class Linear_Equation : public Root {
public:
    explicit Linear_Equation(std::string equation = "(x^2 - 4) / (x + 2) = 0")
        : equation_(std::move(equation)) {}
    FractionEquation parseEquation() const;
    void solveEquation() override;

private:
    std::string equation_;
};
