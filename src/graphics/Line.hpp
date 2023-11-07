#ifndef LINE_HPP_
#define LINE_HPP_

#include <cmath>

#include "Shape.hpp"

class Line : public Shape {
private:
    sf::Vector2f m_direction;  ///< Direction of the line
    float m_thickness;         ///< Thickness of the line

public:
    Line(const sf::Vector2f& point1, const sf::Vector2f& point2,
         sf::Color stroke = sf::Color::White, float stroke_width = 1.f);
    void setThickness(float thickness);
    float getThickness() const;
    float getLength() const;
    virtual std::size_t getPointCount() const;
    virtual sf::Vector2f getPoint(std::size_t index) const;
};

#endif