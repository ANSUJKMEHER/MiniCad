#include "SVGExporter.h"
#include <fstream>
#include <iostream>

SVGExporter::SVGExporter(double width, double height) : width(width), height(height) {}

void SVGExporter::addShape(std::shared_ptr<Shape> shape) {
    shapes.push_back(shape);
}

void SVGExporter::exportToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"" 
        << width << "\" height=\"" << height << "\">\n";

    // Add a border
    out << "<rect width=\"100%\" height=\"100%\" fill=\"white\" stroke=\"black\" stroke-width=\"1\" />\n";

    for (const auto& shape : shapes) {
        out << "    " << shape->toSVG() << "\n";
    }

    out << "</svg>\n";
    out.close();
    
    std::cout << "Successfully exported SVG to " << filename << "\n";
}
