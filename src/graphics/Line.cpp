#include "Line.hpp"

#include <cmath>

Line::Line(const Vector2Df& point1, const Vector2Df& point2, mColor stroke,
           float stroke_width)
    : direction(point2) {
    setPosition(point1);
    setOutlineThickness(stroke_width);
    setOutlineColor(stroke);
}

std::string Line::getClass() const { return "Line"; }

void Line::setDirection(const Vector2Df& direction) {
    this->direction = direction;
}

Vector2Df Line::getDirection() const { return direction; }

float Line::getLength() const {
    return std::sqrt(std::pow(direction.x, 2) + std::pow(direction.y, 2));
}