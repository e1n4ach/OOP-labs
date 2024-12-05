#pragma once

#include <iostream>
#include <initializer_list>

class Point {
public:
    Point();
    Point(double x, double y);
    Point(const Point &other) = default;
    Point(Point &&other) noexcept = default;
    ~Point() = default;

    double getX() const;
    double getY() const;

    Point &operator=(const Point &other);
    Point &operator=(Point &&other) noexcept;
    bool operator==(const Point &other) const;

    friend std::ostream &operator<<(std::ostream &os, const Point &p);
    friend std::istream &operator>>(std::istream &is, Point &p);

private:
    double _x = 0.0;
    double _y = 0.0;
};

class Figure {
public:
    Figure();
    Figure(const std::initializer_list<Point> &lst);
    Figure(const Figure &other);
    Figure(Figure &&other) noexcept;
    virtual ~Figure() noexcept;

    Figure &operator=(const Figure &other);
    Figure &operator=(Figure &&other) noexcept;
    bool operator==(const Figure &other);

    virtual Point get_barycenter() const;
    virtual explicit operator double() const;

    friend std::ostream &operator<<(std::ostream &os, const Figure &fig);
    friend std::istream &operator>>(std::istream &is, Figure &fig);

protected:
    size_t _points_count;
    Point *_points;
};

class Rectangle : public Figure {
public:
    Rectangle(Point start, double width, double height);
    Rectangle(const std::initializer_list<Point> &lst);

    Rectangle(const Rectangle &other);
    Rectangle(Rectangle &&other) noexcept;
    Rectangle &operator=(const Rectangle &other);
    Rectangle &operator=(Rectangle &&other) noexcept;

    operator double() const override;

private:
    double _width, _height;
};

class Trapezoid : public Figure {
public:
    Trapezoid(const std::initializer_list<Point> &lst);

    Trapezoid(const Trapezoid &other);
    Trapezoid(Trapezoid &&other) noexcept;
    Trapezoid &operator=(const Trapezoid &other);
    Trapezoid &operator=(Trapezoid &&other) noexcept;

    operator double() const override;

private:
    double _base1, _base2, _height;
};

class Rhombus : public Figure {
public:
    Rhombus(Point center, double diagonal1, double diagonal2);
    Rhombus(const std::initializer_list<Point> &lst);

    Rhombus(const Rhombus &other);
    Rhombus(Rhombus &&other) noexcept;
    Rhombus &operator=(const Rhombus &other);
    Rhombus &operator=(Rhombus &&other) noexcept;

    operator double() const override;

private:
    double _diagonal1, _diagonal2;
};
