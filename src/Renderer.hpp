#ifndef RENDERER_HPP_
#define RENDERER_HPP_
// clang-format off
#include <winsock2.h>
#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>

// clang-format on
#include <Graphics.hpp>

class Renderer {
public:
    static Renderer* getInstance();
    Renderer(const Renderer&) = delete;
    void operator=(const Renderer&) = delete;

    void draw(Gdiplus::Graphics& graphics, Group* group) const;

private:
    void applyTransform(std::vector< std::string > transform_order,
                        Gdiplus::Graphics& graphics) const;
    void drawLine(Gdiplus::Graphics& graphics, Line* line) const;
    void drawRectangle(Gdiplus::Graphics& graphics, Rect* rectangle) const;
    void drawCircle(Gdiplus::Graphics& graphics, Circle* circle) const;
    void drawEllipse(Gdiplus::Graphics& graphics, Ell* ellipse) const;
    void drawPolygon(Gdiplus::Graphics& graphics, Plygon* polygon) const;
    void drawText(Gdiplus::Graphics& graphics, Text* text) const;
    void drawPolyline(Gdiplus::Graphics& graphics, Plyline* polyline) const;
    void drawPath(Gdiplus::Graphics& graphics, Path* path) const;

    Renderer();
    static Renderer* instance;
};

#endif