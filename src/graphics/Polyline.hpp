#ifndef POLYLINE_HPP_
#define POLYLINE_HPP_

#include "PolyShape.hpp"

/**
 * @brief Represents a polyline in 2D space.
 *
 * The Polyline class is derived from the PolyShape class and defines a polyline
 * with a variable number of vertices.
 */
class Polyline : public PolyShape {
public:
    /**
     * @brief Constructs a Polyline object.
     *
     * @param stroke_width The stroke width of the polyline (default is 0).
     * @param stroke The stroke color of the polyline (default is
     * sf::Color::White).
     * @param fill The fill color of the polyline (default is
     * sf::Color::Transparent).
     */
    Polyline(const Color& fill, const Color& stroke, float stroke_width);

    /**
     * @brief Gets the type of the shape.
     *
     * @return The string "Polyline".
     */
    std::string getClass() const override;
};

#endif  // POLYLINE_HPP_