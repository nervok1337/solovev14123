#include <iostream>
#include <cmath>
#include "rational.h"
using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    Rational a, b, c;
    Rational D, x1, x2;

    //-------------------------------------------------
    while (true) {
        cout << "Enter k (y = kx+b): ";
        cin >> a;

        if (double(a) != 0)
            break;
        else
            cout << "Incorrect value\n";
    }
    cout << "Enter b (y=kx+b): ";
    cin >> b;

    x1 = -b / a;
    
    cout << "\nx = " << x1 << "\n";
    
    //-------------------------------------------------
    while (true) {
        cout << "\nEnter a (y = ax^2 + bx + c): ";
        cin >> a;

        if (double(a) != 0)
            break;
        else
            cout << "\nIncorrect value\n";
    }
    cout << "Enter b: ";
    cin >> b;

    cout << "Enter c: ";
    cin >> c;

    D = b * b - 4 * a * c;

    if ((double)D < 0)
        cout << "\nNo roots, D < 0\n";
    else if ((double)D >= 0) {

        x1 = (-b + sqrt(D)) / (2 * a);
        x2 = (-b - sqrt(D)) / (2 * a);

        cout << "\nRoots: x1 = " << x1 << ", x2 = " << x2 << '\n';
    }

    return 0;
}
