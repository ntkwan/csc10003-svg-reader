#include "Renderer.hpp"

Renderer* Renderer::instance = nullptr;

Renderer::Renderer(Gdiplus::Graphics& graphics) : graphics(graphics) {
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias8x8);
}

Renderer* Renderer::getInstance(Gdiplus::Graphics& graphics) {
    if (instance == nullptr) {
        instance = new Renderer(graphics);
    }
    return instance;
}

std::pair< float, float > getTranslate(std::string transform_value) {
    float trans_x = 0, trans_y = 0;
    if (transform_value.find(",") != std::string::npos) {
        sscanf(transform_value.c_str(), "translate(%f, %f)", &trans_x,
               &trans_y);
    } else {
        sscanf(transform_value.c_str(), "translate(%f %f)", &trans_x, &trans_y);
    }
    return std::pair< float, float >(trans_x, trans_y);
}

float getRotate(std::string transform_value) {
    float degree = 0;
    sscanf(transform_value.c_str(), "rotate(%f)", &degree);
    return degree;
}

float getScale(std::string transform_value) {
    float scale = 0;
    sscanf(transform_value.c_str(), "scale(%f)", &scale);
    return scale;
}

std::pair< float, float > getScaleXY(std::string transform_value) {
    float scale_x = 0, scale_y = 0;
    if (transform_value.find(",") != std::string::npos)
        sscanf(transform_value.c_str(), "scale(%f, %f)", &scale_x, &scale_y);
    else
        sscanf(transform_value.c_str(), "scale(%f %f)", &scale_x, &scale_y);
    return std::pair< float, float >(scale_x, scale_y);
}

void Renderer::applyTransform(
    std::vector< std::string > transform_order) const {
    for (auto type : transform_order) {
        if (type.find("translate") != std::string::npos) {
            float trans_x = getTranslate(type).first,
                  trans_y = getTranslate(type).second;
            graphics.TranslateTransform(trans_x, trans_y);
        } else if (type.find("rotate") != std::string::npos) {
            float degree = getRotate(type);
            graphics.RotateTransform(degree);
        } else if (type.find("scale") != std::string::npos) {
            if (type.find(",") != std::string::npos) {
                float scale_x = getScaleXY(type).first,
                      scale_y = getScaleXY(type).second;
                graphics.ScaleTransform(scale_x, scale_y);
            } else {
                float scale = getScale(type);
                graphics.ScaleTransform(scale, scale);
            }
        }
    }
}

