#pragma once

#include "Shapes.h"
#include <optional>
#include <vector>

namespace GeometryMath {

    // Distance Calculation
    double distance(const Point2D& p1, const Point2D& p2);
    double distanceToLine(const Point2D& p, const Line& line);

    // Intersection Detection
    // Returns the intersection point if one exists, otherwise std::nullopt
    [[nodiscard]] std::optional<Point2D> intersectLineLine(const Line& l1, const Line& l2);
    
    // Returns 0, 1, or 2 intersection points
    [[nodiscard]] std::vector<Point2D> intersectLineCircle(const Line& line, const Circle& circle);

} // namespace GeometryMath
