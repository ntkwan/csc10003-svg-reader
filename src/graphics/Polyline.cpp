#include "Polyline.hpp"

namespace {
    struct Edge {
        sf::Vector2f start, end;
    };
    struct ClosedPolygon {
        std::vector< sf::Vector2f > cP;
    };
    struct PolygonPoint {
        sf::Vector2f point;
        bool fill;
    };
    // point to edge follow the circle shape
    std::vector< Edge > pointToEdge(const std::vector< sf::Vector2f >& points) {
        std::vector< Edge > edges;
        int n = points.size();
        for (int i = 0; i < n; i++) {
            if (i == n - 1)
                edges.push_back({points[0], points[i]});  // virtual edge
            else
                edges.push_back({points[i], points[i + 1]});
        }
        return edges;
    }
    // check the intersection between two edge
    bool isIntersection(const Edge& edge1, const Edge& edge2) {
        float x1 = edge1.start.x;
        float y1 = edge1.start.y;
        float x2 = edge1.end.x;
        float y2 = edge1.end.y;

        float x3 = edge2.start.x;
        float y3 = edge2.start.y;
        float x4 = edge2.end.x;
        float y4 = edge2.end.y;

        float det = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

        if (det == 0) {
            // The lines are coincident
            return 0;
        } else {
            float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / det;
            float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / det;

            if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
                // The lines are intersecting
                return 1;
            }
        }
        // The lines are not intersecting
        return 0;
    }
    int findPointPosition(const std::vector< PolygonPoint >& points,
                          const sf::Vector2f& point) {
        int n = points.size();
        for (int i = 0; i < n; i++) {
            if (points[i].point.x == point.x && points[i].point.y == point.y)
                return i;
        }
        return -1;
    }
    // merge the intersection points between the virtual edge and polyline edges
    std::vector< PolygonPoint > mergeIntersectionPoints(
        const std::vector< sf::Vector2f >& points) {
        std::vector< Edge > edges = pointToEdge(points);
        int n = edges.size();

        std::vector< PolygonPoint > _points;
        for (int i = 0; i < points.size(); i++) {
            _points.push_back({points[i], 0});
        }

        for (int i = 0; i < n - 1; i++) {
            if (isIntersection(edges[i], edges[n - 1])) {
                float x1 = edges[i].start.x;
                float y1 = edges[i].start.y;
                float x2 = edges[i].end.x;
                float y2 = edges[i].end.y;

                float x3 = edges[n - 1].start.x;
                float y3 = edges[n - 1].start.y;
                float x4 = edges[n - 1].end.x;
                float y4 = edges[n - 1].end.y;

                float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) /
                          ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
                float intersectionX = x1 + t * (x2 - x1);
                float intersectionY = y1 + t * (y2 - y1);

                sf::Vector2f intersectionPoint{intersectionX, intersectionY};
                PolygonPoint intersectionPolygonPoint{intersectionPoint, 1};

                int posInsert = findPointPosition(_points, edges[i].start);
                if (posInsert != -1)
                    _points.emplace(_points.begin() + posInsert + 1,
                                    intersectionPolygonPoint);
            }
        }
        return _points;
    }
    // find the intersection points between the virtual edge and polyline edges
    std::vector< sf::Vector2f > findIntersectionPoints(
        const std::vector< sf::Vector2f >& points) {
        std::vector< Edge > edges = pointToEdge(points);
        int n = edges.size();

        std::vector< sf::Vector2f > _points;

        for (int i = 0; i < n - 1; i++) {
            if (isIntersection(edges[i], edges[n - 1])) {
                float x1 = edges[i].start.x;
                float y1 = edges[i].start.y;
                float x2 = edges[i].end.x;
                float y2 = edges[i].end.y;

                float x3 = edges[n - 1].start.x;
                float y3 = edges[n - 1].start.y;
                float x4 = edges[n - 1].end.x;
                float y4 = edges[n - 1].end.y;

                float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) /
                          ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
                float intersectionX = x1 + t * (x2 - x1);
                float intersectionY = y1 + t * (y2 - y1);

                sf::Vector2f intersectionPoint{intersectionX, intersectionY};

                _points.push_back(intersectionPoint);
            }
        }
        return _points;
    }
    // find closed polygon to fill color
    std::vector< ClosedPolygon > findClosedPolygons(
        const std::vector< sf::Vector2f >& points) {
        std::vector< ClosedPolygon > _cP;
        std::vector< PolygonPoint > mPoints = mergeIntersectionPoints(points);
        int n = mPoints.size();
        ClosedPolygon pushCP;
        bool startPolygon = false;
        for (int i = 0; i < n; i++) {
            if (mPoints[i].fill) {
                if (!startPolygon) {
                    startPolygon = true;
                    pushCP.cP.push_back(mPoints[i].point);
                } else if (startPolygon) {
                    pushCP.cP.push_back(mPoints[i].point);
                    _cP.push_back(pushCP);
                    pushCP.cP.clear();
                    pushCP.cP.push_back(mPoints[i].point);
                }
            } else if (startPolygon)
                pushCP.cP.push_back(mPoints[i].point);
        }
        return _cP;
    }
}  // namespace

Polyline::Polyline(float stroke_width, const sf::Color& stroke_color,
                   const sf::Color& fill) {
    setOutlineThickness(stroke_width);
    setOutlineColor(stroke_color);
    setFillColor(fill);
}

void Polyline::addPoint(const sf::Vector2f& point) { points.push_back(point); }

void Polyline::polylineUpdate() { update(); }

sf::Vector2f Polyline::getPoint(std::size_t index) const {
    if (index < points.size()) {
        return points[index];
    }
    return sf::Vector2f(0, 0);
}

std::size_t Polyline::getPointCount() const { return points.size(); }
void Polyline::draw(sf::RenderWindow& target, sf::RenderStates states) const {
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

                target.draw(fillShape);
            }
        }
        target.draw(lineStrip);
    }
}