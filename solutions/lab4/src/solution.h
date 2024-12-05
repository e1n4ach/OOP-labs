#pragma once

#include <algorithm>
#include <cmath>
#include <concepts>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

template <typename T>
concept Scalar = std::is_arithmetic_v<T>;

template <Scalar T>
class Point {
public:
    Point() = default;
    Point(T x, T y);
    Point(const Point<T>& other) = default;
    Point(Point<T>&& other) noexcept = default;
    ~Point() = default;

    T get_x() const;
    T get_y() const;

    Point<T>& operator=(const Point<T>& other);
    Point<T>& operator=(Point<T>&& other) noexcept;
    bool operator==(const Point<T>& other) const;

    template <Scalar U>
    friend std::ostream& operator<<(std::ostream& os, const Point<U>& p);
    template <Scalar U>
    friend std::istream& operator>>(std::istream& is, Point<U>& p);

private:
    T _x = 0;
    T _y = 0;
};

template <Scalar T>
Point<T>::Point(T x, T y) : _x(x), _y(y) {}

template <Scalar T>
T Point<T>::get_x() const { return _x; }

template <Scalar T>
T Point<T>::get_y() const { return _y; }

template <Scalar T>
Point<T>& Point<T>::operator=(const Point<T>& other) {
    if (this != &other) {
        _x = other._x;
        _y = other._y;
    }
    return *this;
}

template <Scalar T>
Point<T>& Point<T>::operator=(Point<T>&& other) noexcept {
    if (this != &other) {
        _x = std::move(other._x);
        _y = std::move(other._y);
    }
    return *this;
}

template <Scalar T>
bool Point<T>::operator==(const Point<T>& other) const {
    if constexpr (std::is_floating_point_v<T>) {
        return std::abs(_x - other._x) < std::numeric_limits<T>::epsilon() &&
               std::abs(_y - other._y) < std::numeric_limits<T>::epsilon();
    } else {
        return _x == other._x && _y == other._y;
    }
}

template <Scalar T>
std::ostream& operator<<(std::ostream& os, const Point<T>& p) {
    os << "(" << p.get_x() << ", " << p.get_y() << ")";
    return os;
}

template <Scalar T>
std::istream& operator>>(std::istream& is, Point<T>& p) {
    T x, y;
    is >> x >> y;
    p = Point<T>(x, y);
    return is;
}

template <Scalar T>
class Figure {
public:
    Figure() = default;
    Figure(const std::initializer_list<Point<T>>& lst);
    Figure(const Figure<T>& other);
    Figure(Figure<T>&& other) noexcept = default;
    virtual ~Figure() noexcept = default;

    Figure<T>& operator=(const Figure<T>& other);
    Figure<T>& operator=(Figure<T>&& other) noexcept = default;
    bool operator==(const Figure<T>& other);

    virtual Point<T> get_barycenter() const;
    virtual explicit operator double() const;

    template <Scalar U>
    friend std::ostream& operator<<(std::ostream& os, const Figure<U>& fig);
    template <Scalar U>
    friend std::istream& operator>>(std::istream& is, Figure<U>& fig);

protected:
    std::vector<std::unique_ptr<Point<T>>> _points;
};

template <Scalar T>
Figure<T>::Figure(const std::initializer_list<Point<T>>& lst) {
    if (lst.size() < 3) {
        throw std::runtime_error("Figure must have at least 3 points");
    }
    _points.reserve(lst.size());
    for (const auto& p : lst) {
        _points.emplace_back(std::make_unique<Point<T>>(p));
    }
}

template <Scalar T>
Figure<T>::Figure(const Figure<T>& other) {
    _points.reserve(other._points.size());
    for (const auto& p : other._points) {
        _points.emplace_back(std::make_unique<Point<T>>(*p));
    }
}

template <Scalar T>
Figure<T>& Figure<T>::operator=(const Figure<T>& other) {
    if (this != &other) {
        _points.clear();
        _points.reserve(other._points.size());
        for (const auto& p : other._points) {
            _points.emplace_back(std::make_unique<Point<T>>(*p));
        }
    }
    return *this;
}

