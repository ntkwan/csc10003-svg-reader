#include "Polyline.hpp"

// #include "Renderer.hpp"

Plyline::Plyline(const mColor& fill, const mColor& stroke, float stroke_width)
    : PolyShape(fill, stroke, stroke_width) {}

std::string Plyline::getClass() const { return "Polyline"; }

// void Polyline::render(Renderer& renderer) const {
//     renderer.renderPolyline(*this);
// }

void Plyline::setFillRule(std::string fill_rule) {
    this->fill_rule = fill_rule;
}

std::string Plyline::getFillRule() const { return fill_rule; }