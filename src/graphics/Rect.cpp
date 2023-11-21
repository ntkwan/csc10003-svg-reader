#include "Rect.hpp"

Rect::Rect(float width, float height, float x, float y, sf::Color fill,
           sf::Color stroke, float stroke_thickness)
    : Polygon(fill, stroke, stroke_thickness), width(width), height(height) {
    addPoint(Vector2Df(0, 0));
    addPoint(Vector2Df(width, 0));
    addPoint(Vector2Df(width, height));
    addPoint(Vector2Df(0, height));
    setPosition(x, y);
    update();
}

std::size_t Rect::getPointCount() const {
    return Polygon::getPointCount();  // Inherited from Polygon
}

Vector2Df Rect::getPoint(std::size_t index) const {
    return Polygon::getPoint(index);  // Inherited from Polygon
}
