#include "Renderer.hpp"

#include <wx/graphics.h>
#include <wx/wx.h>

Renderer::Renderer(wxGraphicsContext* context) : gc(context) {}

void Renderer::renderCircle(const Circle& circle) const {
    gc->SetPen(
        wxPen(wxColour(circle.getOutlineColor().r, circle.getOutlineColor().g,
                       circle.getOutlineColor().b, circle.getOutlineColor().a),
              circle.getOutlineThickness()));
    gc->SetBrush(
        wxBrush(wxColour(circle.getFillColor().r, circle.getFillColor().g,
                         circle.getFillColor().b, circle.getFillColor().a)));
    gc->DrawEllipse(circle.getPosition().x - circle.getRadius().x,
                    circle.getPosition().y - circle.getRadius().y,
                    circle.getRadius().x * 2, circle.getRadius().y * 2);
}

void Renderer::renderEllipse(const class Ellipse& ellipse) const {
    gc->SetPen(wxPen(
        wxColour(ellipse.getOutlineColor().r, ellipse.getOutlineColor().g,
                 ellipse.getOutlineColor().b, ellipse.getOutlineColor().a),
        ellipse.getOutlineThickness()));
    gc->SetBrush(
        wxBrush(wxColour(ellipse.getFillColor().r, ellipse.getFillColor().g,
                         ellipse.getFillColor().b, ellipse.getFillColor().a)));
    gc->DrawEllipse(ellipse.getPosition().x - ellipse.getRadius().x,
                    ellipse.getPosition().y - ellipse.getRadius().y,
                    ellipse.getRadius().x * 2, ellipse.getRadius().y * 2);
}

void Renderer::renderLine(const Line& line) const {
    gc->SetPen(
        wxPen(wxColour(line.getOutlineColor().r, line.getOutlineColor().g,
                       line.getOutlineColor().b, line.getOutlineColor().a),
              line.getOutlineThickness()));
    gc->StrokeLine(line.getPosition().x, line.getPosition().y,
                   line.getDirection().x, line.getDirection().y);
}

void Renderer::renderPath(const Path& path) const {
    gc->SetPen(
        wxPen(wxColour(path.getOutlineColor().r, path.getOutlineColor().g,
                       path.getOutlineColor().b, path.getOutlineColor().a),
              path.getOutlineThickness()));
    gc->SetBrush(
        wxBrush(wxColour(path.getFillColor().r, path.getFillColor().g,
                         path.getFillColor().b, path.getFillColor().a)));
    wxGraphicsPath wxPath = gc->CreatePath();
    for (auto point : path.getPoints()) {
        wxPath.AddLineToPoint(point.Point.x, point.Point.y);
    }
    wxPath.CloseSubpath();
    gc->DrawPath(wxPath);
}

void Renderer::renderPolygon(const class Polygon& polygon) const {
    gc->SetPen(wxPen(
        wxColour(polygon.getOutlineColor().r, polygon.getOutlineColor().g,
                 polygon.getOutlineColor().b, polygon.getOutlineColor().a),
        polygon.getOutlineThickness()));
    gc->SetBrush(
        wxBrush(wxColour(polygon.getFillColor().r, polygon.getFillColor().g,
                         polygon.getFillColor().b, polygon.getFillColor().a)));
    wxGraphicsPath wxPath = gc->CreatePath();
    for (auto point : polygon.getPoints()) {
        wxPath.AddLineToPoint(point.x, point.y);
    }
    wxPath.CloseSubpath();
    gc->DrawPath(wxPath);
}

void Renderer::renderPolyline(const class Polyline& polyline) const {
    gc->SetPen(wxPen(
        wxColour(polyline.getOutlineColor().r, polyline.getOutlineColor().g,
                 polyline.getOutlineColor().b, polyline.getOutlineColor().a),
        polyline.getOutlineThickness()));
    gc->SetBrush(wxBrush(
        wxColour(polyline.getFillColor().r, polyline.getFillColor().g,
                 polyline.getFillColor().b, polyline.getFillColor().a)));
    wxGraphicsPath wxPath = gc->CreatePath();
    for (auto point : polyline.getPoints()) {
        wxPath.AddLineToPoint(point.x, point.y);
    }
    gc->DrawPath(wxPath);
}

void Renderer::renderRect(const Rect& rect) const {
    gc->SetPen(
        wxPen(wxColour(rect.getOutlineColor().r, rect.getOutlineColor().g,
                       rect.getOutlineColor().b, rect.getOutlineColor().a),
              rect.getOutlineThickness()));
    gc->SetBrush(
        wxBrush(wxColour(rect.getFillColor().r, rect.getFillColor().g,
                         rect.getFillColor().b, rect.getFillColor().a)));
    gc->DrawRectangle(rect.getPosition().x, rect.getPosition().y,
                      rect.getWidth(), rect.getHeight());
}

void Renderer::renderText(const Text& text) const {
    gc->SetPen(
        wxPen(wxColour(text.getOutlineColor().r, text.getOutlineColor().g,
                       text.getOutlineColor().b, text.getOutlineColor().a),
              text.getOutlineThickness()));
    gc->SetBrush(
        wxBrush(wxColour(text.getFillColor().r, text.getFillColor().g,
                         text.getFillColor().b, text.getFillColor().a)));

    wxFont font(text.getFontSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                wxFONTWEIGHT_NORMAL);
    gc->SetFont(font,
                wxColour(text.getOutlineColor().r, text.getOutlineColor().g,
                         text.getOutlineColor().b, text.getOutlineColor().a));

    gc->DrawText(text.getContent(), text.getPosition().x, text.getPosition().y);
}
