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
protected:
    const int SCALE =
        100000;        ///< Scale factor for determining the number of points
    Vector2Df radius;  ///< Radii of the ellipse in the x and y directions

public:
    /**
     * @brief Constructs an Ellipse object.
     *
     * @param radius The radii of the ellipse in the x and y directions.
     * @param center The center of the ellipse.
     * @param fill Fill color of the ellipse.
     * @param stroke Outline color of the ellipse.
     * @param stroke_thickness Thickness of the ellipse outline.
     */
    Ellipse(const Vector2Df &radius, const Vector2Df &center, Color fill,
            Color stroke, float stroke_thickness);

    /**
     * @brief Gets the total number of points representing the ellipse.
     *
     * In this case, it returns a large number (SCALE) to approximate a smooth
     * ellipse.
     *
     * @return The number of points representing the ellipse.
     */
    virtual std::size_t getPointCount() const;

    /**
     * @brief Gets the position of a point on the ellipse.
     *
     * @param index The index of the point.
     * @return The position of the specified point on the ellipse.
     */
    virtual Vector2Df getPoint(std::size_t index) const override;
};

#endif  // ELLIPSE_HPP_