void Renderer::draw(SVGElement* shape) const {
    if (shape->getClass() == "Group") {
        Group* group = dynamic_cast< Group* >(shape);
        group->render(*this);
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
    Gdiplus::Matrix original;
    graphics.GetTransform(&original);
    applyTransform(line->getTransforms());
    graphics.DrawLine(&linePen, startPoint, endPoint);
    graphics.SetTransform(&original);
}

void Renderer::drawRectangle(Rect* rectangle) const {
    mColor fill_color = rectangle->getFillColor();
    mColor outline_color = rectangle->getOutlineColor();

    Gdiplus::Pen RectOutline(Gdiplus::Color(outline_color.a, outline_color.r,
                                            outline_color.g, outline_color.b),
                             rectangle->getOutlineThickness());
    Gdiplus::SolidBrush RectFill(
        Gdiplus::Color(fill_color.a, fill_color.r, fill_color.g, fill_color.b));
    Gdiplus::Matrix original;
    graphics.GetTransform(&original);
    applyTransform(rectangle->getTransforms());
    graphics.FillRectangle(&RectFill, rectangle->getPosition().x,
                           rectangle->getPosition().y, rectangle->getWidth(),
                           rectangle->getHeight());
    graphics.DrawRectangle(&RectOutline, rectangle->getPosition().x,
                           rectangle->getPosition().y, rectangle->getWidth(),
                           rectangle->getHeight());
    graphics.SetTransform(&original);
}

void Renderer::drawCircle(Circle* circle) const {
    mColor fill_color = circle->getFillColor();
    mColor outline_color = circle->getOutlineColor();
    Gdiplus::Pen circleOutline(Gdiplus::Color(outline_color.a, outline_color.r,
                                              outline_color.g, outline_color.b),
                               circle->getOutlineThickness());
    Gdiplus::SolidBrush circleFill(
        Gdiplus::Color(fill_color.a, fill_color.r, fill_color.g, fill_color.b));
    Gdiplus::Matrix original;
    graphics.GetTransform(&original);
    applyTransform(circle->getTransforms());
    graphics.FillEllipse(&circleFill,
                         circle->getPosition().x - circle->getRadius().x,
                         circle->getPosition().y - circle->getRadius().y,
                         circle->getRadius().x * 2, circle->getRadius().y * 2);
    graphics.DrawEllipse(&circleOutline,
                         circle->getPosition().x - circle->getRadius().x,
                         circle->getPosition().y - circle->getRadius().y,
                         circle->getRadius().x * 2, circle->getRadius().x * 2);
    graphics.SetTransform(&original);
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
    Gdiplus::Matrix original;
    graphics.GetTransform(&original);
    applyTransform(ellipse->getTransforms());
    graphics.FillEllipse(
        &ellipseFill, ellipse->getPosition().x - ellipse->getRadius().x,
        ellipse->getPosition().y - ellipse->getRadius().y,
        ellipse->getRadius().x * 2, ellipse->getRadius().y * 2);
    graphics.DrawEllipse(
        &ellipseOutline, ellipse->getPosition().x - ellipse->getRadius().x,
        ellipse->getPosition().y - ellipse->getRadius().y,
        ellipse->getRadius().x * 2, ellipse->getRadius().y * 2);
    graphics.SetTransform(&original);
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

    Gdiplus::FillMode fillMode;
    if (polygon->getFillRule() == "evenodd") {
        fillMode = Gdiplus::FillModeAlternate;
    } else if (polygon->getFillRule() == "nonzero") {
        fillMode = Gdiplus::FillModeWinding;
    }
    Gdiplus::Matrix original;
    graphics.GetTransform(&original);
    applyTransform(polygon->getTransforms());
    graphics.FillPolygon(&polygonFill, points, idx, fillMode);
    graphics.DrawPolygon(&polygonOutline, points, idx);
    graphics.SetTransform(&original);
    delete[] points;
}

#include <codecvt>
#include <locale>
void Renderer::drawText(Text* text) const {
    mColor outline_color = text->getOutlineColor();
    mColor fill_color = text->getFillColor();

    graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);

    Gdiplus::SolidBrush textFill(
        Gdiplus::Color(fill_color.a, fill_color.r, fill_color.g, fill_color.b));

    Gdiplus::Pen textOutline(Gdiplus::Color(outline_color.a, outline_color.r,
                                            outline_color.g, outline_color.b),
                             text->getOutlineThickness());

    Gdiplus::FontFamily fontFamily(L"Times New Roman");

    Gdiplus::PointF position(text->getPosition().x, text->getPosition().y);
    Gdiplus::GraphicsPath path;

    std::wstring_convert< std::codecvt_utf8_utf16< wchar_t > > converter;
    std::wstring wideContent = converter.from_bytes(text->getContent());
    Gdiplus::StringFormat stringFormat;
    if (text->getAnchor() == "middle") {
        stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
        position.X += 7;
    } else if (text->getAnchor() == "end") {
        stringFormat.SetAlignment(Gdiplus::StringAlignmentFar);
        position.X += 14;
    } else {
        stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
    }
    Gdiplus::FontStyle fontStyle = Gdiplus::FontStyleRegular;
    if (text->getFontStyle() == "italic" || text->getFontStyle() == "oblique") {
        fontStyle = Gdiplus::FontStyleItalic;
        position.Y -= 1;
    }

    path.AddString(wideContent.c_str(), wideContent.size(), &fontFamily,
                   fontStyle, text->getFontSize(), position, &stringFormat);
    Gdiplus::Matrix original;
    graphics.GetTransform(&original);
    applyTransform(text->getTransforms());
    graphics.FillPath(&textFill, &path);
    graphics.DrawPath(&textOutline, &path);
    graphics.SetTransform(&original);
}

