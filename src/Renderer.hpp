#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include <SFML/Graphics.hpp>

#include "Graphics.hpp"

class Renderer {
public:
    static Renderer* getInstance(sf::RenderWindow& window);
    Renderer(const Renderer&) = delete;
    void operator=(const Renderer&) = delete;

    sf::RenderWindow& window;
    /**
     * @brief Draw the shape on the specified render target
     *
     */
    void draw(Shape* shape) const;

private:
    void drawLine(Line* line) const;

    void drawRectangle(Rect* rectangle) const;

    void drawCircle(Circle* circle) const;

    void drawEllipse(Ellipse* ellipse) const;

    void drawPolygon(Polygon* polygon) const;

    void drawText(Text* text) const;

    void drawPolyline(Polyline* polyline) const;

    void drawCurve(Curve curve) const;

    void drawPath(Path* path) const;

    Renderer(sf::RenderWindow& window);
    static Renderer* instance;
};

#endif