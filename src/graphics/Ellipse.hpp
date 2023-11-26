#ifndef ELLIPSE_HPP_
#define ELLIPSE_HPP_

#include "Shape.hpp"

/**
 * @brief Represents an ellipse in 2D space.
 *
 * The Ellipse class is derived from the Shape class and defines an ellipse
 * with a variable radius in the x and y directions.
 */
class Ellipse : public Shape {
private:
    Vector2Df radius;  ///< Radii of the ellipse in the x and y directions

public:
    /**
     * @brief Constructs an Ellipse object.
     *
     * @param radius The radii of the ellipse in the x and y directions.
     * @param center The center of the ellipse.
     * @param fill Fill color of the ellipse.
     * @param stroke Outline color of the ellipse.
     * @param stroke_width Thickness of the ellipse outline.
     */
    Ellipse(const Vector2Df &radius, const Vector2Df &center, Color fill,
            Color stroke, float stroke_width);

    /**
     * @brief Gets the type of the shape.
     *
     * @return The string "Ellipse".
     *
     * @note This function is used for determining the type of the shape.
     */
    std::string getClass() const override;

    /**
     * @brief Sets the radius of the ellipse.
     *
     * @param radius The new radius of the ellipse.
     */
    void setRadius(const Vector2Df &radius);

    /**
     * @brief Gets the radius of the ellipse.
     *
     * @return The radius of the ellipse.
     */
    Vector2Df getRadius() const;

    void printData() const override;
};

#endif  // ELLIPSE_HPP_