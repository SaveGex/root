#include "Linear_Equation.h"
#include "Quadratic_Equations.h"
#include "Root_from_Root.h"

#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {
void usage() {
    std::cout << "Usage: equation-solver [quadratic|fraction|numeric] \"equation\"\n"
                 "No arguments: run three examples.\n"
                 "Example: equation-solver quadratic \"x^2 - 3*x + 2 = 0\"\n";
}
void run(Root& solver) {
    solver.solveEquation();
    std::cout << solver.require_answer() << '\n';
}
}

int main(int argc, char* argv[]) {
    try {
        if (argc == 1) {
            std::vector<std::unique_ptr<Root>> examples;
            examples.push_back(std::make_unique<Quadratic_Equations>());
            examples.push_back(std::make_unique<Linear_Equation>());
            examples.push_back(std::make_unique<Root_from_Root>());
            for (auto& example : examples) run(*example);
            return 0;
        }
        if (argc == 2 && std::string(argv[1]) == "--help") {
            usage();
            return 0;
        }
        if (argc != 3) {
            usage();
            return 2;
        }
        std::unique_ptr<Root> solver;
        const std::string mode = argv[1];
        if (mode == "quadratic") solver = std::make_unique<Quadratic_Equations>(argv[2]);
        else if (mode == "fraction") solver = std::make_unique<Linear_Equation>(argv[2]);
        else if (mode == "numeric") solver = std::make_unique<Root_from_Root>(argv[2]);
        else {
            usage();
            return 2;
        }
        run(*solver);
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
