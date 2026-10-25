#pragma once

#include <string>
#include <vector>

struct FractionEquation {
    std::string numerator;
    std::string denominator;
};

class Root {
public:
    virtual ~Root() = default;
    virtual void solveEquation() = 0;
    const std::string& require_answer() const { return result_; }
    const std::vector<double>& roots() const { return roots_; }

protected:
    std::string result_;
    std::vector<double> roots_;
};

// Educational fixed-grid search: returns one approximate root, not all roots.
// The optional denominator excludes points outside a fraction's domain.
double findGridRoot(const std::string& left, const std::string& right,
                    const std::string& denominator = "1");
std::string formatNumber(double value);
