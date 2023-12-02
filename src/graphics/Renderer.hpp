#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include <wx/graphics.h>

#include "Graphics.hpp"

class Renderer {
public:
    Renderer(wxGraphicsContext* context);
    void renderLine(const Line& line) const;
    void renderRect(const Rect& rect) const;
    void renderPolyline(const class Polyline& polyline) const;
    void renderText(const Text& text) const;
    void renderCircle(const Circle& circle) const;
    void renderEllipse(const class Ellipse& ellipse) const;
    void renderPath(const Path& path) const;
    void renderPolygon(const class Polygon& polygon) const;

private:
    wxGraphicsContext* gc;
};
#endif  // RENDERER_HPP_