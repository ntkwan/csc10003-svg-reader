#include "Polygon.hpp"

Polygon::Polygon(sf::Color fill, sf::Color stroke, float stroke_thickness) {
    setFillColor(fill);
    setOutlineColor(stroke);
    setOutlineThickness(stroke_thickness);
}

std::size_t Polygon::getPointCount() const { return points.size(); }

sf::Vector2f Polygon::getPoint(std::size_t index) const {
    if (index < points.size()) {
        return points[index];
    } else {
        return points[index % points.size()];
    }
}

void Polygon::addPoint(const sf::Vector2f& point) { points.push_back(point); }

void Polygon::polygonUpdate() { update(); }