#ifndef CIRCLE_HPP_
#define CIRCLE_HPP_

#include "Shape.hpp"
const int SCALE = 100000;

class Circle : public Shape {
private:
    float radius;

public:
    Circle(float radius, const sf::Vector2f &center, sf::Color fill,
           sf::Color stroke, float stroke_thickness);
    virtual std::size_t getPointCount() const;
    virtual sf::Vector2f getPoint(std::size_t index) const;
};

#endif