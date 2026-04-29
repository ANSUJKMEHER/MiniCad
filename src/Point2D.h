#pragma once

#include <cmath>

struct Point2D {
    double x;
    double y;

    constexpr Point2D(double x = 0.0, double y = 0.0) : x(x), y(y) {}

    // Vector operations
    [[nodiscard]] constexpr Point2D operator+(const Point2D& other) const {
        return Point2D(x + other.x, y + other.y);
    }

    [[nodiscard]] constexpr Point2D operator-(const Point2D& other) const {
        return Point2D(x - other.x, y - other.y);
    }

    [[nodiscard]] constexpr Point2D operator*(double scalar) const {
        return Point2D(x * scalar, y * scalar);
    }

    [[nodiscard]] constexpr Point2D operator/(double scalar) const {
        return Point2D(x / scalar, y / scalar);
    }

    // Dot product
    [[nodiscard]] constexpr double dot(const Point2D& other) const {
        return x * other.x + y * other.y;
    }

    // Cross product (2D analog, returns scalar magnitude of Z)
    [[nodiscard]] constexpr double cross(const Point2D& other) const {
        return x * other.y - y * other.x;
    }

    // Magnitude
    [[nodiscard]] double magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    // Normalize
    [[nodiscard]] Point2D normalized() const {
        double mag = magnitude();
        if (mag == 0) return Point2D(0, 0);
        return *this / mag;
    }
};
