#include "Circle.hpp"

Circle::Circle(float radius, const sf::Vector2f &center, sf::Color fill,
               sf::Color stroke, float stroke_thickness)
    : Ellipse(sf::Vector2f(radius, radius), center, fill, stroke,
              stroke_thickness) {}