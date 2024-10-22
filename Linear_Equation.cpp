#include "Linear_Equation.h"

FractionEquation Linear_Equation::parseEquation() {
    // Оновлений регулярний вираз для дробових рівнянь
    boost::regex expr(R"(\(\s*([^/]+)\s*\)\s*/\s*\(\s*([^\)]+)\s*\)\s*=\s*0)");
    boost::smatch matches;

    if (boost::regex_search(equation, matches, expr)) {
        FractionEquation fracEq;
        fracEq.numerator = matches[1].str();
        fracEq.denominator = matches[2].str();
        return fracEq;
    }
    throw invalid_argument("Invalid equation format");
}

void Linear_Equation::solveEquation() {
    FractionEquation fracEq = parseEquation();
    mu::Parser p;
    double x = 0.0;
    p.DefineVar("x", &x);


    // Check zero numerator
    p.SetExpr(fracEq.numerator);
    if (p.Eval() == 0.0) {
        result += to_string(x); // Root found
        return;
    }

    // Solve root of numerator
    p.SetExpr(fracEq.numerator);
    for (double test_x = -100.0; test_x <= 100.0; test_x += 0.01) {
        x = test_x;
        if (abs(p.Eval()) < 1e-3) {
            result += format("\n{}: {}",this->equation, to_string(x));
            return;
        }
    }

    throw runtime_error("Root not found within the range");
}
