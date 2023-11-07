#include "Rect.hpp"

Rect::Rect(float width, float height, float x, float y, sf::Color fill,
           sf::Color stroke, float stroke_thickness)
    : width(width), height(height), x(x), y(y) {
    setPosition(x, y);
    this->rect_size = sf::Vector2f(width, height);
    setSize(this->rect_size);
    setFillColor(fill);
    setOutlineColor(stroke);
    setOutlineThickness(stroke_thickness);
    update();
}

void Rect::setSize(const sf::Vector2f& size) { this->rect_size = size; }

const sf::Vector2f& Rect::getSize() const { return this->rect_size; }

std::size_t Rect::getPointCount() const { return 4; }

sf::Vector2f Rect::getPoint(std::size_t index) const {
    switch (index) {
        default:
        case 0:
            return sf::Vector2f(0, 0);
        case 1:
            return sf::Vector2f(this->rect_size.x, 0);
        case 2:
            return sf::Vector2f(this->rect_size.x, this->rect_size.y);
        case 3:
            return sf::Vector2f(0, this->rect_size.y);
    }
}