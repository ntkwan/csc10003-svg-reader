#include "Rect.hpp"

Rect::Rect(float width, float height, Vector2Df position, Vector2Df radius,
           const ColorShape &fill, const ColorShape &stroke, float stroke_width)
    : SVGElement(fill, stroke, stroke_width, position), width(width),
      height(height), radius(radius) {}

std::string Rect::getClass() const { return "Rect"; }

void Rect::setWidth(float width) { this->width = width; }

float Rect::getWidth() const { return width; }

void Rect::setHeight(float height) { this->height = height; }

float Rect::getHeight() const { return height; }

void Rect::setRadius(const Vector2Df &radius) { this->radius = radius; }

Vector2Df Rect::getRadius() const { return radius; }

void Rect::printData() const {
    SVGElement::printData();
    std::cout << "Width: " << getWidth() << std::endl;
    std::cout << "Height: " << getHeight() << std::endl;
    std::cout << "Radius: " << getRadius().x << " " << getRadius().y
              << std::endl;
}