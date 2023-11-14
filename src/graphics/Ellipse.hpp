#ifndef ELLIPSE_HPP_
#define ELLIPSE_HPP_

#include "Circle.hpp"

/**
 * @brief Represents an ellipse in 2D space.
 *
 * The Ellipse class is derived from the Circle class and defines an ellipse
 * with a variable radius in the x and y directions.
 */
class Ellipse : public Circle {
private:
    sf::Vector2f radius;  ///< Radii of the ellipse in the x and y directions

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
    Ellipse(const sf::Vector2f &radius, const sf::Vector2f &center,
            sf::Color fill, sf::Color stroke, float stroke_thickness);

    /**
     * @brief Gets the position of a point on the ellipse.
     *
     * @param index The index of the point.
     * @return The position of the specified point on the ellipse.
     */
    virtual sf::Vector2f getPoint(std::size_t index) const override;
};

#endif  // ELLIPSE_HPP_
