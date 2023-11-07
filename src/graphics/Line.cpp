#include "Line.hpp"

Line::Line(const sf::Vector2f& point1, const sf::Vector2f& point2,
           sf::Color stroke, float stroke_width)
    : m_direction(point2 - point1), m_thickness(stroke_width) {
    setPosition(point1);
    setThickness(stroke_width);
    setFillColor(stroke);
    update();
}

void Line::setThickness(float thickness) { m_thickness = thickness; }

float Line::getThickness() const { return m_thickness; }

float Line::getLength() const {
    return std::sqrt(m_direction.x * m_direction.x +
                     m_direction.y * m_direction.y);
}

std::size_t Line::getPointCount() const { return 4; }

sf::Vector2f Line::getPoint(std::size_t index) const {
    sf::Vector2f unitDirection = m_direction / getLength();
    sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

    sf::Vector2f offset = (m_thickness / 2.f) * unitPerpendicular;

    switch (index) {
        default:
        case 0:
            return offset;
        case 1:
            return (m_direction + offset);
        case 2:
            return (m_direction - offset);
        case 3:
            return (-offset);
    }
}