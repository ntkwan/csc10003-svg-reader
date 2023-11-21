#include "Circle.hpp"

Circle::Circle(float radius, const Vector2Df &center, Color fill, Color stroke,
               float stroke_thickness)
    : Ellipse(Vector2Df(radius, radius), center, fill, stroke,
              stroke_thickness) {}