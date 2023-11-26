#ifndef SHAPE_HPP_
#define SHAPE_HPP_

#include <SFML/Graphics.hpp>

#include "Color.hpp"
#include "Vector2D.hpp"

/**
 * @brief Represents a shape in 2D space
 * @note This class is abstract and cannot be instantiated.
 * @note This class is applied Abstract Factory design pattern and used as
 * interface for other shapes.
 */
class Shape {
public:
    /**
     *  @brief Virtual constructor
     */
    virtual ~Shape() = default;

    /**
     * @brief Gets the type of the shape
     *
     * @return The type of the shape
     *
     * @note This function is used for determining the type of the shape.
     * @note This function is pure virtual and must be implemented by derived
     * classes.
     */
    virtual std::string getClass() const = 0;

    /**
     * @brief Sets the fill color of the shape.
     *
     * @param color The new fill color of the shape.
     */
    void setFillColor(const Color& color);

    /**
     * @brief Sets the outline color of the shape.
     *
     * @param color The new outline color of the shape.
     */
    void setOutlineColor(const Color& color);

    /**
     * @brief Sets the outline thickness of the shape.
     * @param thickness The new outline thickness of the shape.
     * @note If the thickness is negative, the outline will be inside the shape.
     * If the thickness is positive, the outline will be outside the shape.
     * If the thickness is zero, no outline will be drawn.
     * @note The default outline thickness is 0.
     * @note The outline thickness cannot be greater than the radius of the
     * shape.
     */
    void setOutlineThickness(float thickness);

    /**
     * @brief Sets the position of the shape
     * @param x The x coordinate of the new position
     * @param y The y coordinate of the new position
     * @note The default position of the shape is (0, 0).
     * @note The position of the shape is relative to its origin.
     */
    void setPosition(float x, float y);

    /**
     * @brief Sets the position of the shape
     * @param position The new position of the shape (Vector2f is a typedef
     * of coordination vector)
     * @note The default position of the shape is (0, 0).
     * @note The position of the shape is relative to its origin.
     */
    void setPosition(const Vector2Df& position);

    /**
     * @brief Gets the fill color of the shape.
     * @return The fill color of the shape.
     * @note The default fill color is white.
     */
    const Color& getFillColor() const;

    /**
     * @brief Gets the outline color of the shape.
     * @return The outline color of the shape.
     * @note The default outline color is white.
     */
    const Color& getOutlineColor() const;

    /**
     * @brief Gets the outline thickness of the shape.
     * @return The outline thickness of the shape.
     * @note The default outline thickness is 0.
     */
    float getOutlineThickness() const;

    /**
     * @brief Get the current position of the shape
     *
     * @return The current position of the shape
     * @note The default position of the shape is (0, 0).
     */
    Vector2Df getPosition() const;

    virtual void printData() const;

    void translate(float x, float y);

    void rotate(float angle);

    void scale(float x, float y);

    void scale(float factor);

protected:
    /**
     * @brief Constructs a Shape object
     * @note This constructor is protected because Shape is an abstract class
     * that cannot be instantiated.
     */
    Shape();

private:
    Color fill;          ///< Fill color
    Color stroke;        ///< Outline color
    float stroke_width;  ///< Thickness of the shape's outline
    Vector2Df position;  ///< Position of the shape
    Vector2Df origin;  ///< Origin of translation/rotation/scaling of the object
    float rotation;    ///< Orientation of the object, in degrees
    Vector2Df scale_;  ///< Scale of the object
};

#endif  // SHAPE_HPP_