#include "solution.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

// POINT REALIZATION

Point::Point() = default;
Point::Point(double x, double y) : _x(x), _y(y) {}

double Point::getX() const { return _x; }
double Point::getY() const { return _y; }

Point &Point::operator=(const Point &other) {
    if (this != &other) {
        _x = other._x;
        _y = other._y;
    }
    return *this;
}

Point &Point::operator=(Point &&other) noexcept {
    if (this != &other) {
        _x = other._x;
        _y = other._y;
    }
    return *this;
}

bool Point::operator==(const Point &other) const {
    return std::abs(_x - other._x) < std::numeric_limits<double>::epsilon() &&
           std::abs(_y - other._y) < std::numeric_limits<double>::epsilon();
}

std::ostream &operator<<(std::ostream &os, const Point &p) {
    os << "(" << p.getX() << ", " << p.getY() << ")";
    return os;
}

std::istream &operator>>(std::istream &is, Point &p) {
    double x, y;
    is >> x >> y;
    p = Point(x, y);
    return is;
}

// FIGURE REALIZATION

Figure::Figure() : _points_count(0), _points(nullptr) {}

Figure::Figure(const std::initializer_list<Point> &lst)
    : _points_count(lst.size()), _points(new Point[lst.size()]) {
    if (_points_count < 3) {
        throw std::runtime_error("Figure must have at least 3 points");
    }
    std::copy(lst.begin(), lst.end(), _points);
}

Figure::Figure(const Figure &other)
    : _points_count(other._points_count),
      _points(new Point[other._points_count]) {
    std::copy(other._points, other._points + other._points_count, _points);
}

Figure::Figure(Figure &&other) noexcept
    : _points_count(other._points_count), _points(other._points) {
    other._points_count = 0;
    other._points = nullptr;
}

Figure &Figure::operator=(const Figure &other) {
    if (this != &other) {
        delete[] _points;
        _points_count = other._points_count;
        _points = new Point[_points_count];
        std::copy(other._points, other._points + _points_count, _points);
    }
    return *this;
}

Figure &Figure::operator=(Figure &&other) noexcept {
    if (this != &other) {
        delete[] _points;
        _points_count = other._points_count;
        _points = other._points;
        other._points_count = 0;
        other._points = nullptr;
    }
    return *this;
}

bool Figure::operator==(const Figure &other) {
    if (_points_count != other._points_count) {
        return false;
    }
    for (size_t i = 0; i < _points_count; ++i) {
        if (_points[i] != other._points[i]) {
            return false;
        }
    }
    return true;
}

Figure::~Figure() noexcept { delete[] _points; }

Point Figure::get_barycenter() const {
    double sum_x = 0;
    double sum_y = 0;

    for (size_t i = 0; i < _points_count; ++i) {
        sum_x += _points[i].getX();
        sum_y += _points[i].getY();
    }

    return Point(sum_x / _points_count, sum_y / _points_count);
}

std::ostream &operator<<(std::ostream &os, const Figure &fig) {
    os << "Figure with " << fig._points_count << " points: {";
    for (size_t i = 0; i < fig._points_count; ++i) {
        os << fig._points[i] << (i < fig._points_count - 1 ? ", " : "");
    }
    os << "}";
    return os;
}

std::istream &operator>>(std::istream &is, Figure &fig) {
    size_t points_count;
    is >> points_count;

    delete[] fig._points;
    fig._points = new Point[points_count];
    fig._points_count = points_count;

    for (size_t i = 0; i < fig._points_count; ++i) {
        is >> fig._points[i];
    }
    return is;
}

Figure::operator double() const {
    double area = 0.0;
    for (size_t i = 0; i < _points_count; ++i) {
        size_t j = (i + 1) % _points_count;
        area += (_points[i].getX() * _points[j].getY() -
                 _points[j].getX() * _points[i].getY());
    }
    return std::abs(area) / 2.0;
}

// RECTANGLE REALIZATION

Rectangle::Rectangle(Point start, double width, double height)
    : _width(width), _height(height) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Width and height must be positive");
    }

    delete[] _points;
    _points_count = 4;
    _points = new Point[_points_count];

    _points[0] = start;
    _points[1] = Point(start.getX() + width, start.getY());
    _points[2] = Point(start.getX() + width, start.getY() + height);
    _points[3] = Point(start.getX(), start.getY() + height);
}

Rectangle::Rectangle(const std::initializer_list<Point> &lst) : Figure(lst) {
    if (lst.size() != 4) {
        throw std::invalid_argument("Rectangle must have 4 points");
    }

    _width = std::abs(lst.begin()->getX() - (lst.begin() + 1)->getX());
    _height = std::abs((lst.begin() + 1)->getY() - (lst.begin() + 2)->getY());
}

Rectangle::operator double() const { return _width * _height; }

// TRAPEZOID REALIZATION

Trapezoid::operator double() const {
    double area = 0.5 * (_base1 + _base2) * _height;
    return area;
}

Trapezoid::Trapezoid(const std::initializer_list<Point> &lst) : Figure(lst) {
    if (lst.size() != 4) {
        throw std::invalid_argument("Trapezoid must have 4 points");
    }

    auto it = lst.begin();
    Point p1 = *it;
    Point p2 = *(it + 1);
    Point p3 = *(it + 2);
    Point p4 = *(it + 3);

    if (std::abs(p1.getY() - p2.getY()) < std::numeric_limits<double>::epsilon()) {
        _base1 = std::abs(p1.getX() - p2.getX());
        _base2 = std::abs(p3.getX() - p4.getX());
        _height = std::abs(p1.getY() - p3.getY());
    } else if (std::abs(p2.getY() - p3.getY()) < std::numeric_limits<double>::epsilon()) {
        _base1 = std::abs(p2.getX() - p3.getX());
        _base2 = std::abs(p1.getX() - p4.getX());
        _height = std::abs(p2.getY() - p1.getY());
    } else {
        throw std::invalid_argument("The provided points do not form a trapezoid.");
    }
}

// RHOMBUS REALIZATION

Rhombus::Rhombus(Point center, double diagonal1, double diagonal2)
    : _diagonal1(diagonal1), _diagonal2(diagonal2) {
    if (diagonal1 <= 0 || diagonal2 <= 0) {
        throw std::invalid_argument("Diagonals must be positive");
    }

    delete[] _points;
    _points_count = 4;
    _points = new Point[_points_count];

    _points[0] = Point(center.getX() - diagonal1 / 2, center.getY());
    _points[1] = Point(center.getX(), center.getY() + diagonal2 / 2);
    _points[2] = Point(center.getX() + diagonal1 / 2, center.getY());
    _points[3] = Point(center.getX(), center.getY() - diagonal2 / 2);
}

Rhombus::operator double() const { return (_diagonal1 * _diagonal2) / 2; }
