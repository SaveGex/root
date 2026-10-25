#include "Linear_Equation.h"
#include "Quadratic_Equations.h"
#include "Root_from_Root.h"

#include <cmath>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void check(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}
void near(double actual, double expected, double tolerance = 1e-8) {
    check(std::isfinite(actual) && std::abs(actual - expected) <= tolerance,
          "Expected " + std::to_string(expected) + ", got " + std::to_string(actual));
}
void expectRoots(Root& solver, const std::vector<double>& expected) {
    solver.solveEquation();
    check(solver.roots().size() == expected.size(), "Wrong number of roots");
    for (std::size_t i = 0; i < expected.size(); ++i) near(solver.roots()[i], expected[i]);
}
template<class Error, class Action>
void throws(Action action) {
    try { action(); }
    catch (const Error&) { return; }
    throw std::runtime_error("Expected an exception");
}
}

int main() {
    const std::vector<std::pair<std::string, std::function<void()>>> tests = {
        {"quadratic: two roots and implicit leading coefficient", [] {
            Quadratic_Equations s("x^2 - 3*x + 2 = 0"); expectRoots(s, {1, 2});
        }},
        {"quadratic: original plus-negative syntax", [] {
            Quadratic_Equations s("1 * x^2 + -3 * x + 2 = 0"); expectRoots(s, {1, 2});
        }},
        {"quadratic: repeated root", [] {
            Quadratic_Equations s("x^2 - 2*x + 1 = 0"); expectRoots(s, {1});
        }},
        {"quadratic: no real roots", [] {
            Quadratic_Equations s("4*x^2 - 3*x + 1 = 0"); expectRoots(s, {});
            check(s.require_answer().find("No real roots") != std::string::npos, "Missing explanation");
        }},
        {"quadratic: zero leading coefficient becomes linear", [] {
            Quadratic_Equations s("0*x^2 + 2*x - 4 = 0"); expectRoots(s, {2});
        }},
        {"quadratic: identity and contradiction", [] {
            Quadratic_Equations s("0*x^2 + 0*x + 0 = 0"); expectRoots(s, {});
            check(s.require_answer().find("Infinitely many") != std::string::npos, "Missing identity");
            s.change_equation("0*x^2 + 0*x + 1 = 0"); expectRoots(s, {});
            check(s.require_answer().find("No solution") != std::string::npos, "Missing contradiction");
        }},
        {"quadratic: decimal coefficients", [] {
            Quadratic_Equations s(".5*x^2 - 1.5*x + 1 = 0"); expectRoots(s, {1, 2});
        }},
        {"quadratic: negative leading coefficient", [] {
            Quadratic_Equations s("-x^2 + 3*x - 2 = 0"); expectRoots(s, {1, 2});
        }},
        {"quadratic: large finite coefficients", [] {
            Quadratic_Equations s("1e200*x^2 - 3e200*x + 2e200 = 0"); expectRoots(s, {1, 2});
        }},
        {"quadratic: cancellation-sensitive roots", [] {
            Quadratic_Equations s("x^2 + 1e8*x + 1 = 0"); s.solveEquation();
            check(s.roots().size() == 2, "Expected two roots");
            near(s.roots()[0], -1e8, 1e-6); near(s.roots()[1], -1e-8, 1e-15);
        }},
        {"quadratic: invalid input is rejected completely", [] {
            for (const auto* input : {"garbage x^2 - 3*x + 2 = 0", "x^2 - 3*x + 2 = 0 trailing", "x^2 = 2", "x^2 + x + = 0"}) {
                Quadratic_Equations s(input); throws<std::invalid_argument>([&] { s.solveEquation(); });
            }
        }},
        {"quadratic: repeated calls do not append old results", [] {
            Quadratic_Equations s; expectRoots(s, {1, 2});
            const auto first = s.require_answer(); expectRoots(s, {1, 2});
            check(first == s.require_answer(), "Result accumulated");
            s.change_equation("invalid"); throws<std::invalid_argument>([&] { s.solveEquation(); });
            check(s.roots().empty() && s.require_answer().empty(), "Stale result after failure");
        }},
        {"fraction: reject cancelled root at zero denominator", [] {
            Linear_Equation s("(x^2 - 4) / (x + 2) = 0"); expectRoots(s, {2});
        }},
        {"fraction: x/x has no root", [] {
            Linear_Equation s("(x) / (x) = 0"); throws<std::runtime_error>([&] { s.solveEquation(); });
        }},
        {"fraction: zero denominator everywhere", [] {
            Linear_Equation s("(x - 1) / (0) = 0"); throws<std::runtime_error>([&] { s.solveEquation(); });
        }},
        {"fraction: valid zero root", [] {
            Linear_Equation s("(x) / (x + 1) = 0"); expectRoots(s, {0});
        }},
        {"fraction: malformed or unsupported syntax", [] {
            Linear_Equation s("(x) / (x + 1) = 0 junk");
            throws<std::invalid_argument>([&] { s.solveEquation(); });
        }},
        {"numeric: original examples", [] {
            Root_from_Root a("2.3*x - 5 = 7 - 0.1*x"); expectRoots(a, {5});
            Root_from_Root b("2*x - 12 = 6 - x"); expectRoots(b, {6});
            Root_from_Root c("3*(x - 5) = 18"); expectRoots(c, {11});
        }},
        {"numeric: endpoints included", [] {
            Root_from_Root a("x = -100"); expectRoots(a, {-100});
            Root_from_Root b("x = 100"); expectRoots(b, {100});
        }},
        {"numeric: expressions undefined on part of grid", [] {
            Root_from_Root s("sqrt(x) = 2"); expectRoots(s, {4});
        }},
        {"numeric: off-grid root reports search limitation", [] {
            Root_from_Root s("3*x = 1"); throws<std::runtime_error>([&] { s.solveEquation(); });
        }},
        {"numeric: invalid expression and repeated equals", [] {
            Root_from_Root a("x = 1 = 2"); throws<std::invalid_argument>([&] { a.solveEquation(); });
            Root_from_Root b("unknown(x) = 1"); throws<std::invalid_argument>([&] { b.solveEquation(); });
        }},
        {"numeric: repeated calls are independent", [] {
            Root_from_Root s("x + 5 = 7"); expectRoots(s, {2});
            const auto first = s.require_answer(); expectRoots(s, {2});
            check(s.require_answer() == first, "Result accumulated");
        }}
    };
    int failures = 0;
    for (const auto& [name, test] : tests) {
        try { test(); std::cout << "PASS: " << name << '\n'; }
        catch (const std::exception& error) {
            ++failures;
            std::cerr << "FAIL: " << name << ": " << error.what() << '\n';
        }
    }
    std::cout << (tests.size() - failures) << '/' << tests.size() << " cases passed\n";
    return failures == 0 ? 0 : 1;
}
