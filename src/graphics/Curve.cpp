#include "Curve.hpp"

Curve::Curve(const Color& fill, const Color& stroke, float stroke_width)
    : PolyShape(fill, stroke, stroke_width) {}

std::string Curve::getClass() const { return "Curve"; }