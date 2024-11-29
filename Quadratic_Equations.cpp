#include <map>
#include "Quadratic_Equations.h"

FractionEquation Quadratic_Equations::parseEquation() {
    boost::regex expr(R"(([-+]?\s*\d*\.?\d+)\s*\*\s*x\^2\s*([-+]\s*\-?\d*\.?\d*)\s*\*\s*x\s*([-+]\s*\-?\d*\.?\d*)\s*=\s*0)");
    boost::smatch matches;
//    //пройтись по виразу і якщо до пробілу є тільки знак видалити його з пробілом
//    auto sanitize{ [](string& expr) {
//        size_t enter_pos {};
//        bool digit = false;
//        bool space = false;
//        for (int i = 0; i < expr.size(); i++) {
//            if (expr[i] == ' ' && !space) {
//                space = true;
//                enter_pos = i;
//            }
//            if (isdigit(expr[i])) {
//                digit = true;
//            }
//            if (expr[i] == ' ' && space) {
//                expr.erase(0, enter_pos+1);
//                
//                return expr;
//            }
//        }
//}};

    if (boost::regex_search(this->equation, matches, expr)) {
        FractionEquation eq;
        eq.numerator = matches[1].str();
        eq.denominator = matches[2].str();
        // Зберігаємо третю змінну `c` в деномінатор для узгодженості
        eq.denominator += " " + matches[3].str();
        //sanitize(eq.denominator);
        return eq;
    }
    throw invalid_argument("Invalid equation format");
}


void Quadratic_Equations::solveEquation() {
    FractionEquation fracEq = parseEquation();

    auto space_pos{ [](string expr) {

        map<string, int> coords;
        bool space{}, digits{};
        int first_pos{-1}, last_pos{};
        for (int i = 0; i < expr.size(); i++) {
            if ((expr[i] == ' ' || expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '^' || isdigit(expr[i]) ) && first_pos == -1) {
                first_pos = i;
            }
            if (isdigit(expr[i])) {
                digits = true;
            }
            if (expr[i] == ' ' && first_pos) {
                space = true;
            }
            if (expr[i] == ' ' && !digits) {
                space = false;
                first_pos = -1;
            }
            else if (first_pos != -1 && (expr[i] == ' ' || i == expr.size()-1) && digits) {
                last_pos = i;
                coords["first"] = first_pos;
                coords["last"] = last_pos;

                return coords;
            }
        }
} };

    double a = std::stod(fracEq.numerator);
    double b = std::stod(fracEq.denominator.substr(space_pos(fracEq.denominator)["first"], space_pos(fracEq.denominator)["last"]));
    fracEq.denominator.erase(0, space_pos(fracEq.denominator)["last"]);
    double c = std::stod(fracEq.denominator.substr(space_pos(fracEq.denominator)["first"], space_pos(fracEq.denominator)["last"]));


    double D = b * b - 4 * a * c; // Обчислення дискримінанта

    if (D > 0) {
        double x1 = (-b + sqrt(D)) / (2 * a);
        double x2 = (-b - sqrt(D)) / (2 * a);
        result += format("\n{}: x1 = {}, x2 = {}", equation, to_string(x1), to_string(x2));
    }
    else if (D == 0) {
        double x = -b / (2 * a);
        result += format("\n{}: x = {}", equation, to_string(x));
    }
    else {
        result += format("\n{}: No real roots", equation);
    }
}

void Quadratic_Equations::change_equation(string expression)
{
    this->result.clear();
    this->equation = expression;
}

