#ifndef CIRCLE_HPP_
#define CIRCLE_HPP_

#include "Shape.hpp"

/**
 * @brief Represents a circle in 2D space.
 *
 * The Circle class is derived from the Shape class and defines a circle
 * with a specified radius, center, fill color, stroke color, and stroke
 * thickness.
 */
class Circle : public Shape {
private:
    const int SCALE =
        100000;    ///< Scale factor for determining the number of points
    float radius;  ///< Radius of the circle

public:
    /**
     * @brief Constructs a Circle object.
     *
     * @param radius The radius of the circle.
     * @param center The center of the circle.
     * @param fill Fill color of the circle.
     * @param stroke Outline color of the circle.
     * @param stroke_thickness Thickness of the circle outline.
     */
    Circle(float radius, const sf::Vector2f &center, sf::Color fill,
           sf::Color stroke, float stroke_thickness);

    /**
     * @brief Gets the total number of points representing the circle.
     *
     * In this case, it returns a large number (SCALE) to approximate a smooth
     * circle.
     *
     * @return The number of points representing the circle.
     */
    virtual std::size_t getPointCount() const;

    /**
     * @brief Gets the position of a point on the circle.
     *
     * @param index The index of the point.
     * @return The position of the specified point on the circle.
     */
    virtual sf::Vector2f getPoint(std::size_t index) const;
};

#endif  // CIRCLE_HPP_
