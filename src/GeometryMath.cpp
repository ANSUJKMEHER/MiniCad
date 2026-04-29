#include "GeometryMath.h"

namespace GeometryMath {

double distance(const Point2D& p1, const Point2D& p2) {
    return (p1 - p2).magnitude();
}

double distanceToLine(const Point2D& p, const Line& line) {
    // Distance from point (x0, y0) to line passing through (x1, y1) and (x2, y2):
    // |(x2 - x1)(y1 - y0) - (x1 - x0)(y2 - y1)| / sqrt((x2 - x1)^2 + (y2 - y1)^2)
    double num = std::abs((line.p2.x - line.p1.x) * (line.p1.y - p.y) - 
                          (line.p1.x - p.x) * (line.p2.y - line.p1.y));
    double den = distance(line.p1, line.p2);
    if (den == 0.0) return distance(p, line.p1); // Line is just a point
    return num / den;
}

std::optional<Point2D> intersectLineLine(const Line& l1, const Line& l2) {
    double x1 = l1.p1.x, y1 = l1.p1.y;
    double x2_l1 = l1.p2.x, y2_l1 = l1.p2.y;
    
    // Let's use proper line intersection math (infinite lines)
    // Line 1: p1 to p2
    // Line 2: p3 to p4
    double x3 = l2.p1.x, y3 = l2.p1.y;
    double x4 = l2.p2.x, y4 = l2.p2.y;

    double den = (x1 - x2_l1) * (y3 - y4) - (y1 - y2_l1) * (x3 - x4);
    if (std::abs(den) < 1e-9) {
        return std::nullopt; // Parallel or coincident
    }

    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
    
    // If we wanted line segment intersection, we'd check 0 <= t <= 1 and 0 <= u <= 1
    // For infinite lines, we just use t
    double px = x1 + t * (x2_l1 - x1);
    double py = y1 + t * (y2_l1 - y1);
    
    return Point2D(px, py);
}

std::vector<Point2D> intersectLineCircle(const Line& line, const Circle& circle) {
    std::vector<Point2D> intersections;
    
    // Line: P = P1 + t * (P2 - P1) = P1 + t * D
    Point2D d = line.p2 - line.p1;
    Point2D f = line.p1 - circle.center;

    // t^2 * (d.d) + 2t * (f.d) + (f.f - r^2) = 0
    double a = d.dot(d);
    double b = 2 * f.dot(d);
    double c = f.dot(f) - circle.radius * circle.radius;

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        // No intersection
    } else if (discriminant == 0 || std::abs(discriminant) < 1e-9) {
        // One intersection (tangent)
        double t = -b / (2 * a);
        intersections.push_back(line.p1 + d * t);
    } else {
        // Two intersections
        double t1 = (-b - std::sqrt(discriminant)) / (2 * a);
        double t2 = (-b + std::sqrt(discriminant)) / (2 * a);
        intersections.push_back(line.p1 + d * t1);
        intersections.push_back(line.p1 + d * t2);
    }

    return intersections;
}

} // namespace GeometryMath
