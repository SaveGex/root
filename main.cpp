#include <iostream>
#include "root.h"
#include "Linear_Equation.h"
#include "Quadratic_Equations.h"
#include "Root_from_Root.h"
using namespace std;

int main() {
    //cout << "\033[31mATTENTION\033[0m" << endl;
    //cout << "NO HE DOESN'T SLEEP. HE TO COUNTING!!!" << endl;
    //Root* objects[3]{};
    //objects[0] = new Linear_Equation();
    //objects[1] = new Quadratic_Equations();
    //objects[2] = new Root_from_Root();
    //for (auto obj : objects) {
    //    obj->solveEquation();
    //    cout<<obj->require_answer();
    //}
    //for (auto obj : objects) {
    //    delete obj;
    //}
    ////code can recieve other equation. But i tired make of this... So you can just to change equation in classes.
    Quadratic_Equations quadratic_smthg{};
    quadratic_smthg.change_equation("4 * x^2 - 3 * x + 1 = 0");
    quadratic_smthg.solveEquation();
    cout << quadratic_smthg.require_answer();

    return 0;
}
