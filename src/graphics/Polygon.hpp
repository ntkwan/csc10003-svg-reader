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
};

#endif  // POLYGON_HPP_