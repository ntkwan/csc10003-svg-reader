#include "Renderer.hpp"

Renderer* Renderer::instance = nullptr;

Renderer::Renderer(Gdiplus::Graphics& graphics) : graphics(graphics) {}

Renderer* Renderer::getInstance(Gdiplus::Graphics& graphics) {
    if (instance == nullptr) {
        instance = new Renderer(graphics);
    }
    return instance;
}

void Renderer::draw(SVGElement* shape) const {
    std::cout << shape->getClass() << std::endl;
    if (shape->getClass() == "Group") {
        Group* group = dynamic_cast< Group* >(shape);
        for (auto elem : group->getElements()) {
            std::cout << elem->getClass() << std::endl;
            if (elem->getClass() == "Polyline") {
                Plyline* polyline = dynamic_cast< Plyline* >(elem);
                drawPolyline(polyline);
            } else if (elem->getClass() == "Text") {
                Text* text = dynamic_cast< Text* >(elem);
                drawText(text);
            } else if (elem->getClass() == "Rect") {
                Rect* rectangle = dynamic_cast< Rect* >(elem);
                drawRectangle(rectangle);
            } else if (elem->getClass() == "Circle") {
                Circle* circle = dynamic_cast< Circle* >(elem);
                drawCircle(circle);
            } else if (elem->getClass() == "Ellipse") {
                Ell* ellipse = dynamic_cast< Ell* >(elem);
                drawEllipse(ellipse);
            } else if (elem->getClass() == "Line") {
                Line* line = dynamic_cast< Line* >(elem);
                drawLine(line);
            } else if (elem->getClass() == "Polygon") {
                Plygon* polygon = dynamic_cast< Plygon* >(elem);
                drawPolygon(polygon);
            } else if (elem->getClass() == "Path") {
                Path* path = dynamic_cast< Path* >(elem);
                drawPath(path);
            }
        }
    } else if (shape->getClass() == "Polyline") {
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
    mColor outline_color = rectangle->getOutlineColor();

    Gdiplus::Pen RectOutline(Gdiplus::Color(outline_color.a, outline_color.r,
                                            outline_color.g, outline_color.b),
                             rectangle->getOutlineThickness());
    Gdiplus::SolidBrush RectFill(
        Gdiplus::Color(fill_color.a, fill_color.r, fill_color.g, fill_color.b));
    graphics.DrawRectangle(&RectOutline, rectangle->getPosition().x,
                           rectangle->getPosition().y, rectangle->getWidth(),
                           rectangle->getHeight());
    graphics.FillRectangle(&RectFill, rectangle->getPosition().x,
                           rectangle->getPosition().y, rectangle->getWidth(),
                           rectangle->getHeight());
}

void Renderer::drawCircle(Circle* circle) const {
    mColor fill_color = circle->getFillColor();
    mColor outline_color = circle->getOutlineColor();
    Gdiplus::Pen circleOutline(Gdiplus::Color(outline_color.a, outline_color.r,
                                              outline_color.g, outline_color.b),
                               circle->getOutlineThickness());
    Gdiplus::SolidBrush circleFill(
        Gdiplus::Color(fill_color.a, fill_color.r, fill_color.g, fill_color.b));
    graphics.DrawEllipse(&circleOutline,
                         circle->getPosition().x - circle->getRadius().x,
                         circle->getPosition().y - circle->getRadius().y,
                         circle->getRadius().x * 2, circle->getRadius().x * 2);
    graphics.FillEllipse(&circleFill,
                         circle->getPosition().x - circle->getRadius().x,
                         circle->getPosition().y - circle->getRadius().y,
                         circle->getRadius().x * 2, circle->getRadius().y * 2);
}

void Renderer::drawEllipse(Ell* ellipse) const {
    mColor fill_color = ellipse->getFillColor();
    mColor outline_color = ellipse->getOutlineColor();
    Gdiplus::Pen ellipseOutline(
        Gdiplus::Color(outline_color.a, outline_color.r, outline_color.g,
                       outline_color.b),
        ellipse->getOutlineThickness());
    Gdiplus::SolidBrush ellipseFill(
        Gdiplus::Color(fill_color.a, fill_color.r, fill_color.g, fill_color.b));

    graphics.DrawEllipse(
        &ellipseOutline, ellipse->getPosition().x - ellipse->getRadius().x,
        ellipse->getPosition().y - ellipse->getRadius().y,
        ellipse->getRadius().x * 2, ellipse->getRadius().y * 2);
    graphics.FillEllipse(
        &ellipseFill, ellipse->getPosition().x - ellipse->getRadius().x,
        ellipse->getPosition().y - ellipse->getRadius().y,
        ellipse->getRadius().x * 2, ellipse->getRadius().y * 2);
}

void Renderer::drawPolygon(Plygon* polygon) const {
    mColor fill_color = polygon->getFillColor();
    mColor outline_color = polygon->getOutlineColor();
    Gdiplus::Pen polygonOutline(
        Gdiplus::Color(outline_color.a, outline_color.r, outline_color.g,
                       outline_color.b),
        polygon->getOutlineThickness());
    Gdiplus::SolidBrush polygonFill(
        Gdiplus::Color(fill_color.a, fill_color.r, fill_color.g, fill_color.b));

    Gdiplus::PointF* points = new Gdiplus::PointF[polygon->getPoints().size()];
    int idx = 0;
    const std::vector< Vector2Df >& vertices = polygon->getPoints();
    for (const Vector2Df vertex : vertices) {
        points[idx++] = Gdiplus::PointF(vertex.x, vertex.y);
    }

    graphics.DrawPolygon(&polygonOutline, points, idx);
    graphics.FillPolygon(&polygonFill, points, idx);
}

void Renderer::drawText(Text* text) const {
    mColor outline_color = text->getOutlineColor();
    mColor fill_color = text->getFillColor();

    Gdiplus::SolidBrush textFill(
        Gdiplus::Color(fill_color.a, fill_color.r, fill_color.g, fill_color.b));

    Gdiplus::Pen textOutline(Gdiplus::Color(outline_color.a, outline_color.r,
                                            outline_color.g, outline_color.b),
                             text->getOutlineThickness());

    // Gdiplus::FontFamily fontFamily(L"Arial");
    // Gdiplus::Font font(&fontFamily, text->getFontSize(),
    //                    Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

    // Gdiplus::PointF position(text->getPosition().x, text->getPosition().y);
    // Gdiplus::GraphicsPath path;

    // path.AddString(text->getContent().c_str(), -1, &fontFamily,
    //                Gdiplus::FontStyleRegular, text->getOutlineThickness(),
    //                position, NULL);

    // graphics.DrawPath(&textOutline, &path);
    // graphics.FillPath(&textFill, &path);
}

void Renderer::drawPolyline(Plyline* polyline) const {
    mColor color = polyline->getOutlineColor();
    Gdiplus::Pen polylinePen(Gdiplus::Color(color.a, color.r, color.g, color.b),
                             polyline->getOutlineThickness());
    Gdiplus::GraphicsPath path;
    const std::vector< Vector2Df >& points = polyline->getPoints();
    if (points.size() < 2) {
        return;
    }

    path.StartFigure();
    path.AddLine(points[0].x, points[0].y, points[1].x, points[1].y);
    for (size_t i = 2; i < points.size(); ++i) {
        path.AddLine(points[i - 1].x, points[i - 1].y, points[i].x,
                     points[i].y);
    }
    graphics.DrawPath(&polylinePen, &path);
}

void Renderer::drawPath(Path* path) const {
    mColor outline_color = path->getOutlineColor();
    Gdiplus::Pen pathPen(Gdiplus::Color(outline_color.a, outline_color.r,
                                        outline_color.g, outline_color.b),
                         path->getOutlineThickness());

    Gdiplus::GraphicsPath gdiPath;

    const std::vector< PathPoint >& points = path->getPoints();
    int n = points.size();
    Vector2Df firstPoint{0, 0}, curPoint{0, 0};

    for (int i = 0; i < n; ++i) {
        if (points[i].TC == 'M') {
            firstPoint = points[i].Point;
            gdiPath.StartFigure();
            gdiPath.AddLine(firstPoint.x, firstPoint.y, firstPoint.x,
                            firstPoint.y);
            curPoint = firstPoint;
        } else if (points[i].TC == 'm') {
            firstPoint.x = curPoint.x + points[i].Point.x;
            firstPoint.y = curPoint.y + points[i].Point.y;
            gdiPath.StartFigure();
            gdiPath.AddLine(firstPoint.x, firstPoint.y, firstPoint.x,
                            firstPoint.y);
            curPoint = firstPoint;
        } else if (points[i].TC == 'L') {
            gdiPath.AddLine(points[i].Point.x, points[i].Point.y,
                            points[i].Point.x, points[i].Point.y);
            curPoint = points[i].Point;
        } else if (points[i].TC == 'l') {
            Vector2Df endPoint{curPoint.x + points[i].Point.x,
                               curPoint.y + points[i].Point.y};
            gdiPath.AddLine(endPoint.x, endPoint.y, endPoint.x, endPoint.y);
            curPoint = endPoint;
        } else if (points[i].TC == 'C') {
            if (i + 2 < n) {
                Vector2Df controlPoint1 = points[i].Point;
                Vector2Df controlPoint2 = points[i + 1].Point;
                Vector2Df controlPoint3 = points[i + 2].Point;
                gdiPath.AddBezier(curPoint.x, curPoint.y, controlPoint1.x,
                                  controlPoint1.y, controlPoint2.x,
                                  controlPoint2.y, controlPoint3.x,
                                  controlPoint3.y);
                i += 2;
            }
            curPoint = points[i].Point;
        } else if (points[i].TC == 'c') {
            if (i + 2 < n) {
                Vector2Df controlPoint1 =
                    Vector2Df{curPoint.x + points[i].Point.x,
                              curPoint.y + points[i].Point.y};
                Vector2Df controlPoint2 =
                    Vector2Df{curPoint.x + points[i + 1].Point.x,
                              curPoint.y + points[i + 1].Point.y};
                Vector2Df controlPoint3 =
                    Vector2Df{curPoint.x + points[i + 1].Point.x,
                              curPoint.y + points[i + 1].Point.y};
                gdiPath.AddBezier(curPoint.x, curPoint.y, controlPoint1.x,
                                  controlPoint1.y, controlPoint2.x,
                                  controlPoint2.y, controlPoint3.x,
                                  controlPoint3.y);
                i += 2;
                curPoint = controlPoint3;
            }
        } else if (points[i].TC == 'Z' || points[i].TC == 'z') {
            gdiPath.CloseFigure();
            curPoint = firstPoint;
        }
    }
    graphics.DrawPath(&pathPen, &gdiPath);
}
