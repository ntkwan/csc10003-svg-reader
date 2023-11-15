#include "Ellipse.hpp"

#include <cmath>

Ellipse::Ellipse(const sf::Vector2f &radius, const sf::Vector2f &center,
                 sf::Color fill, sf::Color stroke, float stroke_thickness)
    : radius(radius) {
    setPosition(center);
    setFillColor(fill);
    setOutlineColor(stroke);
    setOutlineThickness(stroke_thickness);
    update();
}

std::size_t Ellipse::getPointCount() const { return SCALE; }

sf::Vector2f Ellipse::getPoint(std::size_t index) const {
    static const float pi = acos(-1);

    float angle = index * 2 * pi / getPointCount() - pi / 2;
    float x = std::cos(angle) * radius.x;
    float y = std::sin(angle) * radius.y;

    return sf::Vector2f(radius.x + x, radius.y + y);
}