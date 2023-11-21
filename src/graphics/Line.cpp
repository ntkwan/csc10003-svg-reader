#include "Line.hpp"

Line::Line(const Vector2Df& point1, const Vector2Df& point2, Color stroke,
           float stroke_width)
    : direction(point2 - point1), thickness(stroke_width) {
    setPosition(point1);
    setThickness(stroke_width);
    setFillColor(stroke);
    update();
}

void Line::setThickness(float thickness) { thickness = thickness; }

float Line::getLength() const {
    return std::sqrt(direction.x * direction.x + direction.y * direction.y);
}

std::size_t Line::getPointCount() const { return 4; }

Vector2Df Line::getPoint(std::size_t index) const {
    Vector2Df unitDirection = direction / getLength();
    Vector2Df unitPerpendicular(-unitDirection.y, unitDirection.x);

    Vector2Df offset = (thickness / 2.f) * unitPerpendicular;

    switch (index) {
        default:
        case 0:
            return offset;
        case 1:
            return (direction + offset);
        case 2:
            return (direction - offset);
        case 3:
            return (-offset);
    }
}