#include "Polygon.hpp"

Plygon::Plygon(ColorShape fill, ColorShape stroke, float stroke_width)
    : PolyShape(fill, stroke, stroke_width) {}

std::string Plygon::getClass() const { return "Polygon"; }