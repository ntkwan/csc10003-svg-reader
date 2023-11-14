#ifndef POLYGON_HPP_
#define POLYGON_HPP_

#include <SFML/Graphics.hpp>
#include <vector>

#include "Shape.hpp"

/**
 * @brief Represents a polygon in 2D space.
 *
 * The Polygon class is derived from the Shape class and defines a polygon
 * with a variable number of vertices.
 */
class Polygon : public Shape {
private:
    std::vector< sf::Vector2f > points;  ///< Vertices of the polygon

public:
    /**
     * @brief Constructs a Polygon object.
     *
     * @param fill Fill color of the polygon (default is
     * sf::Color::Transparent).
     * @param stroke Outline color of the polygon (default is sf::Color::White).
     * @param stroke_thickness Thickness of the polygon outline (default is 0).
     */
    Polygon(sf::Color fill = sf::Color::Transparent,
            sf::Color stroke = sf::Color::White, float stroke_thickness = 0);

    /**
     * @brief Gets the total number of vertices representing the polygon.
     *
     * @return The number of vertices representing the polygon.
     */
    virtual std::size_t getPointCount() const;

    /**
     * @brief Gets the position of a vertex in the polygon.
     *
     * @param index The index of the vertex.
     * @return The position of the specified vertex in the polygon.
     */
    virtual sf::Vector2f getPoint(std::size_t index) const;

    /**
     * @brief Adds a vertex to the polygon.
     *
     * @param point The position of the vertex to be added.
     */
    void addPoint(const sf::Vector2f& point);

    /**
     * @brief Updates the polygon.
     *
     * This method is provided for consistency with other shapes but
     * does not introduce any additional behavior for polygons.
     */
    void polygonUpdate();
};

#endif