#include "Polygon.hpp"

// #include "Renderer.hpp"

Plygon::Plygon(mColor fill, mColor stroke, float stroke_width)
    : PolyShape(fill, stroke, stroke_width) {}

std::string Plygon::getClass() const { return "Polygon"; }

// void Polygon::render(Renderer& renderer) const {
//     renderer.renderPolygon(*this);
// }

void Plygon::setFillRule(std::string fill_rule) { this->fill_rule = fill_rule; }

std::string Plygon::getFillRule() const { return fill_rule; }