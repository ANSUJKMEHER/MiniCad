#include <iostream>
#include <memory>
#include "Shapes.h"
#include "GeometryMath.h"
#include "SVGExporter.h"

int main() {
    std::cout << "--- Mini CAD Geometry Engine ---\n\n";

    // Create Exporter
    SVGExporter exporter(800, 600);

    // 1. Create a Line
    auto line1 = std::make_shared<Line>(Point2D(100, 100), Point2D(500, 300));
    std::cout << "Original Line: ";
    line1->print();

    // 2. Translate and Rotate the line
    line1->translate(50, 50);
    line1->rotate(0.2, Point2D(150, 150)); // Rotate 0.2 radians around (150, 150)
    std::cout << "After Transform: ";
    line1->print();
    exporter.addShape(line1);

    // 3. Create another line for intersection test
    auto line2 = std::make_shared<Line>(Point2D(100, 400), Point2D(600, 100));
    exporter.addShape(line2);

    // Intersection Line-Line
    auto intersection = GeometryMath::intersectLineLine(*line1, *line2);
    if (intersection) {
        std::cout << "\nIntersection between lines at: (" 
                  << intersection->x << ", " << intersection->y << ")\n";
        // Draw a small circle at intersection
        auto intPoint = std::make_shared<Circle>(*intersection, 5);
        exporter.addShape(intPoint);
    } else {
        std::cout << "\nLines do not intersect (parallel).\n";
    }

    // 4. Create a Circle
    auto circle1 = std::make_shared<Circle>(Point2D(400, 300), 100);
    exporter.addShape(circle1);

    // 5. Create a Polygon
    std::vector<Point2D> polyPoints = {
        Point2D(600, 400),
        Point2D(700, 400),
        Point2D(750, 500),
        Point2D(650, 550),
        Point2D(550, 500)
    };
    auto polygon1 = std::make_shared<Polygon>(polyPoints);
    exporter.addShape(polygon1);

    // 6. Test Distance Calculation
    double dist = GeometryMath::distance(circle1->center, line1->p1);
    std::cout << "\nDistance from Circle Center to Line P1: " << dist << "\n";

    double distToLine = GeometryMath::distanceToLine(circle1->center, *line2);
    std::cout << "Distance from Circle Center to Line2: " << distToLine << "\n\n";

    // Export scene to SVG
    exporter.exportToFile("output.svg");

    return 0;
}