template <Scalar T>
bool Figure<T>::operator==(const Figure<T>& other) {
    if (_points.size() != other._points.size()) {
        return false;
    }
    for (size_t i = 0; i < _points.size(); ++i) {
        if (!(*_points[i] == *other._points[i])) {
            return false;
        }
    }
    return true;
}

template <Scalar T>
Point<T> Figure<T>::get_barycenter() const {
    T sum_x = 0;
    T sum_y = 0;
    for (const auto& p : _points) {
        sum_x += p->get_x();
        sum_y += p->get_y();
    }
    size_t n = _points.size();
    return Point<T>(sum_x / n, sum_y / n);
}

template <Scalar T>
Figure<T>::operator double() const {
    double area = 0.0;
    size_t n = _points.size();
    for (size_t i = 0; i < n; ++i) {
        const Point<T>& p1 = *_points[i];
        const Point<T>& p2 = *_points[(i + 1) % n];
        area += static_cast<double>(p1.get_x()) * p2.get_y() - p2.get_x() * p1.get_y();
    }
    return std::abs(area) / 2.0;
}

template <Scalar T>
std::ostream& operator<<(std::ostream& os, const Figure<T>& fig) {
    os << "Figure with " << fig._points.size() << " points: {";
    for (size_t i = 0; i < fig._points.size(); ++i) {
        os << *fig._points[i];
        if (i < fig._points.size() - 1) {
            os << ", ";
        }
    }
    os << "}";
    return os;
}

template <Scalar T>
std::istream& operator>>(std::istream& is, Figure<T>& fig) {
    size_t points_count;
    is >> points_count;

    if (points_count < 3) {
        throw std::runtime_error("Figure must have at least 3 points");
    }

    fig._points.clear();
    fig._points.reserve(points_count);

    for (size_t i = 0; i < points_count; ++i) {
        Point<T> p;
        is >> p;
        fig._points.emplace_back(std::make_unique<Point<T>>(p));
    }
    return is;
}

template <Scalar T>
class Rectangle : public Figure<T> {
public:
    Rectangle(Point<T> start, T width, T height);
    Rectangle(const std::initializer_list<Point<T>>& lst);

    Rectangle(const Rectangle<T>& other);
    Rectangle(Rectangle<T>&& other) noexcept;
    Rectangle<T>& operator=(const Rectangle<T>& other);
    Rectangle<T>& operator=(Rectangle<T>&& other) noexcept;

    operator double() const override;
    Point<T> get_barycenter() const override;

private:
    T _width;
    T _height;
};

template <Scalar T>
Rectangle<T>::Rectangle(Point<T> start, T width, T height) : _width(width), _height(height) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Width and height must be positive");
    }

    this->_points.reserve(4);
    this->_points.emplace_back(std::make_unique<Point<T>>(start));
    this->_points.emplace_back(std::make_unique<Point<T>>(start.get_x() + width, start.get_y()));
    this->_points.emplace_back(std::make_unique<Point<T>>(start.get_x() + width, start.get_y() + height));
    this->_points.emplace_back(std::make_unique<Point<T>>(start.get_x(), start.get_y() + height));
}

template <Scalar T>
Rectangle<T>::Rectangle(const std::initializer_list<Point<T>>& lst) : Figure<T>(lst) {
    if (lst.size() != 4) {
        throw std::invalid_argument("Rectangle must have 4 points");
    }

    auto it = lst.begin();
    T width = std::abs((it + 1)->get_x() - it->get_x());
    T height = std::abs((it + 3)->get_y() - it->get_y());
    _width = width;
    _height = height;
}

template <Scalar T>
Rectangle<T>::Rectangle(const Rectangle<T>& other) : Figure<T>(other), _width(other._width), _height(other._height) {}

template <Scalar T>
Rectangle<T>::Rectangle(Rectangle<T>&& other) noexcept : Figure<T>(std::move(other)), _width(std::move(other._width)), _height(std::move(other._height)) {}

template <Scalar T>
Rectangle<T>& Rectangle<T>::operator=(const Rectangle<T>& other) {
    if (this != &other) {
        Figure<T>::operator=(other);
        _width = other._width;
        _height = other._height;
    }
    return *this;
}

