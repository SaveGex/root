#include "Quadratic_Equations.h"

#include <boost/regex.hpp>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace {
double coefficient(const std::string& value) {
    if (value.empty() || value == "+") return 1.0;
    if (value == "-") return -1.0;
    const double result = std::stod(value);
    if (!std::isfinite(result)) throw std::invalid_argument("Coefficients must be finite.");
    return result;
}
}

QuadraticCoefficients Quadratic_Equations::parseEquation() const {
    std::string compact;
    for (unsigned char character : equation_) {
        if (!std::isspace(character)) compact += static_cast<char>(character);
    }
    // Preserve support for the original examples written as '+ -3*x'.
    for (auto pos = compact.find("+-"); pos != std::string::npos; pos = compact.find("+-")) {
        compact.replace(pos, 2, "-");
    }
    const std::string number = R"((?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?)";
    const boost::regex expression("^([+-]?(?:" + number + ")?)\\*?x\\^2([+-](?:" +
                                  number + ")?)\\*?x([+-]" + number + ")=0$");
    boost::smatch match;
    if (!boost::regex_match(compact, match, expression)) {
        throw std::invalid_argument("Expected a*x^2 + b*x + c = 0 (include all three terms).");
    }
    return {coefficient(match[1].str()), coefficient(match[2].str()), coefficient(match[3].str())};
}

void Quadratic_Equations::solveEquation() {
    result_.clear();
    roots_.clear();
    auto [a, b, c] = parseEquation();
    if (a == 0.0) {
        if (b != 0.0) {
            const double root = -c / b;
            if (!std::isfinite(root)) throw std::overflow_error("Root is outside the supported numeric range.");
            roots_.push_back(root);
        } else {
            result_ = equation_ + (c == 0.0 ? ": Infinitely many solutions" : ": No solution");
            return;
        }
    } else {
        // Scaling avoids overflow in b*b and 4*a*c for large finite coefficients.
        const double scale = std::max({std::abs(a), std::abs(b), std::abs(c)});
        a /= scale;
        b /= scale;
        c /= scale;
        if (a == 0.0) throw std::overflow_error("Coefficient range is too large.");
        const double discriminant = b * b - 4.0 * a * c;
        const double tolerance = 8.0 * std::numeric_limits<double>::epsilon() *
                                 (b * b + std::abs(4.0 * a * c));
        if (discriminant < -tolerance) {
            result_ = equation_ + ": No real roots";
            return;
        }
        if (std::abs(discriminant) <= tolerance) {
            roots_.push_back(-b / (2.0 * a));
        } else {
            // Avoid subtracting nearly equal numbers when calculating one root.
            const double q = -0.5 * (b + std::copysign(std::sqrt(discriminant), b));
            roots_ = {q / a, c / q};
            std::sort(roots_.begin(), roots_.end());
        }
        for (double root : roots_) {
            if (!std::isfinite(root)) throw std::overflow_error("Root is outside the supported numeric range.");
        }
    }
    result_ = equation_ + ": ";
    if (roots_.size() == 1) result_ += "x = " + formatNumber(roots_[0]);
    else result_ += "x1 = " + formatNumber(roots_[0]) + ", x2 = " + formatNumber(roots_[1]);
}

void Quadratic_Equations::change_equation(std::string expression) {
    equation_ = std::move(expression);
    result_.clear();
    roots_.clear();
}
