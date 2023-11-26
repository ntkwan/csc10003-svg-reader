#include "Shape.hpp"

Shape::Shape()
    : fill(Color::Black), stroke(Color::Transparent), stroke_width(1) {}

void Shape::setFillColor(const Color& color) { fill = color; }

const Color& Shape::getFillColor() const { return fill; }

void Shape::setOutlineColor(const Color& color) { stroke = color; }

const Color& Shape::getOutlineColor() const { return stroke; }

void Shape::setOutlineThickness(float thickness) { stroke_width = thickness; }

float Shape::getOutlineThickness() const { return stroke_width; }

void Shape::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

void Shape::setPosition(const Vector2Df& position) {
    setPosition(position.x, position.y);
}

Vector2Df Shape::getPosition() const { return position; }

#include <iostream>

void Shape::printData() const {
    std::cout << "Shape: " << getClass() << std::endl;
    std::cout << "Fill: " << getFillColor() << std::endl;
    std::cout << "Stroke: " << getOutlineColor() << std::endl;
    std::cout << "Stroke width: " << getOutlineThickness() << std::endl;
    std::cout << "Position: " << getPosition().x << " " << getPosition().y
              << std::endl;
}

void Shape::translate(float x, float y) {
    position.x += x;
    position.y += y;
}

void Shape::rotate(float angle) {}

void Shape::scale(float x, float y) {}

void Shape::scale(float factor) {}