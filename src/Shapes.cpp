#include "Shapes.h"
#include <cmath>
#include <sstream>

// Helper to rotate a single point
Point2D rotatePoint(const Point2D& p, double angleRad, const Point2D& origin) {
    double s = std::sin(angleRad);
    double c = std::cos(angleRad);

    // Translate point back to origin
    Point2D p_orig = p - origin;

    // Rotate point
    double xnew = p_orig.x * c - p_orig.y * s;
    double ynew = p_orig.x * s + p_orig.y * c;

    // Translate point back
    return Point2D(xnew, ynew) + origin;
}

// Helper to scale a single point
Point2D scalePoint(const Point2D& p, double factor, const Point2D& origin) {
    Point2D p_orig = p - origin;
    return p_orig * factor + origin;
}

// --- Line ---
Line::Line(const Point2D& p1, const Point2D& p2) : p1(p1), p2(p2) {}

void Line::translate(double dx, double dy) {
    Point2D offset(dx, dy);
    p1 = p1 + offset;
    p2 = p2 + offset;
}

void Line::rotate(double angleRad, const Point2D& origin) {
    p1 = rotatePoint(p1, angleRad, origin);
    p2 = rotatePoint(p2, angleRad, origin);
}

void Line::scale(double factor, const Point2D& origin) {
    p1 = scalePoint(p1, factor, origin);
    p2 = scalePoint(p2, factor, origin);
}

void Line::print() const {
    std::cout << "Line [(" << p1.x << ", " << p1.y << ") to (" << p2.x << ", " << p2.y << ")]\n";
}

std::string Line::toSVG() const {
    std::ostringstream oss;
    oss << "<line x1=\"" << p1.x << "\" y1=\"" << p1.y 
        << "\" x2=\"" << p2.x << "\" y2=\"" << p2.y 
        << "\" stroke=\"black\" stroke-width=\"2\" />";
    return oss.str();
}

// --- Circle ---
Circle::Circle(const Point2D& center, double radius) : center(center), radius(radius) {}

void Circle::translate(double dx, double dy) {
    center = center + Point2D(dx, dy);
}

void Circle::rotate(double angleRad, const Point2D& origin) {
    center = rotatePoint(center, angleRad, origin);
}

void Circle::scale(double factor, const Point2D& origin) {
    center = scalePoint(center, factor, origin);
    radius *= factor;
}

void Circle::print() const {
    std::cout << "Circle [Center: (" << center.x << ", " << center.y << "), Radius: " << radius << "]\n";
}

std::string Circle::toSVG() const {
    std::ostringstream oss;
    oss << "<circle cx=\"" << center.x << "\" cy=\"" << center.y 
        << "\" r=\"" << radius << "\" stroke=\"blue\" stroke-width=\"2\" fill=\"none\" />";
    return oss.str();
}

// --- Polygon ---
Polygon::Polygon(const std::vector<Point2D>& vertices) : vertices(vertices) {}

void Polygon::translate(double dx, double dy) {
    Point2D offset(dx, dy);
    for (auto& v : vertices) {
        v = v + offset;
    }
}

void Polygon::rotate(double angleRad, const Point2D& origin) {
    for (auto& v : vertices) {
        v = rotatePoint(v, angleRad, origin);
    }
}

void Polygon::scale(double factor, const Point2D& origin) {
    for (auto& v : vertices) {
        v = scalePoint(v, factor, origin);
    }
}

void Polygon::print() const {
    std::cout << "Polygon [";
    for (size_t i = 0; i < vertices.size(); ++i) {
        std::cout << "(" << vertices[i].x << ", " << vertices[i].y << ")";
        if (i < vertices.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
}

std::string Polygon::toSVG() const {
    std::ostringstream oss;
    oss << "<polygon points=\"";
    for (const auto& v : vertices) {
        oss << v.x << "," << v.y << " ";
    }
    oss << "\" stroke=\"red\" stroke-width=\"2\" fill=\"none\" />";
    return oss.str();
}
