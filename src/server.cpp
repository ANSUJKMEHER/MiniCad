#define _WIN32_WINNT 0x0A00
#include "httplib.h"
#include "json.hpp"
#include "Shapes.h"
#include "GeometryMath.h"
#include "SVGExporter.h"
#include <iostream>
#include <memory>
#include <vector>

using json = nlohmann::json;

int main() {
    httplib::Server svr;

    // CORS headers for local development
    auto set_cors_headers = [](httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
    };

    svr.Options(".*", [&](const httplib::Request&, httplib::Response& res) {
        set_cors_headers(res);
        res.status = 204;
    });

    svr.Post("/api/geometry", [&](const httplib::Request& req, httplib::Response& res) {
        set_cors_headers(res);
        
        try {
            auto j = json::parse(req.body);
            SVGExporter exporter(800, 600);

            // Parse lines
            std::vector<std::shared_ptr<Line>> lines;
            if (j.contains("lines")) {
                for (const auto& l : j["lines"]) {
                    auto line = std::make_shared<Line>(
                        Point2D(l["x1"].get<double>(), l["y1"].get<double>()),
                        Point2D(l["x2"].get<double>(), l["y2"].get<double>())
                    );
                    lines.push_back(line);
                    exporter.addShape(line);
                }
            }

            // Check intersections if there are at least two lines
            if (lines.size() >= 2) {
                for (size_t i = 0; i < lines.size(); ++i) {
                    for (size_t k = i + 1; k < lines.size(); ++k) {
                        auto intersection = GeometryMath::intersectLineLine(*lines[i], *lines[k]);
                        if (intersection) {
                            // Draw intersection point as a small red circle
                            auto p = std::make_shared<Circle>(*intersection, 6);
                            exporter.addShape(p);
                        }
                    }
                }
            }

            // Parse circles
            if (j.contains("circles")) {
                for (const auto& c : j["circles"]) {
                    auto circle = std::make_shared<Circle>(
                        Point2D(c["cx"].get<double>(), c["cy"].get<double>()),
                        c["r"].get<double>()
                    );
                    exporter.addShape(circle);
                }
            }

            // Generate SVG string (we need a way to get it as string instead of writing to file)
            // Let's create a temporary file, export, read, and return it.
            // Better: update SVGExporter to return string. I'll write to temp file for now.
            exporter.exportToFile("temp_output.svg");
            
            std::ifstream t("temp_output.svg");
            std::string svg_str((std::istreambuf_iterator<char>(t)),
                                 std::istreambuf_iterator<char>());

            json response;
            response["svg"] = svg_str;
            response["status"] = "success";
            
            res.set_content(response.dump(), "application/json");

        } catch (const std::exception& e) {
            json response;
            response["status"] = "error";
            response["message"] = e.what();
            res.set_content(response.dump(), "application/json");
        }
    });

    std::cout << "Starting C++ Geometry Server on http://localhost:8080..." << std::endl;
    svr.listen("0.0.0.0", 8080);

    return 0;
}
