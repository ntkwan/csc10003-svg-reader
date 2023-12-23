#include "Renderer.hpp"

#include <algorithm>

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
    Gdiplus::RectF bound(x, y, width, height);
    Gdiplus::Brush* rect_fill = getBrush(rectangle, bound);
    if (rectangle->getRadius().x != 0 || rectangle->getRadius().y != 0) {
        float dx = rectangle->getRadius().x * 2;
        float dy = rectangle->getRadius().y * 2;
        Gdiplus::GraphicsPath path;
        path.AddArc(x, y, dx, dy, 180, 90);
        path.AddArc(x + width - dx, y, dx, dy, 270, 90);
        path.AddArc(x + width - dx, y + height - dy, dx, dy, 0, 90);
        path.AddArc(x, y + height - dy, dx, dy, 90, 90);
        path.CloseFigure();
        if (Gdiplus::PathGradientBrush* brush =
                dynamic_cast< Gdiplus::PathGradientBrush* >(rect_fill)) {
            mColor color =
                rectangle->getGradient()->getStops().back().getColor();
            Gdiplus::SolidBrush corner_fill(
                Gdiplus::Color(color.a, color.r, color.g, color.b));
            graphics.FillPath(&corner_fill, &path);
        }
        graphics.FillPath(rect_fill, &path);
        graphics.DrawPath(&rect_outline, &path);
    } else {
        if (Gdiplus::PathGradientBrush* brush =
                dynamic_cast< Gdiplus::PathGradientBrush* >(rect_fill)) {
            mColor color =
                rectangle->getGradient()->getStops().back().getColor();
            Gdiplus::SolidBrush corner_fill(
                Gdiplus::Color(color.a, color.r, color.g, color.b));
            graphics.FillRectangle(&corner_fill, x, y, width, height);
        }
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
    Vector2Df min_bound = circle->getMinBound();
    Vector2Df max_bound = circle->getMaxBound();
    Gdiplus::RectF bound(min_bound.x, min_bound.y, max_bound.x - min_bound.x,
                         max_bound.y - min_bound.y);
    Gdiplus::Brush* circle_fill = getBrush(circle, bound);
    if (Gdiplus::PathGradientBrush* brush =
            dynamic_cast< Gdiplus::PathGradientBrush* >(circle_fill)) {
        mColor color = circle->getGradient()->getStops().back().getColor();
        Gdiplus::SolidBrush corner_fill(
            Gdiplus::Color(color.a, color.r, color.g, color.b));
        graphics.FillEllipse(
            &corner_fill, circle->getPosition().x - circle->getRadius().x,
            circle->getPosition().y - circle->getRadius().y,
            circle->getRadius().x * 2, circle->getRadius().y * 2);
    }
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
    Vector2Df min_bound = ellipse->getMinBound();
    Vector2Df max_bound = ellipse->getMaxBound();
    Gdiplus::RectF bound(min_bound.x, min_bound.y, max_bound.x - min_bound.x,
                         max_bound.y - min_bound.y);
    Gdiplus::Brush* ellipse_fill = getBrush(ellipse, bound);
    if (Gdiplus::PathGradientBrush* brush =
            dynamic_cast< Gdiplus::PathGradientBrush* >(ellipse_fill)) {
        mColor color = ellipse->getGradient()->getStops().back().getColor();
        Gdiplus::SolidBrush corner_fill(
            Gdiplus::Color(color.a, color.r, color.g, color.b));
        graphics.FillEllipse(
            &corner_fill, ellipse->getPosition().x - ellipse->getRadius().x,
            ellipse->getPosition().y - ellipse->getRadius().y,
            ellipse->getRadius().x * 2, ellipse->getRadius().y * 2);
    }
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
    Vector2Df min_bound = polygon->getMinBound();
    Vector2Df max_bound = polygon->getMaxBound();
    Gdiplus::RectF bound(min_bound.x, min_bound.y, max_bound.x - min_bound.x,
                         max_bound.y - min_bound.y);
    Gdiplus::Brush* polygon_fill = getBrush(polygon, bound);
    if (Gdiplus::PathGradientBrush* brush =
            dynamic_cast< Gdiplus::PathGradientBrush* >(polygon_fill)) {
        mColor color = polygon->getGradient()->getStops().back().getColor();
        Gdiplus::SolidBrush corner_fill(
            Gdiplus::Color(color.a, color.r, color.g, color.b));
        graphics.FillPolygon(&corner_fill, points, idx, fill_mode);
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
    Gdiplus::RectF bound;
    path.GetBounds(&bound);
    Gdiplus::Brush* text_fill = getBrush(text, bound);
    if (Gdiplus::PathGradientBrush* brush =
            dynamic_cast< Gdiplus::PathGradientBrush* >(text_fill)) {
        mColor color = text->getGradient()->getStops().back().getColor();
        Gdiplus::SolidBrush corner_fill(
            Gdiplus::Color(color.a, color.r, color.g, color.b));
        graphics.FillPath(&corner_fill, &path);
    }
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
    Vector2Df min_bound = polyline->getMinBound();
    Vector2Df max_bound = polyline->getMaxBound();
    Gdiplus::RectF bound(min_bound.x, min_bound.y, max_bound.x - min_bound.x,
                         max_bound.y - min_bound.y);
    Gdiplus::Brush* polyline_fill = getBrush(polyline, bound);
    // if (Gdiplus::PathGradientBrush* brush =
    //         dynamic_cast< Gdiplus::PathGradientBrush* >(polyline_fill)) {
    //     mColor color = polyline->getGradient()->getStops().back().getColor();
    //     Gdiplus::SolidBrush corner_fill(
    //         Gdiplus::Color(color.a, color.r, color.g, color.b));
    //     graphics.FillPath(&corner_fill, &path);
    // }
    graphics.FillPath(polyline_fill, &path);
    graphics.DrawPath(&polyline_outline, &path);
    delete polyline_fill;
}

#define M_PI 3.14159265358979323846
void Renderer::drawPath(Gdiplus::Graphics& graphics, Path* path) const {
    mColor outline_color = path->getOutlineColor();
    Gdiplus::Pen path_outline(Gdiplus::Color(outline_color.a, outline_color.r,
                                             outline_color.g, outline_color.b),
                              path->getOutlineThickness());

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
        } else if (points[i].tc == 'z') {
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
                cur_point = control_point3;
            }
        } else if (points[i].tc == 'q') {
            if (i + 1 < n) {
                Vector2Df control_point = points[i].point;
                Vector2Df end_point = points[i + 1].point;

                Gdiplus::PointF q_points[3];
                q_points[0] = Gdiplus::PointF{cur_point.x, cur_point.y};
                q_points[1] = Gdiplus::PointF{control_point.x, control_point.y};
                q_points[2] = Gdiplus::PointF{end_point.x, end_point.y};
                gdi_path.AddCurve(q_points, 3);
                cur_point = points[i + 1].point;
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
            Vector2Df end_point = points[i].point;
            Gdiplus::PointF t_points[3];
            t_points[0] = Gdiplus::PointF{cur_point.x, cur_point.y};
            t_points[1] =
                Gdiplus::PointF{auto_control_point.x, auto_control_point.y};
            t_points[2] = Gdiplus::PointF{end_point.x, end_point.y};
            gdi_path.AddCurve(t_points, 3);
            cur_point = points[i].point;
        } else if (points[i].tc == 'a') {
            float rx = points[i].radius.x;
            float ry = points[i].radius.y;
            float x_axis_rotation = points[i].x_axis_rotation;
            bool large_arc_flag = points[i].large_arc_flag;
            bool sweep_flag = points[i].sweep_flag;
            Vector2Df end_point{points[i].point.x, points[i].point.y};

            float angle = x_axis_rotation * static_cast< float >(M_PI) / 180.0;
            float cosAngle = cos(angle);
            float sinAngle = sin(angle);

            Vector2Df point1;
            float constant = cosAngle * cosAngle - (sinAngle * -sinAngle);
            point1.x = constant * (cur_point.x - end_point.x) / 2.0;
            point1.y = constant * (cur_point.y - end_point.y) / 2.0;

            float radii_check = (point1.x * point1.x) / (rx * rx) +
                                (point1.y * point1.y) / (ry * ry);
            if (radii_check > 1.0) {
                rx = std::sqrt(radii_check) * rx;
                ry = std::sqrt(radii_check) * ry;
            }

            int sign = (large_arc_flag == sweep_flag ? -1 : 1);
            Vector2Df point2;
            float numo = (rx * rx) * (ry * ry) -
                         (rx * rx) * (point1.y * point1.y) -
                         (ry * ry) * (point1.x * point1.x);
            float deno = (rx * rx) * (point1.y * point1.y) +
                         (ry * ry) * (point1.x * point1.x);

            if (numo < 0) {
                numo = std::abs(numo);
            }

            point2.x = sign * std::sqrt(numo / deno) * (rx * point1.y / ry);
            point2.y = sign * std::sqrt(numo / deno) * (-ry * point1.x / rx);

            Vector2Df center;
            constant = cosAngle * cosAngle - (sinAngle * -sinAngle);
            center.x = constant * point2.x + (cur_point.x + end_point.x) / 2.0;
            center.y = constant * point2.y + (cur_point.y + end_point.y) / 2.0;

            float startAngle =
                atan2((point1.y - point2.y) / ry, (point1.x - point2.x) / rx);
            float endAngle =
                atan2((-point1.y - point2.y) / ry, (-point1.x - point2.x) / rx);

            float deltaAngle = endAngle - startAngle;

            if (sweep_flag && deltaAngle < 0) {
                deltaAngle += 2.0 * M_PI;
            } else if (!sweep_flag && deltaAngle > 0) {
                deltaAngle -= 2.0 * M_PI;
            }

            gdi_path.AddArc(
                center.x - rx, center.y - ry, 2.0 * rx, 2.0 * ry,
                fmod((long double)(startAngle * 180.0) / M_PI, 360),
                fmod((long double)(deltaAngle * 180.0) / M_PI, 360));

            cur_point = end_point;
        }
    }

    Gdiplus::RectF bound;
    gdi_path.GetBounds(&bound);
    Gdiplus::Brush* path_fill = getBrush(path, bound);
    if (Gdiplus::PathGradientBrush* brush =
            dynamic_cast< Gdiplus::PathGradientBrush* >(path_fill)) {
        mColor color = path->getGradient()->getStops().back().getColor();
        Gdiplus::SolidBrush corner_fill(
            Gdiplus::Color(color.a, color.r, color.g, color.b));
        graphics.FillPath(&corner_fill, &gdi_path);
    }
    graphics.FillPath(path_fill, &gdi_path);
    graphics.DrawPath(&path_outline, &gdi_path);
    delete path_fill;
}

