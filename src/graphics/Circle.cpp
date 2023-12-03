#include "Circle.hpp"

Circle::Circle(float radius, const Vector2Df &center, mColor fill,
               mColor stroke, float stroke_width)
    : Ell(Vector2Df(radius, radius), center, fill, stroke, stroke_width) {}

std::string Circle::getClass() const { return "Circle"; }

// void Circle::render(Renderer &renderer) const { renderer.renderCircle(*this);
// }