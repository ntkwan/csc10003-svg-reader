#include "Polygon.hpp"

Polygon::Polygon(Color fill, Color stroke, float stroke_thickness) {
    setFillColor(fill);
    setOutlineColor(stroke);
    setOutlineThickness(stroke_thickness);
}

std::size_t Polygon::getPointCount() const { return points.size(); }

Vector2Df Polygon::getPoint(std::size_t index) const {
    if (index < points.size()) {
        return points[index];
    } else {
        return points[index % points.size()];
    }
}

void Polygon::addPoint(const Vector2Df& point) { points.push_back(point); }

void Polygon::updateShape() { update(); }