Gdiplus::Brush* Renderer::getBrush(SVGElement* shape,
                                   Gdiplus::RectF bound) const {
    Gradient* gradient = shape->getGradient();
    if (gradient != NULL) {
        std::pair< Vector2Df, Vector2Df > points = gradient->getPoints();
        std::vector< Stop > stops = gradient->getStops();
        int stop_size = stops.size() + 2;
        Gdiplus::Color* colors = new Gdiplus::Color[stop_size];
        float* offsets = new float[stop_size];
        if (gradient->getClass() == "LinearGradient") {
            if (gradient->getUnits() == "userSpaceOnUse") {
                bound.X = points.first.x;
                bound.Y = points.first.y;
                bound.Width = points.second.x;
                bound.Height = points.second.y;
            }
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
                    bound, colors[0], colors[stop_size - 1],
                    Gdiplus::LinearGradientMode::LinearGradientModeHorizontal);
            fill->SetInterpolationColors(colors, offsets, stop_size);
            fill->SetWrapMode(Gdiplus::WrapModeTileFlipXY);
            applyTransformsOnBrush(gradient->getTransforms(), fill);
            delete[] colors;
            delete[] offsets;
            return fill;
        } else if (gradient->getClass() == "RadialGradient") {
            RadialGradient* radial_gradient =
                dynamic_cast< RadialGradient* >(gradient);
            Vector2Df radius = radial_gradient->getRadius();
            if (gradient->getUnits() == "userSpaceOnUse") {
                bound.X = points.first.x - radius.x;
                bound.Y = points.first.y - radius.x;
                bound.Width = radius.x * 2;
                bound.Height = radius.x * 2;
                std::cout << bound.X << " " << bound.Y << " " << bound.Width
                          << " " << bound.Height << std::endl;
            }
            Gdiplus::GraphicsPath path;
            path.AddEllipse(bound);
            Gdiplus::PathGradientBrush* fill =
                new Gdiplus::PathGradientBrush(&path);
            offsets[0] = 0;
            offsets[stop_size - 1] = 1;
            colors[0] = Gdiplus::Color(stops[stop_size - 3].getColor().a,
                                       stops[stop_size - 3].getColor().r,
                                       stops[stop_size - 3].getColor().g,
                                       stops[stop_size - 3].getColor().b);
            colors[stop_size - 1] =
                Gdiplus::Color(stops[0].getColor().a, stops[0].getColor().r,
                               stops[0].getColor().g, stops[0].getColor().b);

            for (size_t i = 1; i < stop_size - 1; ++i) {
                colors[i] =
                    Gdiplus::Color(stops[stop_size - 2 - i].getColor().a,
                                   stops[stop_size - 2 - i].getColor().r,
                                   stops[stop_size - 2 - i].getColor().g,
                                   stops[stop_size - 2 - i].getColor().b);
                offsets[i] = stops[i - 1].getOffset();
                std::cout << offsets[i] << " "
                          << stops[stop_size - 2 - i].getColor().a << " "
                          << stops[stop_size - 2 - i].getColor().r << " "
                          << stops[stop_size - 2 - i].getColor().g << " "
                          << stops[stop_size - 2 - i].getColor().b << std::endl;
            }
            fill->SetInterpolationColors(colors, offsets, stop_size);
            applyTransformsOnBrush(gradient->getTransforms(), fill);
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

void Renderer::applyTransformsOnBrush(
    std::vector< std::string > transform_order,
    Gdiplus::PathGradientBrush*& brush) const {
    for (auto type : transform_order) {
        if (type.find("translate") != std::string::npos) {
            float trans_x = getTranslate(type).first,
                  trans_y = getTranslate(type).second;
            brush->TranslateTransform(trans_x, trans_y);
        } else if (type.find("rotate") != std::string::npos) {
            float degree = getRotate(type);
            brush->RotateTransform(degree);
        } else if (type.find("scale") != std::string::npos) {
            if (type.find(",") != std::string::npos) {
                float scale_x = getScaleXY(type).first,
                      scale_y = getScaleXY(type).second;
                brush->ScaleTransform(scale_x, scale_y);
            } else {
                float scale = getScale(type);
                brush->ScaleTransform(scale, scale);
            }
        } else if (type.find("matrix") != std::string::npos) {
            float a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;
            if (type.find(",") != std::string::npos) {
                type.erase(std::remove(type.begin(), type.end(), ','),
                           type.end());
            }
            sscanf(type.c_str(), "matrix(%f %f %f %f %f %f)", &a, &b, &c, &d,
                   &e, &f);
            Gdiplus::Matrix matrix(a, b, c, d, e, f);
            float m[6] = {0};
            matrix.GetElements(m);
            std::cout << m[0] << " " << m[1] << " " << m[2] << " " << m[3]
                      << " " << m[4] << " " << m[5] << std::endl;
            brush->SetTransform(&matrix);
        }
    }
}