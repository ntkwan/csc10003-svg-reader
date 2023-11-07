#include "Polyline.hpp"

PolyLine::PolyLine(float stroke_Width, const sf::Color& stroke_color,
                   const sf::Color& fill) {
    setOutlineThickness(stroke_Width);
    setOutlineColor(stroke_color);
    setFillColor(fill);
}

void PolyLine::addPoint(const sf::Vector2f& point) { points.push_back(point); }

void PolyLine::polylineUpdate() { update(); }

sf::Vector2f PolyLine::getPoint(std::size_t index) const {
    if (index < points.size()) {
        return points[index];
    }
    return sf::Vector2f(0, 0);
}

std::size_t PolyLine::getPointCount() const { return points.size(); }
void PolyLine::draw(sf::RenderWindow& window) {
    if (points.size() < 2) return;
    sf::VertexArray lineStrip(sf::PrimitiveType::Quads);
    sf::Vector2f p1a, p1b, p2a, p2b;
    for (std::size_t i = 1; i < points.size(); i++) {
        sf::Vector2f p1 = points[i - 1];
        sf::Vector2f p2 = points[i];

        sf::Vector2f delta = p2 - p1;
        float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);

        sf::Vector2f unitDirection = delta / length;

        sf::Vector2f perpendicularDirection(-unitDirection.y, unitDirection.x);

        float thickness = getOutlineThickness();
        sf::Color stroke = getOutlineColor();

        p1a = p1 - perpendicularDirection * (thickness / 2.0f);
        p1b = p1 + perpendicularDirection * (thickness / 2.0f);
        if (i > 1 && thickness >= 2) {
            lineStrip.append(sf::Vertex(p1a, stroke));
            lineStrip.append(sf::Vertex(p1b, stroke));
            lineStrip.append(sf::Vertex(p2b, stroke));
            lineStrip.append(sf::Vertex(p2a, stroke));
        }
        p2a = p2 - perpendicularDirection * (thickness / 2.0f);
        p2b = p2 + perpendicularDirection * (thickness / 2.0f);
        lineStrip.append(sf::Vertex(p1a, stroke));
        lineStrip.append(sf::Vertex(p1b, stroke));
        lineStrip.append(sf::Vertex(p2b, stroke));
        lineStrip.append(sf::Vertex(p2a, stroke));
    }
    std::vector< ClosedPolygon > cP = findClosedPolygons(points);
    if (cP.size() > 0) {
        sf::Color fillColor = getFillColor();
        for (const ClosedPolygon& polygon : cP) {
            if (polygon.cP.size() > 2) {  // Ensure it's a valid polygon
                sf::ConvexShape fillShape;
                fillShape.setFillColor(fillColor);
                fillShape.setOutlineThickness(0);
                fillShape.setPointCount(polygon.cP.size());

                // Set the points of the shape based on the polygon
                for (std::size_t j = 0; j < polygon.cP.size(); j++) {
                    fillShape.setPoint(j, polygon.cP[j]);
                }

                window.draw(fillShape);
            }
        }
        window.draw(lineStrip);
    }
}