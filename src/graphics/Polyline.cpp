#include "Polyline.hpp"

Polyline::Polyline(float stroke_width, const Color& stroke_color,
                   const Color& fill) {
    setOutlineThickness(stroke_width);
    setOutlineColor(stroke_color);
    setFillColor(fill);
}

void Polyline::addPoint(const Vector2Df& point) { points.push_back(point); }

void Polyline::updateShape() {}

Vector2Df Polyline::getPoint(std::size_t index) const {
    if (index < points.size()) {
        return points[index];
    }
    return Vector2Df(0, 0);
}

std::size_t Polyline::getPointCount() const { return points.size(); }
