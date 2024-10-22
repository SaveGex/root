#include "Quadratic_Equations.h"

FractionEquation Quadratic_Equations::parseEquation() {
    boost::regex expr(R"(([-+]?\d*\.?\d+)\s*\*\s*x\^2\s*([-+]?\s*\-?\d*\.?\d*)\s*\*\s*x\s*([-+]?\s*\-?\d*\.?\d*)\s*=\s*0)");
    boost::smatch matches;

    if (boost::regex_search(equation, matches, expr)) {
        FractionEquation eq;
        eq.numerator = matches[1].str();
        eq.denominator = matches[2].str();
        // «бер≥гаЇмо третю зм≥нну `c` в деном≥натор дл€ узгодженост≥
        eq.denominator += " " + matches[3].str();
        return eq;
    }
    throw invalid_argument("Invalid equation format");
}


void Quadratic_Equations::solveEquation() {
    FractionEquation fracEq = parseEquation();
    mu::Parser p;
    double x = 0.0;
    p.DefineVar("x", &x);


    // Check zero numerator
    p.SetExpr(fracEq.numerator);
    if (p.Eval() == 0.0) {
        result +=format("\n{}: {}",this->equation, to_string(x)); // Root found
        return;
    }

    // Solve root of numerator
    p.SetExpr(fracEq.numerator);
    for (double test_x = -100.0; test_x <= 100.0; test_x += 0.01) {
        x = test_x;
        if (abs(p.Eval()) < 1e-6) {
            result += format("\n{}: {}", this->equation, to_string(x));
            return;
        }
    }
}

