#ifndef RENDERER_HPP_
#define RENDERER_HPP_
// clang-format off
#include <winsock2.h>
#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>

// clang-format on
#include <Graphics.hpp>
/**
 * @brief Singleton class responsible for rendering shapes using GDI+.
 *
 * The Renderer class provides a singleton instance for drawing SVGElement-based
 * shapes using Gdiplus::Graphics. It supports various shapes such as lines,
 * rectangles, circles, ellipses, text, polygons, polylines, and paths. The
 * shapes are drawn in a polymorphic manner using the draw function, which takes
 * a Gdiplus::Graphics context and an SVGElement. The draw function dynamically
 * determines the type of the shape and invokes the corresponding draw method to
 * render the shape with all necessary details. The detailed information for
 * each shape is obtained from an SVG file and processed through the draw
 * function in a polymorphic way.
 */
class Renderer {
public:
    /**
     * @brief Gets the singleton instance of the Renderer class.
     *
     * @return The singleton instance of the Renderer class.
     */
    static Renderer* getInstance();

    /**
     * @brief Deleted copy constructor to enforce the singleton pattern.
     */
    Renderer(const Renderer&) = delete;

    /**
     * @brief Deleted copy assignment operator to enforce the singleton pattern.
     */
    void operator=(const Renderer&) = delete;

    /**
     * @brief Draws a shape using Gdiplus::Graphics based on its type.
     *
     * @param graphics The Gdiplus::Graphics context for drawing.
     * @param shape The SVGElement representing the shape to be drawn.
     */
    void draw(Gdiplus::Graphics& graphics, Group* group) const;

private:
    /**
     * @brief Utility function to apply a series of transformations to the
     * graphics context.
     *
     * @param transform_order The order in which transformations should be
     * applied.
     * @param graphics The Gdiplus::Graphics context to apply transformations
     * to.
     */
    void applyTransform(std::vector< std::string > transform_order,
                        Gdiplus::Graphics& graphics) const;

    /**
     * @brief Draws a line shape using Gdiplus::Graphics.
     *
     * @param graphics The Gdiplus::Graphics context for drawing.
     * @param line The Line object representing the line to be drawn.
     */
    void drawLine(Gdiplus::Graphics& graphics, Line* line) const;

    /**
     * @brief Draws a rectangle shape using Gdiplus::Graphics.
     *
     * @param graphics The Gdiplus::Graphics context for drawing.
     * @param rectangle The Rect object representing the rectangle to be drawn.
     */
    void drawRectangle(Gdiplus::Graphics& graphics, Rect* rectangle) const;

    /**
     * @brief Draws a circle shape using Gdiplus::Graphics.
     *
     * @param graphics The Gdiplus::Graphics context for drawing.
     * @param circle The Circle object representing the circle to be drawn.
     */
    void drawCircle(Gdiplus::Graphics& graphics, Circle* circle) const;

    /**
     * @brief Draws an ellipse shape using Gdiplus::Graphics.
     *
     * @param graphics The Gdiplus::Graphics context for drawing.
     * @param ellipse The Ell object representing the ellipse to be drawn.
     */
    void drawEllipse(Gdiplus::Graphics& graphics, Ell* ellipse) const;

    /**
     * @brief Draws a polygon shape using Gdiplus::Graphics.
     *
     * @param graphics The Gdiplus::Graphics context for drawing.
     * @param polygon The Plygon object representing the polygon to be drawn.
     */
    void drawPolygon(Gdiplus::Graphics& graphics, Plygon* polygon) const;

    /**
     * @brief Draws text using Gdiplus::Graphics.
     *
     * @param graphics The Gdiplus::Graphics context for drawing.
     * @param text The Text object representing the text to be drawn.
     */
    void drawText(Gdiplus::Graphics& graphics, Text* text) const;

    /**
     * @brief Draws a polyline shape using Gdiplus::Graphics.
     *
     * @param graphics The Gdiplus::Graphics context for drawing.
     * @param polyline The Plyline object representing the polyline to be drawn.
     */
    void drawPolyline(Gdiplus::Graphics& graphics, Plyline* polyline) const;

    /**
     * @brief Draws a path shape using Gdiplus::Graphics.
     *
     * @param graphics The Gdiplus::Graphics context for drawing.
     * @param path The Path object representing the path to be drawn.
     */
    void drawPath(Gdiplus::Graphics& graphics, Path* path) const;

    /**
     * @brief Gets the Gdiplus::brush object for the shape fill.
     *
     * @param shape The SVGElement representing the shape.
     * @param bound The bounding box of the shape.
     * @return The Gdiplus::brush object for the shape fill.
     */
    Gdiplus::Brush* getBrush(SVGElement* shape, Gdiplus::RectF bound) const;

    /**
     * @brief Utility function to apply a series of transformations to the brush
     * object.
     *
     * @param transform_order The order in which transformations should be
     * applied.
     * @param brush The Gdiplus::LinearGradientBrush object for the shape fill.
     */
    void applyTransformsOnBrush(std::vector< std::string > transform_order,
                                Gdiplus::LinearGradientBrush*& brush) const;

    /**
     * @brief Utility function to apply a series of transformations to the brush
     * object.
     *
     * @param transform_order The order in which transformations should be
     * applied.
     * @param brush The Gdiplus::PathGradientBrush object for the shape fill.
     *
     */
    void applyTransformsOnBrush(std::vector< std::string > transform_order,
                                Gdiplus::PathGradientBrush*& brush) const;

    /**
     * @brief Private constructor for the Renderer class.
     */
    Renderer();

    static Renderer* instance;  ///< Singleton instance of the Renderer class
};

#endif