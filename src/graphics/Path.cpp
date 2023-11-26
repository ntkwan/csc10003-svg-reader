#include "Path.hpp"

Path::Path(const Color& fill, const Color& stroke, float stroke_width) {
    setFillColor(fill);
    setOutlineColor(stroke);
    setOutlineThickness(stroke_width);
}

std::string Path::getClass() const { return "Path"; }

void Path::addPoint(PathPoint point) { points.push_back(point); }

std::vector< PathPoint > Path::getPoints() const { return points; }