#ifndef POLYGON_HPP_
#define POLYGON_HPP_

#include <SFML/Graphics.hpp>
#include <vector>

#include "Shape.hpp"

class Polygon : public Shape {
private:
    std::vector< sf::Vector2f > points;

public:
    Polygon(sf::Color fill = sf::Color::Transparent,
            sf::Color stroke = sf::Color::White, float stroke_thickness = 0);
    virtual std::size_t getPointCount() const;
    virtual sf::Vector2f getPoint(std::size_t index) const;
    void addPoint(const sf::Vector2f& point);
    void polygonUpdate();
};

#endif
