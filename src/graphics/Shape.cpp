#include "Shape.hpp"

Shape::Shape()
    : fill(mColor::Black), stroke(mColor::Transparent), stroke_width(1) {}

void Shape::setFillColor(const mColor& color) { fill = color; }

const mColor& Shape::getFillColor() const { return fill; }

void Shape::setOutlineColor(const mColor& color) { stroke = color; }

const mColor& Shape::getOutlineColor() const { return stroke; }

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

void Shape::setTransforms(const std::vector< std::string >& transforms) {
    this->transforms = transforms;
}

std::vector< std::string > Shape::getTransforms() const { return transforms; }