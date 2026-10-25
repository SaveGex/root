#pragma once

#include "root.h"
#include <utility>

class Root_from_Root : public Root {
public:
    explicit Root_from_Root(std::string equation = "2.3*x - 5 = 7 - 0.1*x")
        : equation_(std::move(equation)) {}
    void solveEquation() override;

private:
    std::string equation_;
};
