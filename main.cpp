#include <iostream>
#include <boost/regex.hpp>
#include <muParser.h>

// Функція для знаходження кореня рівняння
double solveEquation(const std::string& equation) {
    // Регулярний вираз для розбиття рівняння на дві частини
    boost::regex expression("(.*)=(.*)");
    boost::smatch match;

    if (boost::regex_match(equation, match, expression)) {
        std::string leftSide = match[1];
        std::string rightSide = match[2];

        // Ініціалізуємо muParser
        mu::Parser parser;
        double x;  // Змінна для невідомого
        parser.DefineVar("x", &x);

        parser.SetExpr(leftSide);
        double leftValue = parser.Eval();

        parser.SetExpr(rightSide);
        double rightValue = parser.Eval();

        // Перебираємо варіанти для знаходження кореня
        double step = 0.01;  // Крок зміни x
        for (x = -1000; x < 1000; x += step) {
            parser.SetExpr(leftSide);
            double leftVal = parser.Eval();
            parser.SetExpr(rightSide);
            double rightVal = parser.Eval();

            if (std::abs(leftVal - rightVal) < 1e-3) {
                return x;  // Повертаємо корінь
            }
        }
    }

    throw std::runtime_error("Корінь не знайдено або рівняння не коректне");
}

int main() {
    std::string equation1 = "2.3*x-5=7-0.1*x";
    std::string equation2 = "3*(x-5)=18";
    std::string equation3 = "x+5=7";
    std::string equation4 = "2*x-12=6-x";

    try {
        std::cout << "Корінь рівняння 1: x = " << solveEquation(equation1) << std::endl;
        std::cout << "Корінь рівняння 2: x = " << solveEquation(equation2) << std::endl;
        std::cout << "Корінь рівняння 3: x = " << solveEquation(equation3) << std::endl;
        std::cout << "Корінь рівняння 4: x = " << solveEquation(equation4) << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Помилка: " << e.what() << std::endl;
    }

    return 0;
}
