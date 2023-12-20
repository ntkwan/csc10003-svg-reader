#include "Renderer.hpp"

Renderer* Renderer::instance = nullptr;

Renderer::Renderer() {}

Renderer* Renderer::getInstance() {
    if (instance == nullptr) {
        instance = new Renderer();
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

void Renderer::applyTransform(std::vector< std::string > transform_order,
                              Gdiplus::Graphics& graphics) const {
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

void Renderer::draw(Gdiplus::Graphics& graphics, Group* group) const {
    for (auto shape : group->getElements()) {
        Gdiplus::Matrix original;
        graphics.GetTransform(&original);
        applyTransform(shape->getTransforms(), graphics);
        if (shape->getClass() == "Group") {
            Group* group = dynamic_cast< Group* >(shape);
            draw(graphics, group);
        } else if (shape->getClass() == "Polyline") {
            Plyline* polyline = dynamic_cast< Plyline* >(shape);
            drawPolyline(graphics, polyline);
        } else if (shape->getClass() == "Text") {
            Text* text = dynamic_cast< Text* >(shape);
            drawText(graphics, text);
        } else if (shape->getClass() == "Rect") {
            Rect* rectangle = dynamic_cast< Rect* >(shape);
            drawRectangle(graphics, rectangle);
        } else if (shape->getClass() == "Circle") {
            Circle* circle = dynamic_cast< Circle* >(shape);
            drawCircle(graphics, circle);
        } else if (shape->getClass() == "Ellipse") {
            Ell* ellipse = dynamic_cast< Ell* >(shape);
            drawEllipse(graphics, ellipse);
        } else if (shape->getClass() == "Line") {
            Line* line = dynamic_cast< Line* >(shape);
            drawLine(graphics, line);
        } else if (shape->getClass() == "Polygon") {
            Plygon* polygon = dynamic_cast< Plygon* >(shape);
            drawPolygon(graphics, polygon);
        } else if (shape->getClass() == "Path") {
            Path* path = dynamic_cast< Path* >(shape);
            drawPath(graphics, path);
        }
        graphics.SetTransform(&original);
    }
}

void Renderer::drawLine(Gdiplus::Graphics& graphics, Line* line) const {
    mColor color = line->getOutlineColor();
    Gdiplus::Pen linePen(Gdiplus::Color(color.a, color.r, color.g, color.b),
                         line->getOutlineThickness());
    Gdiplus::PointF startPoint(line->getPosition().x, line->getPosition().y);
    Gdiplus::PointF endPoint(line->getDirection().x, line->getDirection().y);
    graphics.DrawLine(&linePen, startPoint, endPoint);
}

void Renderer::drawRectangle(Gdiplus::Graphics& graphics,
                             Rect* rectangle) const {
    float x = rectangle->getPosition().x;
    float y = rectangle->getPosition().y;
    float width = rectangle->getWidth();
    float height = rectangle->getHeight();
    mColor outline_color = rectangle->getOutlineColor();
    Gdiplus::Pen rect_outline(Gdiplus::Color(outline_color.a, outline_color.r,
                                             outline_color.g, outline_color.b),
                              rectangle->getOutlineThickness());
    Gdiplus::Brush* rect_fill = getBrush(rectangle);
    if (rectangle->getRadius().x != 0 || rectangle->getRadius().y != 0) {
        float dx = rectangle->getRadius().x * 2;
        float dy = rectangle->getRadius().y * 2;
        Gdiplus::GraphicsPath path;
        path.AddArc(x, y, dx, dy, 180, 90);
        path.AddArc(x + width - dx, y, dx, dy, 270, 90);
        path.AddArc(x + width - dx, y + height - dy, dx, dy, 0, 90);
        path.AddArc(x, y + height - dy, dx, dy, 90, 90);
        path.CloseFigure();
        graphics.FillPath(rect_fill, &path);
        graphics.DrawPath(&rect_outline, &path);
    } else {
        graphics.FillRectangle(rect_fill, x, y, width, height);
        graphics.DrawRectangle(&rect_outline, x, y, width, height);
    }
    delete rect_fill;
}

void Renderer::drawCircle(Gdiplus::Graphics& graphics, Circle* circle) const {
    mColor outline_color = circle->getOutlineColor();
    Gdiplus::Pen circle_outline(
        Gdiplus::Color(outline_color.a, outline_color.r, outline_color.g,
                       outline_color.b),
        circle->getOutlineThickness());
    Gdiplus::Brush* circle_fill = getBrush(circle);
    graphics.FillEllipse(circle_fill,
                         circle->getPosition().x - circle->getRadius().x,
                         circle->getPosition().y - circle->getRadius().y,
                         circle->getRadius().x * 2, circle->getRadius().y * 2);
    graphics.DrawEllipse(&circle_outline,
                         circle->getPosition().x - circle->getRadius().x,
                         circle->getPosition().y - circle->getRadius().y,
                         circle->getRadius().x * 2, circle->getRadius().x * 2);
    delete circle_fill;
}

void Renderer::drawEllipse(Gdiplus::Graphics& graphics, Ell* ellipse) const {
    mColor outline_color = ellipse->getOutlineColor();
    Gdiplus::Pen ellipse_outline(
        Gdiplus::Color(outline_color.a, outline_color.r, outline_color.g,
                       outline_color.b),
        ellipse->getOutlineThickness());
    Gdiplus::Brush* ellipse_fill = getBrush(ellipse);
    graphics.FillEllipse(
        ellipse_fill, ellipse->getPosition().x - ellipse->getRadius().x,
        ellipse->getPosition().y - ellipse->getRadius().y,
        ellipse->getRadius().x * 2, ellipse->getRadius().y * 2);
    graphics.DrawEllipse(
        &ellipse_outline, ellipse->getPosition().x - ellipse->getRadius().x,
        ellipse->getPosition().y - ellipse->getRadius().y,
        ellipse->getRadius().x * 2, ellipse->getRadius().y * 2);
    delete ellipse_fill;
}

void Renderer::drawPolygon(Gdiplus::Graphics& graphics, Plygon* polygon) const {
    mColor outline_color = polygon->getOutlineColor();
    Gdiplus::Pen polygon_outline(
        Gdiplus::Color(outline_color.a, outline_color.r, outline_color.g,
                       outline_color.b),
        polygon->getOutlineThickness());
    Gdiplus::Brush* polygon_fill = getBrush(polygon);

    Gdiplus::PointF* points = new Gdiplus::PointF[polygon->getPoints().size()];
    int idx = 0;
    const std::vector< Vector2Df >& vertices = polygon->getPoints();
    for (const Vector2Df vertex : vertices) {
        points[idx++] = Gdiplus::PointF(vertex.x, vertex.y);
    }

    Gdiplus::FillMode fill_mode;
    if (polygon->getFillRule() == "evenodd") {
        fill_mode = Gdiplus::FillModeAlternate;
    } else if (polygon->getFillRule() == "nonzero") {
        fill_mode = Gdiplus::FillModeWinding;
    }
    graphics.FillPolygon(polygon_fill, points, idx, fill_mode);
    graphics.DrawPolygon(&polygon_outline, points, idx);
    delete[] points;
    delete polygon_fill;
}

#include <codecvt>
#include <locale>
void Renderer::drawText(Gdiplus::Graphics& graphics, Text* text) const {
    mColor outline_color = text->getOutlineColor();
    graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);
    Gdiplus::Brush* text_fill = getBrush(text);

    Gdiplus::Pen text_outline(Gdiplus::Color(outline_color.a, outline_color.r,
                                             outline_color.g, outline_color.b),
                              text->getOutlineThickness());

    Gdiplus::FontFamily font_family(L"Times New Roman");

    Gdiplus::PointF position(text->getPosition().x, text->getPosition().y);
    Gdiplus::GraphicsPath path;

    std::wstring_convert< std::codecvt_utf8_utf16< wchar_t > > converter;
    std::wstring wide_content = converter.from_bytes(text->getContent());
    Gdiplus::StringFormat string_format;
    if (text->getAnchor() == "middle") {
        string_format.SetAlignment(Gdiplus::StringAlignmentCenter);
        position.X += 7;
    } else if (text->getAnchor() == "end") {
        string_format.SetAlignment(Gdiplus::StringAlignmentFar);
        position.X += 14;
    } else {
        string_format.SetAlignment(Gdiplus::StringAlignmentNear);
    }
    Gdiplus::FontStyle font_style = Gdiplus::FontStyleRegular;
    if (text->getFontStyle() == "italic" || text->getFontStyle() == "oblique") {
        font_style = Gdiplus::FontStyleItalic;
        position.Y -= 1;
    }

    path.AddString(wide_content.c_str(), wide_content.size(), &font_family,
                   font_style, text->getFontSize(), position, &string_format);
    graphics.FillPath(text_fill, &path);
    graphics.DrawPath(&text_outline, &path);
    delete text_fill;
}

void Renderer::drawPolyline(Gdiplus::Graphics& graphics,
                            Plyline* polyline) const {
    mColor outline_color = polyline->getOutlineColor();
    Gdiplus::Pen polyline_outline(
        Gdiplus::Color(outline_color.a, outline_color.r, outline_color.g,
                       outline_color.b),
        polyline->getOutlineThickness());
    Gdiplus::Brush* polyline_fill = getBrush(polyline);

    Gdiplus::FillMode fill_mode;
    if (polyline->getFillRule() == "evenodd") {
        fill_mode = Gdiplus::FillModeAlternate;
    } else if (polyline->getFillRule() == "nonzero") {
        fill_mode = Gdiplus::FillModeWinding;
    }
    Gdiplus::GraphicsPath path(fill_mode);
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
    graphics.FillPath(polyline_fill, &path);
    graphics.DrawPath(&polyline_outline, &path);
    delete polyline_fill;
}

void Renderer::drawPath(Gdiplus::Graphics& graphics, Path* path) const {
    mColor outline_color = path->getOutlineColor();
    Gdiplus::Pen path_outline(Gdiplus::Color(outline_color.a, outline_color.r,
                                             outline_color.g, outline_color.b),
                              path->getOutlineThickness());
    Gdiplus::Brush* path_fill = getBrush(path);

    Gdiplus::FillMode fill_mode;
    if (path->getFillRule() == "evenodd") {
        fill_mode = Gdiplus::FillModeAlternate;
    } else if (path->getFillRule() == "nonzero") {
        fill_mode = Gdiplus::FillModeWinding;
    }
    Gdiplus::GraphicsPath gdi_path(fill_mode);

    const std::vector< PathPoint >& points = path->getPoints();
    int n = points.size();
    Vector2Df first_point{0, 0}, cur_point{0, 0};

    for (int i = 0; i < n; ++i) {
        if (points[i].tc == 'm') {
            first_point = points[i].point;
            gdi_path.StartFigure();
            cur_point = first_point;
        } else if (points[i].tc == 'l' || points[i].tc == 'h' ||
                   points[i].tc == 'v') {
            gdi_path.AddLine(cur_point.x, cur_point.y, points[i].point.x,
                             points[i].point.y);
            cur_point = points[i].point;
        } else if (points[i].tc == 'c') {
            if (i + 2 < n) {
                Vector2Df control_point1 = points[i].point;
                Vector2Df control_point2 = points[i + 1].point;
                Vector2Df control_point3 = points[i + 2].point;
                gdi_path.AddBezier(cur_point.x, cur_point.y, control_point1.x,
                                   control_point1.y, control_point2.x,
                                   control_point2.y, control_point3.x,
                                   control_point3.y);
                i += 2;
                cur_point = control_point3;
            }
        } else if (points[i].tc == 'Z' || points[i].tc == 'z') {
            gdi_path.CloseFigure();
            cur_point = first_point;
        } else if (points[i].tc == 's') {
            if (i + 1 < n) {
                Vector2Df auto_control_point;
                if (i > 0 &&
                    (points[i - 1].tc == 'c' || points[i - 1].tc == 's')) {
                    auto_control_point.x =
                        cur_point.x * 2 - points[i - 2].point.x;
                    auto_control_point.y =
                        cur_point.y * 2 - points[i - 2].point.y;
                } else {
                    auto_control_point = cur_point;
                }
                Vector2Df control_point2 = points[i].point;
                Vector2Df control_point3 = points[i + 1].point;
                gdi_path.AddBezier(cur_point.x, cur_point.y,
                                   auto_control_point.x, auto_control_point.y,
                                   control_point2.x, control_point2.y,
                                   control_point3.x, control_point3.y);
                i += 1;
                cur_point = points[i + 1].point;
            }
        } else if (points[i].tc == 'q') {
            if (i + 1 < n) {
                Vector2Df control_point1{
                    static_cast< float >(cur_point.x +
                                         2.0 / 3.0 *
                                             (points[i].point.x - cur_point.x)),
                    static_cast< float >(
                        cur_point.y +
                        2.0 / 3.0 * (points[i].point.y - cur_point.y))};
                Vector2Df control_point2{
                    static_cast< float >(
                        points[i + 1].point.x +
                        2.0 / 3.0 *
                            (points[i].point.x - points[i + 1].point.x)),
                    static_cast< float >(
                        points[i + 1].point.y +
                        2.0 / 3.0 *
                            (points[i].point.y - points[i + 1].point.y))};
                Vector2Df endPoint{points[i + 1].point.x,
                                   points[i + 1].point.y};

                gdi_path.AddBezier(cur_point.x, cur_point.y, control_point1.x,
                                   control_point1.y, control_point2.x,
                                   control_point2.y, endPoint.x, endPoint.y);

                cur_point = endPoint;
                i += 1;
            }
        } else if (points[i].tc == 't') {
            Vector2Df auto_control_point;
            if (i > 0 && (points[i - 1].tc == 'q' || points[i - 1].tc == 't')) {
                auto_control_point.x = cur_point.x * 2 - points[i - 2].point.x;
                auto_control_point.y = cur_point.y * 2 - points[i - 2].point.y;
            } else {
                auto_control_point = cur_point;
            }
            Vector2Df control_point1{
                static_cast< float >(cur_point.x +
                                     2.0 / 3.0 *
                                         (auto_control_point.x - cur_point.x)),
                static_cast< float >(cur_point.y +
                                     2.0 / 3.0 *
                                         (auto_control_point.y - cur_point.y))};
            Vector2Df control_point2{
                static_cast< float >(
                    points[i].point.x +
                    2.0 / 3.0 * (auto_control_point.x - points[i].point.x)),
                static_cast< float >(
                    points[i].point.y +
                    2.0 / 3.0 * (auto_control_point.y - points[i].point.y))};
            Vector2Df endPoint{points[i].point.x, points[i].point.y};
            gdi_path.AddBezier(cur_point.x, cur_point.y, control_point1.x,
                               control_point1.y, control_point2.x,
                               control_point2.y, endPoint.x, endPoint.y);
            cur_point = endPoint;
        }
    }
    graphics.FillPath(path_fill, &gdi_path);
    graphics.DrawPath(&path_outline, &gdi_path);
    delete path_fill;
}

Gdiplus::Brush* Renderer::getBrush(SVGElement* shape) const {
    Gradient* gradient = shape->getGradient();
    if (gradient != NULL) {
        if (gradient->getClass() == "LinearGradient") {
            std::pair< Vector2Df, Vector2Df > points = gradient->getPoints();
            std::vector< Stop > stops = gradient->getStops();
            int stop_size = stops.size() + 2;
            Gdiplus::Color* colors = new Gdiplus::Color[stop_size];
            float* offsets = new float[stop_size];
            offsets[0] = 0;
            offsets[stop_size - 1] = 1;
            colors[0] =
                Gdiplus::Color(stops[0].getColor().a, stops[0].getColor().r,
                               stops[0].getColor().g, stops[0].getColor().b);
            colors[stop_size - 1] =
                Gdiplus::Color(stops[stop_size - 3].getColor().a,
                               stops[stop_size - 3].getColor().r,
                               stops[stop_size - 3].getColor().g,
                               stops[stop_size - 3].getColor().b);
            for (size_t i = 1; i < stop_size - 1; ++i) {
                colors[i] = Gdiplus::Color(
                    stops[i - 1].getColor().a, stops[i - 1].getColor().r,
                    stops[i - 1].getColor().g, stops[i - 1].getColor().b);
                offsets[i] = stops[i - 1].getOffset();
            }
            Gdiplus::LinearGradientBrush* fill =
                new Gdiplus::LinearGradientBrush(
                    Gdiplus::PointF(points.first.x, points.first.y),
                    Gdiplus::PointF(points.second.x, points.second.y),
                    colors[0], colors[stop_size - 1]);
            fill->SetInterpolationColors(colors, offsets, stop_size);

            // nếu hình nằm ở một vùng của gradient thì ta chỉ thấy 1 phần
            // chuyển màu của vùng đó -> cần phải sửa lại

            applyTransformsOnBrush(gradient->getTransforms(), fill);
            delete[] colors;
            delete[] offsets;
            return fill;
        } else if (gradient->getClass() == "RadialGradient") {
            std::pair< Vector2Df, Vector2Df > points = gradient->getPoints();
            std::vector< Stop > stops = gradient->getStops();
            Gdiplus::GraphicsPath path;
            path.AddEllipse(points.first.x, points.first.y,
                            points.second.x - points.first.x,
                            points.second.y - points.first.y);
            Gdiplus::PathGradientBrush* fill =
                new Gdiplus::PathGradientBrush(&path);
            Gdiplus::Color* colors = new Gdiplus::Color[stops.size()];
            float* offsets = new float[stops.size()];
            for (size_t i = 0; i < stops.size(); ++i) {
                mColor color = stops[i].getColor();
                colors[i] = Gdiplus::Color(color.a, color.r, color.g, color.b);
                offsets[i] = stops[i].getOffset();
            }
            fill->SetInterpolationColors(colors, offsets, stops.size());
            delete[] colors;
            delete[] offsets;
            return fill;
        }
    } else {
        mColor color = shape->getFillColor();
        Gdiplus::SolidBrush* fill = new Gdiplus::SolidBrush(
            Gdiplus::Color(color.a, color.r, color.g, color.b));
        return fill;
    }
    return NULL;
}

void Renderer::applyTransformsOnBrush(
    std::vector< std::string > transform_order,
    Gdiplus::LinearGradientBrush*& brush) const {
    for (auto type : transform_order) {
        if (type.find("translate") != std::string::npos) {
            float trans_x = getTranslate(type).first,
                  trans_y = getTranslate(type).second;
            brush->TranslateTransform(trans_x, trans_y);
        } else if (type.find("rotate") != std::string::npos) {
            float degree = getRotate(type);
            brush->RotateTranform(degree);
        } else if (type.find("scale") != std::string::npos) {
            if (type.find(",") != std::string::npos) {
                float scale_x = getScaleXY(type).first,
                      scale_y = getScaleXY(type).second;
                brush->ScaleTransform(scale_x, scale_y);
            } else {
                float scale = getScale(type);
                brush->ScaleTransform(scale, scale);
            }
        }
    }
}