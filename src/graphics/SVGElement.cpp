#include "SVGElement.hpp"

#include <iostream>

SVGElement::SVGElement()
    : fill(mColor::Black), stroke(mColor::Transparent), stroke_width(1),
      gradient(NULL) {}

SVGElement::SVGElement(const mColor& fill, const mColor& stroke,
                       float stroke_width)
    : fill(fill), stroke(stroke), stroke_width(stroke_width), gradient(NULL) {}

SVGElement::SVGElement(const mColor& fill, const mColor& stroke,
                       float stroke_width, const Vector2Df& position)
    : fill(fill), stroke(stroke), stroke_width(stroke_width),
      position(position), gradient(NULL) {}

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

Vector2Df SVGElement::getMinBound() const { return Vector2Df(); }

Vector2Df SVGElement::getMaxBound() const { return Vector2Df(); }

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
    if (gradient != NULL)
        std::cout << "Gradient: " << gradient->getClass() << " "
                  << gradient->getPoints().first.x << " "
                  << gradient->getPoints().first.y << " "
                  << gradient->getPoints().second.x << " "
                  << gradient->getPoints().second.y << std::endl;
}

void SVGElement::setTransforms(const std::vector< std::string >& transforms) {
    this->transforms = transforms;
}

std::vector< std::string > SVGElement::getTransforms() const {
    return transforms;
}

void SVGElement::setParent(SVGElement* parent) { this->parent = parent; }

SVGElement* SVGElement::getParent() const { return parent; }

void SVGElement::setGradient(Gradient* gradient) { this->gradient = gradient; }

Gradient* SVGElement::getGradient() const { return gradient; }

void SVGElement::addElement(SVGElement* element) {}