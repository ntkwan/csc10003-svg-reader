#ifndef ELLIPSE_H_
#define ELLIPSE_H_

#include "Circle.hpp"

class Ellipse : public Circle {
private:
    sf::Vector2f radius;

public:
    Ellipse(const sf::Vector2f &radius, float center_x, float center_y,
            sf::Color fill, sf::Color stroke, float stroke_thickness);
    virtual sf::Vector2f getPoint(std::size_t index) const override;
};

#endif