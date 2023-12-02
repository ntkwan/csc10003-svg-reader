#ifndef POLYGON_HPP_
#define POLYGON_HPP_

#include "PolyShape.hpp"

/**
 * @brief Represents a polygon in 2D space.
 *
 * The Polygon class is derived from the PolyShape class and defines a polygon
 * with a variable number of vertices.
 */
class Polygon : public PolyShape {
public:
    /**
     * @brief Constructs a Polygon object.
     *
     * @param fill Fill color of the polygon (default is
     * sf::Color::Transparent).
     * @param stroke Outline color of the polygon (default is sf::Color::White).
     * @param stroke_width Thickness of the polygon outline (default is 0).
     */
    Polygon(Color fill, Color stroke, float stroke_width);

    /**
     * @brief Gets the type of the shape.
     *
     * @return The string "Polygon".

     */
    std::string getClass() const override;

    /**
     * @brief Renders the shape using the given renderer.
     *
     * @param renderer The renderer to be used for rendering the shape.
     */
    void render(Renderer& renderer) const override;
};

#endif  // POLYGON_HPP_