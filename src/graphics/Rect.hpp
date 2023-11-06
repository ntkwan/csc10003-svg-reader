#ifndef RECT_HPP_
#define RECT_HPP_

#include "Shape.hpp"

class Rect : public Shape {
private:
    float width;
    float height;
    float x, y;
    sf::Vector2f rect_size;

public:
    Rect(float width, float height, float x, float y, sf::Color fill,
         sf::Color stroke, float stroke_thickness);
    const sf::Vector2f& getSize() const;
    void setSize(const sf::Vector2f& size);
    virtual std::size_t getPointCount() const;
    virtual sf::Vector2f getPoint(std::size_t index) const;
};

#endif