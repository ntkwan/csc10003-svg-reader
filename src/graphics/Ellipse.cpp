#include "Ellipse.hpp"

#include <cmath>

Ellipse::Ellipse(const Vector2Df &radius, const Vector2Df &center, Color fill,
                 Color stroke, float stroke_thickness)
    : radius(radius) {
    setPosition(center);
    setFillColor(fill);
    setOutlineColor(stroke);
    setOutlineThickness(stroke_thickness);
}

std::size_t Ellipse::getPointCount() const { return 0; }

Vector2Df Ellipse::getPoint(std::size_t index) const { return Vector2Df(0, 0); }

Vector2Df Ellipse::getRadius() const { return radius; }