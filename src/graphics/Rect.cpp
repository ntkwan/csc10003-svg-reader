#include "Rect.hpp"

Rect::Rect(float width, float height, float x, float y, sf::Color fill,
           sf::Color stroke, float stroke_thickness)
    : Polygon(fill, stroke, stroke_thickness), width(width), height(height) {
    addPoint(sf::Vector2f(0, 0));
    addPoint(sf::Vector2f(width, 0));
    addPoint(sf::Vector2f(width, height));
    addPoint(sf::Vector2f(0, height));

    setPosition(x, y);
    update();
}

std::size_t Rect::getPointCount() const {
    return Polygon::getPointCount();  // Inherited from Polygon
}

sf::Vector2f Rect::getPoint(std::size_t index) const {
    return Polygon::getPoint(index);  // Inherited from Polygon
}
