#include "Rect.hpp"

// #include "Renderer.hpp"

Rect::Rect(float width, float height, Vector2Df position, Vector2Df radius,
           const Color &fill, const Color &stroke, float stroke_width)
    : PolyShape(fill, stroke, stroke_width), width(width), height(height),
      radius(radius) {
    addPoint(Vector2Df(0, 0));
    addPoint(Vector2Df(width, 0));
    addPoint(Vector2Df(width, height));
    addPoint(Vector2Df(0, height));
    setPosition(position);
}

std::string Rect::getClass() const { return "Rect"; }

// void Rect::render(Renderer &renderer) const { renderer.renderRect(*this); }

void Rect::setWidth(float width) {
    this->width = width;
    points[1].x = width;
    points[2].x = width;
}

float Rect::getWidth() const { return width; }

void Rect::setHeight(float height) {
    this->height = height;
    points[2].y = height;
    points[3].y = height;
}

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