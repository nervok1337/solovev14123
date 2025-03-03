#ifndef _RATIONAL_H
#define _RATIONAL_H
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

class Rational
{
private:
    void simplify();

public:
    int number;
    int denom;

    Rational();
    Rational(int num);
    Rational(int n, int d);

    friend Rational sqrt(const Rational& r);

    Rational& operator +=(const Rational& r);
    Rational operator +(const Rational& r) const;

    Rational& operator -= (const Rational& r);
    Rational operator - () const;
    Rational operator - (const Rational& r) const;

    Rational& operator *=(const Rational& r);
    Rational& operator *= (const int num);
    Rational operator * (const Rational& r) const;
    friend Rational operator*(const Rational& r, int num);
    friend Rational operator*(int num, const Rational& r);

    Rational& operator /= (const Rational& r);
    friend Rational operator / (const Rational& r, int num);
    friend Rational operator / (int num, const Rational& r);
    Rational operator / (const Rational& r) const;

    Rational& operator ++ ();
    Rational operator ++(int);

    operator int() const;
    operator double() const;

    bool operator == (const Rational& r) const;
    bool operator != (const Rational& r) const;

    friend istream& operator >>(istream& in, Rational& r);
    friend ostream& operator <<(ostream& out, const Rational& r);
};
#endif
