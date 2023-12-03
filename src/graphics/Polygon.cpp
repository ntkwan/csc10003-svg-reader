#include "Polygon.hpp"

// #include "Renderer.hpp"

Plygon::Plygon(mColor fill, mColor stroke, float stroke_width)
    : PolyShape(fill, stroke, stroke_width) {}

std::string Plygon::getClass() const { return "Polygon"; }

// void Polygon::render(Renderer& renderer) const {
//     renderer.renderPolygon(*this);
// }