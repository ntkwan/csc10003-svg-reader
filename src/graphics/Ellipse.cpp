#include "Ellipse.hpp"

#include <cmath>

Ell::Ell(const Vector2Df &radius, const Vector2Df &center, mColor fill,
         mColor stroke, float stroke_thickness)
    : SVGElement(fill, stroke, stroke_thickness, center), radius(radius) {}

std::string Ell::getClass() const { return "Ellipse"; }

void Ell::setRadius(const Vector2Df &radius) { this->radius = radius; }

Vector2Df Ell::getRadius() const { return radius; }

void Ell::printData() const {
    SVGElement::printData();
    std::cout << "Radius: " << getRadius().x << " " << getRadius().y
              << std::endl;
}