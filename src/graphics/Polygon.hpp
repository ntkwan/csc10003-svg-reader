#ifndef POLYGON_HPP_
#define POLYGON_HPP_

#include "PolyShape.hpp"

/**
 * @brief Represents a polygon in 2D space.
 *
 * The Polygon class is derived from the PolyShape class and defines a polygon
 * with a variable number of vertices.
 */
class Plygon : public PolyShape {
public:
    /**
     * @brief Constructs a Polygon object.
     *
     * @param fill Fill color of the polygon (default is
     * sf::Color::Transparent).
     * @param stroke Outline color of the polygon (default is sf::Color::White).
     * @param stroke_width Thickness of the polygon outline (default is 0).
     */
    Plygon(mColor fill, mColor stroke, float stroke_width);

    /**
     * @brief Gets the type of the shape.
     *
     * @return The string "Polygon".

     */
    std::string getClass() const override;

    // /**
    //  * @brief Renders the shape using the given renderer.
    //  *
    //  * @param renderer The renderer to be used for rendering the shape.
    //  */
    // void render(Renderer& renderer) const override;

    /**
     * @brief Sets the fill rule of the polygon.
     *
     * @param fill_rule The new fill rule of the polygon.
     */
    void setFillRule(std::string fill_rule);

    /**
     * @brief Gets the fill rule of the polygon.
     *
     * @return The fill rule of the polygon.
     */
    std::string getFillRule() const;

private:
    std::string fill_rule;  ///< Fill rule of the polygon
};

#endif  // POLYGON_HPP_