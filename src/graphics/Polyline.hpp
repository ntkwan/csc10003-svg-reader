#ifndef Polyline_HPP_
#define Polyline_HPP_

#include <math.h>

#include <SFML/Graphics.hpp>
#include <vector>

#include "Shape.hpp"

class Polyline : public Shape {
private:
    std::vector< sf::Vector2f > points;

public:
    Polyline(float stroke_Width = 0,
             const sf::Color& stroke_color = sf::Color::White,
             const sf::Color& fill = sf::Color::Transparent);
    void addPoint(const sf::Vector2f& point);
    void draw(sf::RenderWindow& target,
              sf::RenderStates states = sf::RenderStates::Default) const;
    sf::Vector2f getPoint(std::size_t index) const override;
    std::size_t getPointCount() const override;
    void polylineUpdate();
};

#endif
