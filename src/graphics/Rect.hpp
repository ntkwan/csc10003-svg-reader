#ifndef RECT_HPP_
#define RECT_HPP_

#include "SVGElement.hpp"

/**
 * @brief Represents a rectangle in 2D space.
 *
 * The Rect class is derived from the SVGElement class and defines a rectangle
 * with a specified width, height, position, fill color, stroke color, and
 * stroke thickness.
 */
class Rect : public SVGElement {
private:
    float width;       ///< Width of the rectangle
    float height;      ///< Height of the rectangle
    Vector2Df radius;  ///< Radii of the rectangle in the x and y directions

public:
    /**
     * @brief Constructs a Rect object.
     *
     * @param width The width of the rectangle.
     * @param height The height of the rectangle.
     * @param position The position of the rectangle.
     * @param radius The radii of the rectangle in the x and y directions.
     * @param fill Fill color of the rectangle.
     * @param stroke Outline color of the rectangle.
     * @param stroke_width Thickness of the rectangle outline.
     */
    Rect(float width, float height, Vector2Df position, Vector2Df radius,
         const ColorShape& fill, const ColorShape& stroke, float stroke_width);

    /**
     * @brief Gets the type of the shape.
     *
     * @return The string "Rect".
     */
    std::string getClass() const override;

    /**
     * @brief Sets the width of the rectangle.
     *
     * @param width The new width of the rectangle.
     */
    void setWidth(float width);

    /**
     * @brief Gets the width of the rectangle.
     *
     * @return The width of the rectangle.
     */
    float getWidth() const;

    /**
     * @brief Sets the height of the rectangle.
     *
     * @param height The new height of the rectangle.
     */
    void setHeight(float height);

    /**
     * @brief Gets the height of the rectangle.
     *
     * @return The height of the rectangle.
     */
    float getHeight() const;

    /**
     * @brief Sets the radii of the rectangle.
     *
     * @param radius The new radii of the rectangle.
     */
    void setRadius(const Vector2Df& radius);

    /**
     * @brief Gets the radii of the rectangle.
     *
     * @return The radii of the rectangle.
     */
    Vector2Df getRadius() const;

    /**
     * @brief Prints the data of the rectangle.
     *
     * @note This function is used for debugging purposes.
     */
    void printData() const override;
};

#endif  // RECT_HPP_
