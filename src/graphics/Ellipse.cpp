#include "Ellipse.hpp"

#include <cmath>

// #include "Renderer.hpp"

Ellipse::Ellipse(const Vector2Df &radius, const Vector2Df &center, Color fill,
                 Color stroke, float stroke_thickness)
    : radius(radius) {
    setPosition(center);
    setFillColor(fill);
    setOutlineColor(stroke);
    setOutlineThickness(stroke_thickness);
}

std::string Ellipse::getClass() const { return "Ellipse"; }

// void Ellipse::render(Renderer &renderer) const {
//     renderer.renderEllipse(*this);
// }

void Ellipse::setRadius(const Vector2Df &radius) { this->radius = radius; }

Vector2Df Ellipse::getRadius() const { return radius; }

void Ellipse::printData() const {
    SVGElement::printData();
    std::cout << "Radius: " << getRadius().x << " " << getRadius().y
              << std::endl;
}