void Renderer::drawPolyline(Plyline* polyline) const {
    mColor outline_color = polyline->getOutlineColor();
    mColor fill_color = polyline->getFillColor();
    Gdiplus::Pen polylinePen(Gdiplus::Color(outline_color.a, outline_color.r,
                                            outline_color.g, outline_color.b),
                             polyline->getOutlineThickness());
    Gdiplus::SolidBrush polylineFill(
        Gdiplus::Color(fill_color.a, fill_color.r, fill_color.g, fill_color.b));

    Gdiplus::FillMode fillMode;
    if (polyline->getFillRule() == "evenodd") {
        fillMode = Gdiplus::FillModeAlternate;
    } else if (polyline->getFillRule() == "nonzero") {
        fillMode = Gdiplus::FillModeWinding;
    }
    Gdiplus::GraphicsPath path(fillMode);
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
    Gdiplus::Matrix original;
    graphics.GetTransform(&original);
    applyTransform(polyline->getTransforms());
    graphics.FillPath(&polylineFill, &path);
    graphics.DrawPath(&polylinePen, &path);
    graphics.SetTransform(&original);
}

void Renderer::drawPath(Path* path) const {
    mColor outline_color = path->getOutlineColor();
    mColor fill_color = path->getFillColor();
    Gdiplus::Pen pathPen(Gdiplus::Color(outline_color.a, outline_color.r,
                                        outline_color.g, outline_color.b),
                         path->getOutlineThickness());
    Gdiplus::SolidBrush pathFill(
        Gdiplus::Color(fill_color.a, fill_color.r, fill_color.g, fill_color.b));

    Gdiplus::FillMode fillMode;
    if (path->getFillRule() == "evenodd") {
        fillMode = Gdiplus::FillModeAlternate;
    } else if (path->getFillRule() == "nonzero") {
        fillMode = Gdiplus::FillModeWinding;
    }
    Gdiplus::GraphicsPath gdiPath(fillMode);

    const std::vector< PathPoint >& points = path->getPoints();
    int n = points.size();
    Vector2Df firstPoint{0, 0}, curPoint{0, 0};

    for (int i = 0; i < n; ++i) {
        if (points[i].TC == 'M') {
            firstPoint = points[i].Point;
            gdiPath.StartFigure();
            curPoint = firstPoint;
        } else if (points[i].TC == 'm') {
            firstPoint.x = curPoint.x + points[i].Point.x;
            firstPoint.y = curPoint.y + points[i].Point.y;
            gdiPath.StartFigure();
            curPoint = firstPoint;
        } else if (points[i].TC == 'L') {
            gdiPath.AddLine(curPoint.x, curPoint.y, points[i].Point.x,
                            points[i].Point.y);
            curPoint = points[i].Point;
        } else if (points[i].TC == 'l') {
            Vector2Df endPoint{curPoint.x + points[i].Point.x,
                               curPoint.y + points[i].Point.y};
            gdiPath.AddLine(curPoint.x, curPoint.y, endPoint.x, endPoint.y);
            curPoint = endPoint;
        } else if (points[i].TC == 'H') {
            Vector2Df endPoint{points[i].Point.x, curPoint.y};
            gdiPath.AddLine(curPoint.x, curPoint.y, endPoint.x, endPoint.y);
            curPoint = endPoint;
        } else if (points[i].TC == 'h') {
            Vector2Df endPoint{curPoint.x + points[i].Point.x, curPoint.y};
            gdiPath.AddLine(curPoint.x, curPoint.y, endPoint.x, endPoint.y);
            curPoint = endPoint;
        } else if (points[i].TC == 'V') {
            Vector2Df endPoint{curPoint.x, points[i].Point.y};
            gdiPath.AddLine(curPoint.x, curPoint.y, endPoint.x, endPoint.y);
            curPoint = endPoint;
        } else if (points[i].TC == 'v') {
            Vector2Df endPoint{curPoint.x, curPoint.y + points[i].Point.y};
            gdiPath.AddLine(curPoint.x, curPoint.y, endPoint.x, endPoint.y);
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
                curPoint = controlPoint3;
            }
        } else if (points[i].TC == 'c') {
            if (i + 2 < n) {
                Vector2Df controlPoint1 =
                    Vector2Df{curPoint.x + points[i].Point.x,
                              curPoint.y + points[i].Point.y};
                Vector2Df controlPoint2 =
                    Vector2Df{curPoint.x + points[i + 1].Point.x,
                              curPoint.y + points[i + 1].Point.y};
                Vector2Df controlPoint3 =
                    Vector2Df{curPoint.x + points[i + 2].Point.x,
                              curPoint.y + points[i + 2].Point.y};
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
    Gdiplus::Matrix original;
    graphics.GetTransform(&original);
    applyTransform(path->getTransforms());
    graphics.FillPath(&pathFill, &gdiPath);
    graphics.DrawPath(&pathPen, &gdiPath);
    graphics.SetTransform(&original);
}
