#include "rational.h"
#include <cmath>

//Конструкторы
Rational::Rational(){
    number = 0;
    denom = 1;
}
Rational::Rational(int num){
    number = num;
    denom = 1;
}
Rational::Rational(int n, int d){
    number = n;
    denom = d;

    simplify();
}

//Корень
Rational sqrt(const Rational& r) {
    double sqrt_numer = sqrt(r.number);
    double sqrt_denom = sqrt(r.denom);

    return Rational((double)sqrt_numer, (double)sqrt_denom);
}

//Сокращение дроби
void Rational::simplify(){
    int a = abs(number);
    int b = abs(denom);

    while (b != 0)
    {
        int r = a % b;
        a = b;
        b = r;
    }

    number /= a;
    denom /= a;
}

//Сложение с присваиванием и сложения
Rational& Rational::operator +=(const Rational& r){
    number = (number * r.denom + denom * r.number);
    denom *= r.denom;
    simplify();

    return *this;
}
Rational Rational::operator +(const Rational& r) const{
    Rational res(*this);

    return res += r;
}

//Вычитание с присваиванием и вычитания
Rational& Rational::operator-=(const Rational& r) {
    return (*this += (-r));
}
Rational Rational::operator - () const{
    Rational r(-number, denom);

    return r;
}
Rational Rational::operator-(const Rational& r) const{
    Rational res(*this);

    return res -= r;
}

//Умножения с присваиванием и умножения
Rational& Rational::operator*=(const Rational& r){
    number *= r.number;
    denom *= r.denom;

    simplify();

    return *this;
}
Rational& Rational::operator *= (const int num) {
    number *= num;
    simplify();

    return *this;
}
Rational Rational::operator*(const Rational& r) const{
    Rational res(*this);

    return res *= r;
}
Rational operator*(const Rational& r, int num) {
    Rational res(r);

    return res *= num;
}
Rational operator*(int num, const Rational& r) {
    return r * num;
}

//Деление с присваиванием и деления
Rational& Rational::operator /= (const Rational& r) {
    number *= r.denom;
    denom *= r.number;

    simplify();

    return *this;
}
Rational operator /(const Rational& r, int num) {
    Rational res(r);

    return res /= num;
}
Rational operator /(int num, const Rational& r) {
    return r / num;
}
Rational Rational::operator / (const Rational& r) const{
    Rational res(*this);

    return res /= r;
}

//Инкрименты
Rational& Rational::operator ++(){
    number += denom;

    return *this;
}
Rational Rational::operator ++(int){
    Rational r(*this);
    number += denom;

    return r;
}

//Равенство и неравенство
bool Rational::operator ==(const Rational& r) const{
    return (number == r.number && denom == r.denom);
}
bool Rational::operator !=(const Rational& r) const{
    return !(*this == r);
}

//Приведения к int и double
Rational::operator int() const{
    return number / denom;
}
Rational::operator double() const{
    return ((double)number) / denom;
}

//Ввод и вывод
istream& operator >>(istream& in, Rational& r){
    in >> r.number >> r.denom;

    return in;
}
ostream& operator <<(ostream& out, const Rational& r){
    out << r.number << "/" << r.denom;

    return out;
}
