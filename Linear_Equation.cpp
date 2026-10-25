#include "Linear_Equation.h"

#include <boost/regex.hpp>
#include <stdexcept>

FractionEquation Linear_Equation::parseEquation() const {
    // Deliberately limited grammar: no nested parentheses.
    static const boost::regex expression(R"(^\s*\(([^()]+)\)\s*/\s*\(([^()]+)\)\s*=\s*0\s*$)");
    boost::smatch match;
    if (!boost::regex_match(equation_, match, expression)) {
        throw std::invalid_argument("Expected (numerator) / (denominator) = 0; nested parentheses are not supported.");
    }
    return {match[1].str(), match[2].str()};
}

void Linear_Equation::solveEquation() {
    result_.clear();
    roots_.clear();
    const auto fraction = parseEquation();
    const double root = findGridRoot(fraction.numerator, "0", fraction.denominator);
    roots_.push_back(root);
    result_ = equation_ + ": x ~= " + formatNumber(root) + " (grid search)";
}
