#include "Polyline.hpp"

Plyline::Plyline(const ColorShape& fill, const ColorShape& stroke,
                 float stroke_width)
    : PolyShape(fill, stroke, stroke_width) {}

std::string Plyline::getClass() const { return "Polyline"; }