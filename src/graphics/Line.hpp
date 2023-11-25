#ifndef LINE_HPP_
#define LINE_HPP_

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

public:
    /**
     * @brief Constructs a Line object.
     *
     * @param point1 The starting point of the line.
     * @param point2 The ending point of the line.
     * @param stroke The color of the line (default is sf::Color::White).
     * @param stroke_width The thickness of the line (default is 1.0).
     */
    Line(const Vector2Df& point1, const Vector2Df& point2, Color stroke,
         float stroke_width);

    /**
     * @brief Gets the type of the shape.
     *
     * @return The string "Line".
     */
    std::string getClass() const override;

    /**
     * @brief Sets the direction of the line.
     *
     * @param direction The new direction of the line.
     */
    void setDirection(const Vector2Df& direction);

    /**
     * @brief Gets the direction of the line.
     *
     * @return The direction of the line.
     */
    Vector2Df getDirection() const;

    /**
     * @brief Gets the length of the line.
     *
     * @return The length of the line.
     */
    float getLength() const;
};

#endif