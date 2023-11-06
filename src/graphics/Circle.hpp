#ifndef CIRCLE_HPP_
#define CIRCLE_HPP_

#include "Shape.hpp"
#define SCALE 1000000

class Circle : public Shape {
private:
    float radius;

public:
    Circle(float, float, float, sf::Color, sf::Color, float);
    virtual std::size_t getPointCount() const;
    virtual sf::Vector2f getPoint(std::size_t index) const;
};

#endif