#include "PolyShape.hpp"

PolyShape::PolyShape(const mColor& fill, const mColor& stroke,
                     float stroke_width)
    : SVGElement(fill, stroke, stroke_width) {}

void PolyShape::addPoint(const Vector2Df& point) { points.push_back(point); }

const std::vector< Vector2Df >& PolyShape::getPoints() const { return points; }

void PolyShape::setFillRule(std::string fill_rule) {
    this->fill_rule = fill_rule;
}

std::string PolyShape::getFillRule() const { return fill_rule; }

Vector2Df PolyShape::getMinBound() const {
    float min_x = points[0].x;
    float min_y = points[0].y;
    for (auto& point : points) {
        min_x = std::min(min_x, point.x);
        min_y = std::min(min_y, point.y);
    }
    return Vector2Df(min_x, min_y);
}

Vector2Df PolyShape::getMaxBound() const {
    float max_x = points[0].x;
    float max_y = points[0].y;
    for (auto& point : points) {
        max_x = std::max(max_x, point.x);
        max_y = std::max(max_y, point.y);
    }
    return Vector2Df(max_x, max_y);
}

void PolyShape::printData() const {
    SVGElement::printData();
    std::cout << "Points: ";
    for (auto& point : getPoints()) {
        std::cout << point.x << "," << point.y << " ";
    }
    std::cout << std::endl;
}