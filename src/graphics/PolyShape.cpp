#include "PolyShape.hpp"

PolyShape::PolyShape(const Color& fill, const Color& stroke,
                     float stroke_width) {
    setFillColor(fill);
    setOutlineColor(stroke);
    setOutlineThickness(stroke_width);
}

void PolyShape::addPoint(const Vector2Df& point) { points.push_back(point); }

const std::vector< Vector2Df >& PolyShape::getPoints() const { return points; }