#pragma once

#include "Shape.h"
#include <vector>

class Line : public Shape {
public:
    Point2D p1, p2;

    Line(const Point2D& p1, const Point2D& p2);

    void translate(double dx, double dy) override;
    void rotate(double angleRad, const Point2D& origin = Point2D(0, 0)) override;
    void scale(double factor, const Point2D& origin = Point2D(0, 0)) override;
    
    void print() const override;
    std::string toSVG() const override;
};

class Circle : public Shape {
public:
    Point2D center;
    double radius;

    Circle(const Point2D& center, double radius);

    void translate(double dx, double dy) override;
    void rotate(double angleRad, const Point2D& origin = Point2D(0, 0)) override;
    void scale(double factor, const Point2D& origin = Point2D(0, 0)) override;
    
    void print() const override;
    std::string toSVG() const override;
};

class Polygon : public Shape {
public:
    std::vector<Point2D> vertices;

    Polygon(const std::vector<Point2D>& vertices);

    void translate(double dx, double dy) override;
    void rotate(double angleRad, const Point2D& origin = Point2D(0, 0)) override;
    void scale(double factor, const Point2D& origin = Point2D(0, 0)) override;
    
    void print() const override;
    std::string toSVG() const override;
};
