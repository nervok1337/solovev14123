#include "figure.h"

// Circle
Circle::Circle(Point c, double r) : center(c), radius(r) {} // Конструктор круга
double Circle::calc_area() const { // Вычисление площади круга
    return M_PI * radius * radius;
}
double Circle::calc_perimetr() const { // Вычисление периметра круга
    return 2 * M_PI * radius;
}
void Circle::name() const {
    cout << "Figure: Circle\n";
}

// Ellipse
Ellipse::Ellipse(Point c, double a, double b) : center(c), a(a), b(b) {} // Конструктор эллипса
double Ellipse::calc_area() const { // Вычисление площади эллипса
    return M_PI * a * b;
}
double Ellipse::calc_perimetr() const { // Приблизительный периметр эллипса
    return 2 * M_PI * sqrt((a * a + b * b) / 2);
}
void Ellipse::name() const {
    cout << "Figure: Ellipse\n";
}

// Triangle
Triangle::Triangle(const vector<Point>& pts) : points(pts) {} // Конструктор треугольника
double Triangle::calc_area() const { // Площадь треугольника по формуле Герона
    double a = hypot(points[0].x - points[1].x, points[0].y - points[1].y);
    double b = hypot(points[1].x - points[2].x, points[1].y - points[2].y);
    double c = hypot(points[2].x - points[0].x, points[2].y - points[0].y);
    double p = (a + b + c) / 2;

    return sqrt(p * (p - a) * (p - b) * (p - c));
}
double Triangle::calc_perimetr() const { // Периметр треугольника
    double sum = 0;
    for (int i = 0; i < 3; ++i)
        sum += hypot(points[i].x - points[(i + 1) % 3].x, points[i].y - points[(i + 1) % 3].y);

    return sum;
}
void Triangle::name() const {
    cout << "Figure: Triangle\n";
}

// Rectangle
Rectangle::Rectangle(const vector<Point>& pts) : points(pts) {} // Конструктор прямоугольника
double Rectangle::calc_area() const { // Площадь прямоугольника
    double a = hypot(points[0].x - points[1].x, points[0].y - points[1].y);
    double b = hypot(points[1].x - points[2].x, points[1].y - points[2].y);

    return a * b;
}
double Rectangle::calc_perimetr() const { // Периметр прямоугольника
    double sum = 0;
    for (int i = 0; i < 4; ++i)
        sum += hypot(points[i].x - points[(i + 1) % 4].x, points[i].y - points[(i + 1) % 4].y);

    return sum;
}
void Rectangle::name() const {
    cout << "Figure: Rectangle\n";
}

// Polygon
Polygon::Polygon(const string& filename) { // Конструктор многоугольника: читает вершины из файла
    ifstream file(filename);
    if (!file) {
        cerr << "Error open file\n";
        return;
    }

    int n;
    file >> n;
    double x, y;
    for (int i = 0; i < n && file >> x >> y; ++i)
        points.emplace_back(x, y);
}
double Polygon::calc_area() const { // Площадь многоугольника методом шнуровки
    double area = 0;
    int n = points.size();
    for (int i = 0; i < n; ++i)
        area += (points[i].x * points[(i + 1) % n].y) - (points[(i + 1) % n].x * points[i].y);

    return fabs(area) / 2.0;
}
double Polygon::calc_perimetr() const { // Периметр многоугольника
    double perimetr = 0;
    int n = points.size();
    for (int i = 0; i < n; ++i)
        perimetr += hypot(points[i].x - points[(i + 1) % n].x, points[i].y - points[(i + 1) % n].y);

    return perimetr;
}
void Polygon::name() const {
    cout << "Figure: Polygon\n";
}
int Polygon::vertex_count() const { // Количество вершин многоугольника
    return points.size();
}

