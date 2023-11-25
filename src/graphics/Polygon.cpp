#include "Polygon.hpp"

Polygon::Polygon(Color fill, Color stroke, float stroke_width)
    : PolyShape(fill, stroke, stroke_width) {}

std::string Polygon::getClass() const { return "Polygon"; }