#include "Path.hpp"

Path::Path(const ColorShape& fill, const ColorShape& stroke, float stroke_width)
    : SVGElement(fill, stroke, stroke_width) {}

std::string Path::getClass() const { return "Path"; }

void Path::addPoint(PathPoint point) { points.push_back(point); }

std::vector< PathPoint > Path::getPoints() const { return points; }

void Path::setFillRule(std::string fill_rule) { this->fill_rule = fill_rule; }

std::string Path::getFillRule() const { return fill_rule; }

void Path::printData() const {
    SVGElement::printData();
    std::cout << "Points: ";
    for (auto point : points) {
        std::cout << point.tc << " " << point.point.x << " " << point.point.y
                  << " ";
    }
}