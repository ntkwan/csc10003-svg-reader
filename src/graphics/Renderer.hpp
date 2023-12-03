#ifndef RENDERER_HPP_
#define RENDERER_HPP_
// clang-format off
#include <winsock2.h>
#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>

//clang-format on
#include "Graphics.hpp"

class Renderer {
public:
    static Renderer* getInstance(Gdiplus::Graphics& graphics);
    Renderer(const Renderer&) = delete;
    void operator=(const Renderer&) = delete;

    Gdiplus::Graphics& graphics;
    void draw(Shape* shape) const;

private:
    void drawLine(Line* line) const;
    void drawRectangle(Rect* rectangle) const;
    void drawCircle(Circle* circle) const;
    void drawEllipse(Ell* ellipse) const;
    void drawPolygon(Plygon* polygon) const;
    void drawText(Text* text) const;
    void drawPolyline(Plyline* polyline) const;
    void drawPath(Path* path) const;

    Renderer(Gdiplus::Graphics& graphics);
    static Renderer* instance;
};

#endif