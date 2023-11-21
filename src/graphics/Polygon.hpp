#ifndef POLYGON_HPP_
#define POLYGON_HPP_

#include <SFML/Graphics.hpp>
#include <vector>

#include "PolyShape.hpp"
#include "Shape.hpp"

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
     * @param stroke_thickness Thickness of the polygon outline (default is 0).
     */
    Polygon(Color fill = Color::Transparent, Color stroke = Color::White,
            float stroke_thickness = 0);

    /**
     * @brief Gets the total number of vertices representing the polygon.
     *
     * @return The number of vertices representing the polygon.
     */
    std::size_t getPointCount() const override;

    /**
     * @brief Gets the position of a vertex in the polygon.
     *
     * @param index The index of the vertex.
     * @return The position of the specified vertex in the polygon.
     */
    Vector2Df getPoint(std::size_t index) const override;

    /**
     * @brief Adds a vertex to the polygon.
     *
     * @param point The position of the vertex to be added.
     */
    void addPoint(const Vector2Df& point) override;

    /**
     * @brief Updates the polygon.
     *
     * This method is provided for consistency with other shapes but
     * does not introduce any additional behavior for polygons.
     */
    void updateShape() override;
};

#endif  // POLYGON_HPP_