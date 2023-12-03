#include "Path.hpp"

// #include "Renderer.hpp"

Path::Path(const Color& fill, const Color& stroke, float stroke_width) {
    setFillColor(fill);
    setOutlineColor(stroke);
    setOutlineThickness(stroke_width);
}

std::string Path::getClass() const { return "Path"; }

// void Path::render(Renderer& renderer) const { renderer.renderPath(*this); }

void Path::addPoint(PathPoint point) { points.push_back(point); }

std::vector< PathPoint > Path::getPoints() const { return points; }

void Path::printData() const {
    SVGElement::printData();
    std::cout << "Points: ";
    for (auto point : points) {
        std::cout << point.TC << " " << point.Point.x << " " << point.Point.y
                  << " ";
    }
}