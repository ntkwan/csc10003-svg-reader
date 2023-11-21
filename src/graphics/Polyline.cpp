#include "Polyline.hpp"

namespace {
    struct Edge {
        Vector2Df start, end;
    };
    struct ClosedPolygon {
        std::vector< Vector2Df > cP;
    };
    struct PolygonPoint {
        Vector2Df point;
        bool fill;
    };
    // point to edge follow the circle shape
    std::vector< Edge > pointToEdge(const std::vector< Vector2Df >& points) {
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
    bool isPerpendicular(const Edge& edge1, const Edge& edge2) {
        Vector2Df dir1 = edge1.end - edge1.start;
        Vector2Df dir2 = edge2.end - edge2.start;

        float dotProduct = dir1.x * dir2.x + dir1.y * dir2.y;
        return dotProduct == 0;
    }
    int findPointPosition(const std::vector< PolygonPoint >& points,
                          const Vector2Df& point) {
        int n = points.size();
        for (int i = 0; i < n; i++) {
            if (points[i].point.x == point.x && points[i].point.y == point.y)
                return i;
        }
        return -1;
    }
    bool notExisted(const std::vector< PolygonPoint >& points,
                    const PolygonPoint& point) {
        int n = points.size();
        for (int i = 0; i < n; i++) {
            if (points[i].point.x == point.point.x &&
                points[i].point.y == point.point.y &&
                points[i].fill == point.fill)
                return 0;
        }
        return 1;
    }
    // merge the intersection points between the virtual edge and polyline edges
    std::vector< PolygonPoint > mergeIntersectionPoints(
        const std::vector< Vector2Df >& points) {
        std::vector< Edge > edges = pointToEdge(points);
        int n = edges.size();

        std::vector< PolygonPoint > _points;
        for (int i = 0; i < points.size(); i++) {
            _points.push_back({points[i], 0});
        }
        for (int i = 0; i < n - 1; i++) {
            float x1 = edges[i].start.x;
            float y1 = edges[i].start.y;
            float x2 = edges[i].end.x;
            float y2 = edges[i].end.y;

            float x3 = edges[n - 1].start.x;
            float y3 = edges[n - 1].start.y;
            float x4 = edges[n - 1].end.x;
            float y4 = edges[n - 1].end.y;

            float det = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

            if (det == 0) continue;

            float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) /
                      ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
            float intersectionX = x1 + t * (x2 - x1);
            float intersectionY = y1 + t * (y2 - y1);

            Vector2Df intersectionPoint{intersectionX, intersectionY};
            PolygonPoint intersectionPolygonPoint{intersectionPoint, 1};

            int posInsert = findPointPosition(_points, edges[i].start);
            if (posInsert != -1 &&
                notExisted(_points, intersectionPolygonPoint))
                _points.emplace(_points.begin() + posInsert + 1,
                                intersectionPolygonPoint);
        }
        return _points;
    }
    // find closed polygon to fill color
    std::vector< ClosedPolygon > findClosedPolygons(
        const std::vector< Vector2Df >& points) {
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
    Vector2Df findIntersection(const Edge& edge1, const Edge& edge2) {
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
            return Vector2Df(-1, -1);
        }

        float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) /
                  ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

        float intersectionX = x1 + t * (x2 - x1);
        float intersectionY = y1 + t * (y2 - y1);

        return Vector2Df(intersectionX, intersectionY);
    }
}  // namespace

Polyline::Polyline(float stroke_width, const sf::Color& stroke_color,
                   const sf::Color& fill) {
    setOutlineThickness(stroke_width);
    setOutlineColor(stroke_color);
    setFillColor(fill);
}

void Polyline::addPoint(const Vector2Df& point) { points.push_back(point); }

void Polyline::updateShape() { update(); }

Vector2Df Polyline::getPoint(std::size_t index) const {
    if (index < points.size()) {
        return points[index];
    }
    return Vector2Df(0, 0);
}

std::size_t Polyline::getPointCount() const { return points.size(); }
void Polyline::draw(Renderer& target) const {
    if (points.size() < 2) return;
    sf::VertexArray lineStrip(sf::PrimitiveType::Quads);
    Vector2Df p1a, p1b, p2a, p2b;
    Vector2Df r_p1a, r_p1b, r_p2a, r_p2b;
    for (std::size_t i = 1; i < points.size(); i++) {
        Vector2Df p1 = points[i - 1];
        Vector2Df p2 = points[i];

        Vector2Df delta = p2 - p1;
        float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);

        Vector2Df unitDirection = delta / length;

        Vector2Df perpendicularDirection(-unitDirection.y, unitDirection.x);

        float thickness = getOutlineThickness();
        sf::Color stroke = getOutlineColor();

        p1a = p1 - perpendicularDirection * (thickness / 2.0f);
        p1b = p1 + perpendicularDirection * (thickness / 2.0f);
        p2a = p2 - perpendicularDirection * (thickness / 2.0f);
        p2b = p2 + perpendicularDirection * (thickness / 2.0f);
        if (i > 1) {
            sf::VertexArray lS(sf::PrimitiveType::Quads);
            if (isPerpendicular({points[i], points[i - 1]},
                                {points[i - 1], points[i - 2]})) {
                Vector2Df point_1 =
                    findIntersection({r_p1a, r_p2a}, {p1a, p2a});
                Vector2Df point_2 =
                    findIntersection({r_p1a, r_p2a}, {p1b, p2b});
                Vector2Df point_3 =
                    findIntersection({r_p1b, r_p2b}, {p1b, p2b});
                Vector2Df point_4 =
                    findIntersection({r_p1b, r_p2b}, {p1a, p2a});

                lS.append(
                    sf::Vertex(sf::Vector2f(point_1.x, point_1.y), stroke));
                lS.append(
                    sf::Vertex(sf::Vector2f(point_2.x, point_2.y), stroke));
                lS.append(
                    sf::Vertex(sf::Vector2f(point_3.x, point_3.y), stroke));
                lS.append(
                    sf::Vertex(sf::Vector2f(point_4.x, point_4.y), stroke));

            } else {
                lS.append(sf::Vertex(sf::Vector2f(p1a.x, p1a.y), stroke));
                lS.append(sf::Vertex(sf::Vector2f(r_p2a.x, r_p2a.y), stroke));
                lS.append(sf::Vertex(sf::Vector2f(p1b.x, p1b.y), stroke));
                lS.append(sf::Vertex(sf::Vector2f(r_p2b.x, r_p2b.y), stroke));
            }
            target.window.draw(lS);
        }
        r_p1a = p1a;
        r_p1b = p1b;
        r_p2a = p2a;
        r_p2b = p2b;
        lineStrip.append(sf::Vertex(sf::Vector2f(p1a.x, p1a.y), stroke));
        lineStrip.append(sf::Vertex(sf::Vector2f(p1b.x, p1b.y), stroke));
        lineStrip.append(sf::Vertex(sf::Vector2f(p2b.x, p2b.y), stroke));
        lineStrip.append(sf::Vertex(sf::Vector2f(p2a.x, p2a.y), stroke));
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
                    fillShape.setPoint(
                        j, sf::Vector2f(polygon.cP[j].x, polygon.cP[j].y));
                }

                target.window.draw(fillShape);
            }
        }
        target.window.draw(lineStrip);
    }
}