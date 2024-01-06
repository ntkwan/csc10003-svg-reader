#include "Circle.hpp"

Circle::Circle(float radius, const Vector2Df &center, ColorShape fill,
               ColorShape stroke, float stroke_width)
    : Ell(Vector2Df(radius, radius), center, fill, stroke, stroke_width) {}

std::string Circle::getClass() const { return "Circle"; }
