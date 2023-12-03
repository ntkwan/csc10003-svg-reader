#include "Renderer.hpp"

Renderer* Renderer::instance = nullptr;

Renderer::Renderer(Gdiplus::Graphics& graphics) : graphics(graphics) {}

Renderer* Renderer::getInstance(Gdiplus::Graphics& graphics) {
    if (instance == nullptr) {
        instance = new Renderer(graphics);
    }
    return instance;
}

void Renderer::draw(Shape* shape) const {
    if (shape->getClass() == "Polyline") {
        Plyline* polyline = dynamic_cast< Plyline* >(shape);
        drawPolyline(polyline);
    } else if (shape->getClass() == "Text") {
        Text* text = dynamic_cast< Text* >(shape);
        drawText(text);
    } else if (shape->getClass() == "Rect") {
        Rect* rectangle = dynamic_cast< Rect* >(shape);
        drawRectangle(rectangle);
    } else if (shape->getClass() == "Circle") {
        Circle* circle = dynamic_cast< Circle* >(shape);
        drawCircle(circle);
    } else if (shape->getClass() == "Ellipse") {
        Ell* ellipse = dynamic_cast< Ell* >(shape);
        drawEllipse(ellipse);
    } else if (shape->getClass() == "Line") {
        Line* line = dynamic_cast< Line* >(shape);
        drawLine(line);
    } else if (shape->getClass() == "Polygon") {
        Plygon* polygon = dynamic_cast< Plygon* >(shape);
        drawPolygon(polygon);
    } else if (shape->getClass() == "Path") {
        Path* path = dynamic_cast< Path* >(shape);
        drawPath(path);
    }
}

void Renderer::drawLine(Line* line) const {
    mColor color = line->getOutlineColor();
    Gdiplus::Pen linePen(Gdiplus::Color(color.a, color.r, color.g, color.b),
                         line->getOutlineThickness());
    Gdiplus::PointF startPoint(line->getPosition().x, line->getPosition().y);
    Gdiplus::PointF endPoint(line->getDirection().x, line->getDirection().y);
    graphics.DrawLine(&linePen, startPoint, endPoint);
}

void Renderer::drawRectangle(Rect* rectangle) const {
    mColor fill_color = rectangle->getFillColor();
    Gdiplus::SolidBrush RectFill(
        Gdiplus::Color(fill_color.a, fill_color.r, fill_color.g, fill_color.b));
    graphics.FillRectangle(&RectFill, rectangle->getPosition().x,
                           rectangle->getPosition().y, rectangle->getWidth(),
                           rectangle->getHeight());
    mColor outline_color = rectangle->getOutlineColor();
    Gdiplus::Pen RectOutline(Gdiplus::Color(outline_color.a, outline_color.r,
                                            outline_color.g, outline_color.b),
                             rectangle->getOutlineThickness());
    graphics.DrawRectangle(&RectOutline, rectangle->getPosition().x,
                           rectangle->getPosition().y, rectangle->getWidth(),
                           rectangle->getHeight());
}

void Renderer::drawCircle(Circle* circle) const {
    mColor fill_color = circle->getFillColor();
    Gdiplus::SolidBrush circleFill(
        Gdiplus::Color(fill_color.a, fill_color.r, fill_color.g, fill_color.b));

    mColor outline_color = circle->getOutlineColor();
    Gdiplus::Pen circleOutline(Gdiplus::Color(outline_color.a, outline_color.r,
                                              outline_color.g, outline_color.b),
                               circle->getOutlineThickness());
    float ellipseX = circle->getPosition().x - circle->getRadius().x;
    float ellipseY = circle->getPosition().y - circle->getRadius().y;
    float ellipseWidth = 2 * circle->getRadius().x;
    float ellipseHeight = 2 * circle->getRadius().y;
    graphics.FillEllipse(&circleFill, ellipseX, ellipseY, ellipseWidth,
                         ellipseHeight);
    graphics.DrawEllipse(&circleOutline, ellipseX, ellipseY, ellipseWidth,
                         ellipseHeight);
}

void Renderer::drawEllipse(Ell* ellipse) const {
    mColor fill_color = ellipse->getFillColor();
    Gdiplus::SolidBrush ellipseFill(
        Gdiplus::Color(fill_color.a, fill_color.r, fill_color.g, fill_color.b));
    graphics.FillEllipse(ellipseFill, ellipse->getPosition().x,
                         ellipse->getPosition().y, ellipse->getRadius().x,
                         ellipse->getRadius().y);
    mColor outline_color = ellipse->getOutlineColor();
    Gdiplus::Pen ellipseOutline(
        Gdiplus::Color(outline_color.a, outline_color.r, outline_color.g,
                       outline_color.b),
        ellipse->getOutlineThickness());
    graphics.DrawEllipse(&ellipseOutline, ellipse->getPosition().x,
                         ellipse->getPosition().y, 2 * ellipse->getRadius().x,
                         2 * ellipse->getRadius().y);
}

void Renderer::drawPolygon(Plygon* polygon) const {
    mColor fill_color = polygon->getFillColor();
    Gdiplus::SolidBrush polygonFill(
        Gdiplus::Color(fill_color.a, fill_color.r, fill_color.g, fill_color.b));

    std::vector< Gdiplus::PointF > points;
    const std::vector< Vector2Df >& vertices = polygon->getPoints();
    for (const Vector2Df& vertex : vertices) {
        points.push_back(Gdiplus::PointF(vertex.x, vertex.y));
    }

    graphics.FillPolygon(&polygonFill, &points[0],
                         static_cast< int >(points.size()));

    mColor outline_color = polygon->getOutlineColor();
    Gdiplus::Pen polygonOutline(
        Gdiplus::Color(outline_color.a, outline_color.r, outline_color.g,
                       outline_color.b),
        polygon->getOutlineThickness());
    graphics.DrawPolygon(&polygonOutline, &points[0],
                         static_cast< int >(points.size()));
}

void Renderer::drawText(Text* text) const {}

void Renderer::drawPolyline(Plyline* polyline) const {}

void Renderer::drawPath(Path* path) const {}
