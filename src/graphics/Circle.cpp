#include "Circle.hpp"

#include "Renderer.hpp"
Circle::Circle(float radius, const Vector2Df &center, Color fill, Color stroke,
               float stroke_width)
    : Ellipse(Vector2Df(radius, radius), center, fill, stroke, stroke_width) {}

std::string Circle::getClass() const { return "Circle"; }

void Circle::render(Renderer &renderer) const { renderer.renderCircle(*this); }