#include "Polygon.hpp"

Polygon::Polygon(sf::Color fill, sf::Color stroke, float stroke_thickness,
                 int _scale)
    : scale(_scale) {
    setFillColor(fill);
    setOutlineColor(stroke);
    setOutlineThickness(stroke_thickness);
    update();
}

std::size_t Polygon::getPointCount() const {
    return points.size() * static_cast< int >(scale);
}

sf::Vector2f Polygon::getPoint(std::size_t index) const {
    if (index < static_cast< std::size_t >(points.size() * scale)) {
        std::size_t i = index / static_cast< int >(scale);
        std::size_t j = index % static_cast< int >(scale);

        sf::Vector2f p1 = points[i];
        sf::Vector2f p2 = points[(i + 1) % points.size()];

        float scaleFloat = static_cast< float >(scale);
        sf::Vector2f point =
            p1 + (p2 - p1) * static_cast< float >(j) / scaleFloat;

        return point;
    }
    return sf::Vector2f();
}
void Polygon::addPoint(const sf::Vector2f& point) {
    points.push_back(point);
    update();
}