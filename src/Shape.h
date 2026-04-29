#pragma once

#include "Point2D.h"
#include <string>
#include <iostream>

class Shape {
public:
    virtual ~Shape() = default;

    // Transformations
    virtual void translate(double dx, double dy) = 0;
    virtual void rotate(double angleRad, const Point2D& origin = Point2D(0, 0)) = 0;
    virtual void scale(double factor, const Point2D& origin = Point2D(0, 0)) = 0;

    // Output
    virtual void print() const = 0;
    [[nodiscard]] virtual std::string toSVG() const = 0;
};
