#include "Polygon.hpp"

#include "Renderer.hpp"

Polygon::Polygon(Color fill, Color stroke, float stroke_width)
    : PolyShape(fill, stroke, stroke_width) {}

std::string Polygon::getClass() const { return "Polygon"; }

void Polygon::render(Renderer& renderer) const {
    renderer.renderPolygon(*this);
}