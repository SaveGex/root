#include "Root_from_Root.h"

#include <boost/regex.hpp>
#include <stdexcept>

void Root_from_Root::solveEquation() {
    result_.clear();
    roots_.clear();
    static const boost::regex expression(R"(^\s*([^=]+)\s*=\s*([^=]+)\s*$)");
    boost::smatch match;
    if (!boost::regex_match(equation_, match, expression)) {
        throw std::invalid_argument("Expected one equation: left expression = right expression.");
    }
    const double root = findGridRoot(match[1].str(), match[2].str());
    roots_.push_back(root);
    result_ = equation_ + ": x ~= " + formatNumber(root) + " (grid search)";
}
