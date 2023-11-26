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

std::string Ellipse::getClass() const { return "Ellipse"; }

void Ellipse::setRadius(const Vector2Df &radius) { this->radius = radius; }

Vector2Df Ellipse::getRadius() const { return radius; }

void Ellipse::printData() const {
    Shape::printData();
    std::cout << "Radius: " << getRadius().x << " " << getRadius().y
              << std::endl;
}