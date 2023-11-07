#include "Polygon.hpp"

Polygon::Polygon(sf::Color fill, sf::Color stroke, float stroke_thickness) {
    setFillColor(fill);
    setOutlineColor(stroke);
    setOutlineThickness(stroke_thickness);
}

std::size_t Polygon::getPointCount() const {
    return points.size() * static_cast< int >(scale);
}

sf::Vector2f Polygon::getPoint(std::size_t index) const {
    if (points.size() < 2) {
        return sf::Vector2f();
    }

    std::size_t i = index / scale;
    std::size_t j = index % scale;

    sf::Vector2f p1 = points[i];
    sf::Vector2f p2 = points[(i + 1) % points.size()];

    float t = static_cast< float >(j) / scale;
    sf::Vector2f point = p1 + (p2 - p1) * t;

    return point;
}

void Polygon::addPoint(const sf::Vector2f& point) { points.push_back(point); }

void Polygon::polygonUpdate() { update(); }