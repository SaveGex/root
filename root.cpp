#include "root.h"

#include <muParser.h>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>

std::string formatNumber(double value) {
    std::ostringstream stream;
    stream << std::setprecision(10) << (value == 0.0 ? 0.0 : value);
    return stream.str();
}

double findGridRoot(const std::string& left, const std::string& right,
                    const std::string& denominator) {
    double x = 0.0;
    mu::Parser leftParser, rightParser, denominatorParser;
    for (auto* parser : {&leftParser, &rightParser, &denominatorParser}) {
        parser->DefineVar("x", &x);
    }
    try {
        leftParser.SetExpr(left);
        rightParser.SetExpr(right);
        denominatorParser.SetExpr(denominator);
        // Integer indexing avoids accumulated error from repeatedly adding 0.01.
        for (int sample = 0; sample <= 20000; ++sample) {
            x = -100.0 + sample * 0.01;
            const double divisor = denominatorParser.Eval();
            if (!std::isfinite(divisor) || std::abs(divisor) <= 1e-12) continue;
            const double lhs = leftParser.Eval();
            const double rhs = rightParser.Eval();
            if (std::isfinite(lhs) && std::isfinite(rhs) && std::abs(lhs - rhs) <= 1e-7) {
                return x;
            }
        }
    } catch (const mu::Parser::exception_type& error) {
        throw std::invalid_argument("Invalid expression: " + error.GetMsg());
    }
    throw std::runtime_error("No root found on the grid [-100, 100] (step 0.01). "
                             "This does not prove that no root exists.");
}
