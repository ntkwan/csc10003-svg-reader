#ifndef RECT_HPP_
#define RECT_HPP_

#include "Shape.hpp"

/**
 * @brief Represents a rectangle in 2D space.
 *
 * The Rect class is derived from the Shape class and defines a rectangle
 * with a specified width, height, position, fill color, stroke color, and
 * stroke thickness.
 */
class Rect : public Shape {
private:
    float width;             ///< Width of the rectangle
    float height;            ///< Height of the rectangle
    float x;                 ///< X-coordinate of the rectangle's position
    float y;                 ///< Y-coordinate of the rectangle's position
    sf::Vector2f rect_size;  ///< Size of the rectangle

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
     * @brief Gets the size of the rectangle.
     *
     * @return The size of the rectangle.
     */
    const sf::Vector2f& getSize() const;

    /**
     * @brief Sets the size of the rectangle.
     *
     * @param size The new size of the rectangle.
     */
    void setSize(const sf::Vector2f& size);

    /**
     * @brief Gets the total number of points representing the rectangle.
     *
     * In this case, it always returns 4 since a rectangle has four corners.
     *
     * @return The number of points representing the rectangle.
     */
    virtual std::size_t getPointCount() const;

    /**
     * @brief Gets the position of a corner of the rectangle.
     *
     * @param index The index of the corner (0 for top-left, 1 for top-right, 2
     * for bottom-right, 3 for bottom-left).
     * @return The position of the specified corner of the rectangle.
     */
    virtual sf::Vector2f getPoint(std::size_t index) const;
};

#endif  // RECT_HPP_