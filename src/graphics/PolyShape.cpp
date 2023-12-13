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

void PolyShape::printData() const {
    SVGElement::printData();
    std::cout << "Points: ";
    for (auto& point : getPoints()) {
        std::cout << point.x << "," << point.y << " ";
    }
    std::cout << std::endl;
}