template <Scalar T>
Rectangle<T>& Rectangle<T>::operator=(Rectangle<T>&& other) noexcept {
    if (this != &other) {
        Figure<T>::operator=(std::move(other));
        _width = std::move(other._width);
        _height = std::move(other._height);
    }
    return *this;
}

template <Scalar T>
Rectangle<T>::operator double() const {
    return static_cast<double>(_width) * _height;
}

template <Scalar T>
Point<T> Rectangle<T>::get_barycenter() const {
    T x = this->_points[0]->get_x() + _width / 2;
    T y = this->_points[0]->get_y() + _height / 2;
    return Point<T>(x, y);
}

template <Scalar T>
class Trapezoid : public Figure<T> {
public:
    Trapezoid(const std::initializer_list<Point<T>>& lst);

    Trapezoid(const Trapezoid<T>& other);
    Trapezoid(Trapezoid<T>&& other) noexcept;
    Trapezoid<T>& operator=(const Trapezoid<T>& other);
    Trapezoid<T>& operator=(Trapezoid<T>&& other) noexcept;

    operator double() const override;
    Point<T> get_barycenter() const override;

private:
    T _base1, _base2, _height;

    void calculate_parameters();
};

template <Scalar T>
Trapezoid<T>::Trapezoid(const std::initializer_list<Point<T>>& lst) : Figure<T>(lst) {
    if (lst.size() != 4) {
        throw std::invalid_argument("Trapezoid must have 4 points");
    }
    calculate_parameters();
}

template <Scalar T>
void Trapezoid<T>::calculate_parameters() {
    const Point<T>& p0 = *this->_points[0];
    const Point<T>& p1 = *this->_points[1];
    const Point<T>& p2 = *this->_points[2];
    const Point<T>& p3 = *this->_points[3];

    _base1 = std::hypot(p1.get_x() - p0.get_x(), p1.get_y() - p0.get_y());
    _base2 = std::hypot(p3.get_x() - p2.get_x(), p3.get_y() - p2.get_y());

    _height = std::abs(p0.get_y() - p2.get_y());
}

template <Scalar T>
Trapezoid<T>::Trapezoid(const Trapezoid<T>& other) : Figure<T>(other), _base1(other._base1), _base2(other._base2), _height(other._height) {}

template <Scalar T>
Trapezoid<T>::Trapezoid(Trapezoid<T>&& other) noexcept : Figure<T>(std::move(other)), _base1(std::move(other._base1)), _base2(std::move(other._base2)), _height(std::move(other._height)) {}

template <Scalar T>
Trapezoid<T>& Trapezoid<T>::operator=(const Trapezoid<T>& other) {
    if (this != &other) {
        Figure<T>::operator=(other);
        _base1 = other._base1;
        _base2 = other._base2;
        _height = other._height;
    }
    return *this;
}

template <Scalar T>
Trapezoid<T>& Trapezoid<T>::operator=(Trapezoid<T>&& other) noexcept {
    if (this != &other) {
        Figure<T>::operator=(std::move(other));
        _base1 = std::move(other._base1);
        _base2 = std::move(other._base2);
        _height = std::move(other._height);
    }
    return *this;
}

template <Scalar T>
Trapezoid<T>::operator double() const {
    return 0.5 * (_base1 + _base2) * _height;
}

template <Scalar T>
Point<T> Trapezoid<T>::get_barycenter() const {
    return Figure<T>::get_barycenter();
}

template <Scalar T>
class Rhombus : public Figure<T> {
public:
    Rhombus(Point<T> center, T diag1, T diag2);
    Rhombus(const std::initializer_list<Point<T>>& lst);

    Rhombus(const Rhombus<T>& other);
    Rhombus(Rhombus<T>&& other) noexcept;
    Rhombus<T>& operator=(const Rhombus<T>& other);
    Rhombus<T>& operator=(Rhombus<T>&& other) noexcept;

    operator double() const override;
    Point<T> get_barycenter() const override;

private:
    T _diag1, _diag2;
};

