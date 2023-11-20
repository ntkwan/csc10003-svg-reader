#ifndef LINE_HPP_
#define LINE_HPP_

#include <cmath>

#include "Shape.hpp"

/**
 * @brief Represents a line in 2D space.
 *
 * The Line class is derived from the Shape class and defines a line segment
 * with a specified direction and thickness.
 */
class Line : public Shape {
private:
    Vector2Df direction;  ///< Direction of the line
    float thickness;      ///< Thickness of the line

public:
    /**
     * @brief Constructs a Line object.
     *
     * @param point1 The starting point of the line.
     * @param point2 The ending point of the line.
     * @param stroke The color of the line (default is sf::Color::White).
     * @param stroke_width The thickness of the line (default is 1.0).
     */
    Line(const Vector2Df& point1, const Vector2Df& point2,
         sf::Color stroke = sf::Color::White, float stroke_width = 1.f);

    /**
     * @brief Sets the thickness of the line.
     *
     * @param thickness The new thickness of the line.
     */
    void setThickness(float thickness);

    /**
     * @brief Calculates and returns the length of the line.
     *
     * @return The length of the line.
     */
    float getLength() const;

    /**
     * @brief Gets the total number of points representing the line.
     *
     * In this case, it always returns 4 since a line is represented by 4 points
     * (start, end, and two additional points for thickness).
     *
     * @return The number of points representing the line.
     */
    virtual std::size_t getPointCount() const;

    /**
     * @brief Gets the position of a point on the line.
     *
     * @param index The index of the point (0 for the starting point, 1 for the
     * end point, 2 for the first additional point, 3 for the second additional
     * point).
     * @return The position of the specified point on the line.
     */
    virtual Vector2Df getPoint(std::size_t index) const;
};

#endif