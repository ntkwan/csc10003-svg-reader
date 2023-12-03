#include "Polyline.hpp"

// #include "Renderer.hpp"

Plyline::Plyline(const mColor& fill, const mColor& stroke, float stroke_width)
    : PolyShape(fill, stroke, stroke_width) {}

std::string Plyline::getClass() const { return "Polyline"; }

// void Polyline::render(Renderer& renderer) const {
//     renderer.renderPolyline(*this);
// }