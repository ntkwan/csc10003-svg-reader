#include "Line.hpp"

#include <cmath>

#include "Renderer.hpp"

Line::Line(const Vector2Df& point1, const Vector2Df& point2, Color stroke,
           float stroke_width)
    : direction(point2) {
    setPosition(point1);
    setOutlineThickness(stroke_width);
    setFillColor(stroke);
}

std::string Line::getClass() const { return "Line"; }

void Line::render(Renderer& renderer) const { renderer.renderLine(*this); }

void Line::setDirection(const Vector2Df& direction) {
    this->direction = direction;
}

Vector2Df Line::getDirection() const { return direction; }

float Line::getLength() const {
    return std::sqrt(std::pow(direction.x, 2) + std::pow(direction.y, 2));
}