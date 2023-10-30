#include <SFML/Graphics.hpp>
#ifndef CIRCLE_H_
#define CIRCLE_H_
#define SCALE 1000000

class Circle : public sf::Shape {
private:
    float radius;

public:
    Circle(float, float, float, sf::Color, sf::Color, float);
    virtual std::size_t getPointCount() const;
    virtual sf::Vector2f getPoint(std::size_t index) const;
};

#endif