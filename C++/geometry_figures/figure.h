#ifndef FIGURE_H
#define FIGURE_H

using namespace std;

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
};

class GeometricFigure {
public:
    virtual double calc_area() const = 0;
    virtual double calc_perimetr() const = 0;
    virtual void name() const = 0;
    virtual ~GeometricFigure() {}
};

class Circle : public GeometricFigure {
    Point center;
    double radius;
public:
    Circle(Point c, double r);
    double calc_area() const override;
    double calc_perimetr() const override;
    void name() const override;
};

class Ellipse : public GeometricFigure {
    Point center;
    double a, b;
public:
    Ellipse(Point c, double a, double b);
    double calc_area() const override;
    double calc_perimetr() const override;
    void name() const override;
};

class Triangle : public GeometricFigure {
    std::vector<Point> points;
public:
    Triangle(const std::vector<Point>& pts);
    double calc_area() const override;
    double calc_perimetr() const override;
    void name() const override;
};

class Rectangle : public GeometricFigure {
    std::vector<Point> points;
public:
    Rectangle(const std::vector<Point>& pts);
    double calc_area() const override;
    double calc_perimetr() const override;
    void name() const override;
};

class Polygon : public GeometricFigure {
    std::vector<Point> points;
public:
    Polygon(const std::string& filename);
    double calc_area() const override;
    double calc_perimetr() const override;
    void name() const override;
    int vertex_count() const;
};

#endif