template <Scalar T>
Rhombus<T>::Rhombus(Point<T> center, T diag1, T diag2) : _diag1(diag1), _diag2(diag2) {
    if (diag1 <= 0 || diag2 <= 0) {
        throw std::invalid_argument("Diagonals must be positive");
    }

    this->_points.reserve(4);
    this->_points.emplace_back(std::make_unique<Point<T>>(center.get_x() - diag1 / 2, center.get_y()));
    this->_points.emplace_back(std::make_unique<Point<T>>(center.get_x(), center.get_y() + diag2 / 2));
    this->_points.emplace_back(std::make_unique<Point<T>>(center.get_x() + diag1 / 2, center.get_y()));
    this->_points.emplace_back(std::make_unique<Point<T>>(center.get_x(), center.get_y() - diag2 / 2));
}

template <Scalar T>
Rhombus<T>::Rhombus(const std::initializer_list<Point<T>>& lst) : Figure<T>(lst) {
    if (lst.size() != 4) {
        throw std::invalid_argument("Rhombus must have 4 points");
    }

    const Point<T>& p0 = *this->_points[0];
    const Point<T>& p2 = *this->_points[2];
    const Point<T>& p1 = *this->_points[1];
    const Point<T>& p3 = *this->_points[3];

    _diag1 = std::hypot(p2.get_x() - p0.get_x(), p2.get_y() - p0.get_y());
    _diag2 = std::hypot(p1.get_x() - p3.get_x(), p1.get_y() - p3.get_y());
}

template <Scalar T>
Rhombus<T>::Rhombus(const Rhombus<T>& other) : Figure<T>(other), _diag1(other._diag1), _diag2(other._diag2) {}

template <Scalar T>
Rhombus<T>::Rhombus(Rhombus<T>&& other) noexcept : Figure<T>(std::move(other)), _diag1(std::move(other._diag1)), _diag2(std::move(other._diag2)) {}

template <Scalar T>
Rhombus<T>& Rhombus<T>::operator=(const Rhombus<T>& other) {
    if (this != &other) {
        Figure<T>::operator=(other);
        _diag1 = other._diag1;
        _diag2 = other._diag2;
    }
    return *this;
}

template <Scalar T>
Rhombus<T>& Rhombus<T>::operator=(Rhombus<T>&& other) noexcept {
    if (this != &other) {
        Figure<T>::operator=(std::move(other));
        _diag1 = std::move(other._diag1);
        _diag2 = std::move(other._diag2);
    }
    return *this;
}

template <Scalar T>
Rhombus<T>::operator double() const {
    return static_cast<double>(_diag1) * _diag2 / 2.0;
}

template <Scalar T>
Point<T> Rhombus<T>::get_barycenter() const {
    const Point<T>& p0 = *this->_points[0];
    const Point<T>& p2 = *this->_points[2];
    T x = (p0.get_x() + p2.get_x()) / 2;
    T y = (p0.get_y() + p2.get_y()) / 2;
    return Point<T>(x, y);
}

template <typename T>
class Array {
public:
    Array();
    Array(size_t capacity);

    void push_back(const T& value);
    void erase(size_t index);

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    size_t get_size() const;
    size_t get_capacity() const;

private:
    std::shared_ptr<T[]> data;
    size_t size;
    size_t capacity;
};

template <typename T>
Array<T>::Array() : data(nullptr), size(0), capacity(0) {}

template <typename T>
Array<T>::Array(size_t capacity) : size(0), capacity(capacity) {
    data = std::shared_ptr<T[]>(new T[capacity]);
}

template <typename T>
void Array<T>::push_back(const T& value) {
    if (size >= capacity) {
        size_t newCapacity = capacity == 0 ? 1 : capacity * 2;
        std::shared_ptr<T[]> newData(new T[newCapacity]);
        for (size_t i = 0; i < size; ++i) {
            newData[i] = std::move(data[i]);
        }
        data = newData;
        capacity = newCapacity;
    }
    data[size++] = value;
}

template <typename T>
void Array<T>::erase(size_t index) {
    if (index < size) {
        for (size_t i = index; i < size - 1; ++i) {
            data[i] = std::move(data[i + 1]);
        }
        size--;
    }
}

template <typename T>
T& Array<T>::operator[](size_t index) {
    return data[index];
}

template <typename T>
const T& Array<T>::operator[](size_t index) const {
    return data[index];
}

template <typename T>
size_t Array<T>::get_size() const {
    return size;
}

template <typename T>
size_t Array<T>::get_capacity() const {
    return capacity;
}
