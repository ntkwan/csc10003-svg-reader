#ifndef RECT_HPP_
#define RECT_HPP_

#include "Polygon.hpp"

/**
 * @brief Represents a rectangle in 2D space.
 *
 * The Rect class is derived from the Polygon class and defines a rectangle
 * with a specified width, height, position, fill color, stroke color, and
 * stroke thickness.
 */
class Rect : public Polygon {
private:
    float width;          ///< Width of the rectangle
    float height;         ///< Height of the rectangle
    Vector2Df rect_size;  ///< Size of the rectangle

public:
    /**
     * @brief Constructs a Rect object.
     *
     * @param width The width of the rectangle.
     * @param height The height of the rectangle.
     * @param x The X-coordinate of the position.
     * @param y The Y-coordinate of the position.
     * @param fill Fill color of the rectangle.
     * @param stroke Outline color of the rectangle.
     * @param stroke_thickness Thickness of the rectangle outline.
     */
    Rect(float width, float height, float x, float y, sf::Color fill,
         sf::Color stroke, float stroke_thickness);

    /**
     * @brief Gets the total number of vertices representing the rectangle.
     *
     * In this case, it always returns 4 since a rectangle has four corners.
     *
     * @return The number of vertices representing the rectangle.
     */
    virtual std::size_t getPointCount() const override;

    /**
     * @brief Gets the position of a vertex in the rectangle.
     *
     * @param index The index of the vertex (0 for top-left, 1 for top-right, 2
     * for bottom-right, 3 for bottom-left).
     * @return The position of the specified vertex in the rectangle.
     */
    virtual Vector2Df getPoint(std::size_t index) const override;
};

#endif  // RECT_HPP_
