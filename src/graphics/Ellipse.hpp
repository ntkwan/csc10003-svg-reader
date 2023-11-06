#ifndef ELLIPSE_HPP_
#define ELLIPSE_HPP_

#include "Circle.hpp"

class Ellipse : public Circle {
private:
    sf::Vector2f radius;

public:
    Ellipse(const sf::Vector2f &radius, const sf::Vector2f &center,
            sf::Color fill, sf::Color stroke, float stroke_thickness);
    virtual sf::Vector2f getPoint(std::size_t index) const override;
};

#endif