#include "Circle.h"
#include <cmath>

Circle::Circle(float radius, float center_x, float center_y, sf::Color fill, sf::Color stroke, float stroke_thickness) : radius(radius)
{
    setPosition(center_x, center_y);
    setFillColor(fill);
    setOutlineColor(stroke);
    setOutlineThickness(stroke_thickness);
    update();
}

std::size_t Circle::getPointCount() const
{
    return SCALE;
}

sf::Vector2f Circle::getPoint(std::size_t index) const
{
    static const float pi = 3.141592654f;

    float angle = index * 2 * pi / getPointCount() - pi / 2;
    float x = std::cos(angle) * radius;
    float y = std::sin(angle) * radius;

    return sf::Vector2f(radius + x, radius + y);
}