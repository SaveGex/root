#include "Root_from_Root.h"
#include <string>


FractionEquation Root_from_Root::parseEquation()
{
    return FractionEquation();
}

void Root_from_Root::solveEquation() {
    if (examples.size() == this->vector_size && this->break_counter == 0) {
        this->break_counter++;
        for (int i = 0; i < this->vector_size; i++) {
            equation = examples[i];
            solveEquation();
        }
    }
    // regular equpation for break the equpation by two sides
    boost::regex expression("(.*)=(.*)");
    boost::smatch match;

    if (boost::regex_match(this->equation, match, expression)) {
        std::string leftSide = match[1];
        std::string rightSide = match[2];

        // Initializing muParser
        mu::Parser parser;
        double x;  // Variable for unknown
        parser.DefineVar("x", &x);

        parser.SetExpr(leftSide);
        double leftValue = parser.Eval();

        parser.SetExpr(rightSide);
        double rightValue = parser.Eval();

        // go through a variants for find out the root
        double step = 0.05;  // Step of x
        for (x = -100; x < 100; x += step) {
            parser.SetExpr(leftSide);
            double leftVal = parser.Eval();
            parser.SetExpr(rightSide);
            double rightVal = parser.Eval();

            if (std::abs(leftVal - rightVal) < 1e-2) {
                this->result += format("\n{}: {}",this->equation, std::to_string(x));
                return;
            }
        }
    }

    throw std::runtime_error("Root wasn't found or equpation isn't correct");
}

