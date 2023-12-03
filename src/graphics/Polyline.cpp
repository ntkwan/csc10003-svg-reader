#include "Polyline.hpp"

// #include "Renderer.hpp"

Polyline::Polyline(const Color& fill, const Color& stroke, float stroke_width)
    : PolyShape(fill, stroke, stroke_width) {}

std::string Polyline::getClass() const { return "Polyline"; }

// void Polyline::render(Renderer& renderer) const {
//     renderer.renderPolyline(*this);
// }