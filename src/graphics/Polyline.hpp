#ifndef POLYLINE_HPP_
#define POLYLINE_HPP_

#include "PolyShape.hpp"
#include "Shape.hpp"

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
     * @param stroke_Width The stroke width of the polyline (default is 0).
     * @param stroke_color The stroke color of the polyline (default is
     * sf::Color::White).
     * @param fill The fill color of the polyline (default is
     * sf::Color::Transparent).
     */
    Polyline(float stroke_Width = 0, const Color& stroke_color = Color::White,
             const Color& fill = Color::Transparent);

    /**
     * @brief Adds a vertex to the polyline.
     *
     * @param point The position of the vertex to be added.
     */
    void addPoint(const Vector2Df& point) override;

    std::string getClass() const override { return "Polyline"; }
    /**
     * @brief Gets the position of a vertex in the polyline.
     *
     * @param index The index of the vertex.
     * @return The position of the specified vertex in the polyline.
     */
    Vector2Df getPoint(std::size_t index) const override;

    /**
     * @brief Gets the total number of vertices representing the polyline.
     *
     * @return The number of vertices representing the polyline.
     */
    std::size_t getPointCount() const override;

    /**
     * @brief Updates the polyline.
     *
     * This method is provided for consistency with other shapes but
     * does not introduce any additional behavior for polylines.
     */
    void updateShape() override;
};

#endif  // POLYLINE_HPP_