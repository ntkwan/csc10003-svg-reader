#include "Path.hpp"

Path::Path(const mColor& fill, const mColor& stroke, float stroke_width) {
    setFillColor(fill);
    setOutlineColor(stroke);
    setOutlineThickness(stroke_width);
}

std::string Path::getClass() const { return "Path"; }

void Path::addPoint(PathPoint point) { points.push_back(point); }

std::vector< PathPoint > Path::getPoints() const { return points; }

void Path::setFillRule(std::string fill_rule) { this->fill_rule = fill_rule; }

std::string Path::getFillRule() const { return fill_rule; }

void Path::printData() const {
    SVGElement::printData();
    std::cout << "Points: ";
    for (auto point : points) {
        std::cout << point.TC << " " << point.Point.x << " " << point.Point.y
                  << " ";
    }
}