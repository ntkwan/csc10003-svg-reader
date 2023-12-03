#include "SVGElement.hpp"

#include <iostream>

SVGElement::SVGElement()
    : fill(mColor::Black), stroke(mColor::Transparent), stroke_width(1) {}

void SVGElement::setFillColor(const mColor& color) { fill = color; }

const mColor& SVGElement::getFillColor() const { return fill; }

void SVGElement::setOutlineColor(const mColor& color) { stroke = color; }

const mColor& SVGElement::getOutlineColor() const { return stroke; }

void SVGElement::setOutlineThickness(float thickness) {
    stroke_width = thickness;
}

float SVGElement::getOutlineThickness() const { return stroke_width; }

void SVGElement::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

void SVGElement::setPosition(const Vector2Df& position) {
    setPosition(position.x, position.y);
}

Vector2Df SVGElement::getPosition() const { return position; }

void SVGElement::printData() const {
    std::cout << "Shape: " << getClass() << std::endl;
    std::cout << "Fill: " << getFillColor() << std::endl;
    std::cout << "Stroke: " << getOutlineColor() << std::endl;
    std::cout << "Stroke width: " << getOutlineThickness() << std::endl;
    std::cout << "Position: " << getPosition().x << " " << getPosition().y
              << std::endl;
    std::cout << "Transforms: ";
    for (auto transform : transforms) {
        std::cout << transform << " ";
    }
    std::cout << std::endl;
}

void SVGElement::setTransforms(const std::vector< std::string >& transforms) {
    this->transforms = transforms;
}

std::vector< std::string > SVGElement::getTransforms() const {
    return transforms;
}

void SVGElement::setParent(SVGElement* parent) { this->parent = parent; }

SVGElement* SVGElement::getParent() const { return parent; }

void SVGElement::addElement(SVGElement* element) {}