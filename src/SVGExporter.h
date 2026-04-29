#pragma once

#include "Shape.h"
#include <vector>
#include <string>
#include <memory>

class SVGExporter {
private:
    std::vector<std::shared_ptr<Shape>> shapes;
    double width;
    double height;

public:
    SVGExporter(double width = 800, double height = 600);

    void addShape(std::shared_ptr<Shape> shape);
    void exportToFile(const std::string& filename) const;
};
