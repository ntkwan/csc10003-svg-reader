#include "Ellipse.hpp"

#include <cmath>

Ell::Ell(const Vector2Df &radius, const Vector2Df &center, ColorShape fill,
         ColorShape stroke, float stroke_thickness)
    : SVGElement(fill, stroke, stroke_thickness, center), radius(radius) {}

std::string Ell::getClass() const { return "Ellipse"; }

void Ell::setRadius(const Vector2Df &radius) { this->radius = radius; }

Vector2Df Ell::getRadius() const { return radius; }

Vector2Df Ell::getMinBound() const {
    return Vector2Df(getPosition().x - getRadius().x,
                     getPosition().y - getRadius().y);
}

Vector2Df Ell::getMaxBound() const {
    return Vector2Df(getPosition().x + getRadius().x,
                     getPosition().y + getRadius().y);
}

void Ell::printData() const {
    SVGElement::printData();
    std::cout << "Radius: " << getRadius().x << " " << getRadius().y
              << std::endl;
}