#ifndef SVG_ELEMENT_HPP_
#define SVG_ELEMENT_HPP_

#include <vector>

#include "Color.hpp"
#include "Gradient.hpp"
#include "Vector2D.hpp"

/**
 * @brief Represents an element in an SVG file.
 * @note This class is abstract and cannot be instantiated.
 * @note This class is applied Abstract Factory design pattern and used as
 * interface for other shapes.
 * @note This class is applied Composite design pattern and used as base class
 * for other shapes.
 */
class SVGElement {
public:
    /**
     *  @brief Virtual constructor
     */
    virtual ~SVGElement() = default;

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
    void setFillColor(const mColor& color);

    /**
     * @brief Sets the outline color of the shape.
     *
     * @param color The new outline color of the shape.
     */
    void setOutlineColor(const mColor& color);

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
    const mColor& getFillColor() const;

    /**
     * @brief Gets the outline color of the shape.
     * @return The outline color of the shape.
     * @note The default outline color is white.
     */
    const mColor& getOutlineColor() const;

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

    /**
     * @brief Gets the minimum bound of the shape.
     *
     * @return The minimum bound of the shape.
     */
    virtual Vector2Df getMinBound() const;

    /**
     * @brief Gets the maximum bound of the shape.
     *
     * @return The maximum bound of the shape.
     */
    virtual Vector2Df getMaxBound() const;

    /**
     * @brief Prints the data of the shape.
     *
     * @note This function is used for debugging purposes.
     * @note This function is virtual and can be overridden by derived classes.
     */
    virtual void printData() const;

    /**
     * @brief Sets the transformations of the shape.
     *
     * @param transforms The new transformations of the shape.
     * @note The default transformations of the shape is empty.
     * @note The transformations can be either "translate", "rotate", "scale",
     */
    void setTransforms(const std::vector< std::string >& transforms);

    /**
     * @brief Gets the transformations of the shape.
     *
     * @return The transformations of the shape.
     * @note The default transformations of the shape is empty.
     * @note The transformations can be either "translate", "rotate", "scale",
     */
    std::vector< std::string > getTransforms() const;

    /**
     * @brief Parent pointer setter to make the composite design pattern
     *
     * @param parent The parent pointer
     * @note This function is used for composite design pattern
     */
    void setParent(SVGElement* parent);

    /**
     * @brief Parent pointer getter
     *
     * @return The parent pointer
     * @note This function is used for composite design pattern
     */
    SVGElement* getParent() const;

    /**
     * @brief Sets the gradient of the shape.
     *
     * @param gradient The new gradient of the shape.
     * @note The default gradient of the shape is NULL.
     */
    void setGradient(Gradient* gradient);

    /**
     * @brief Gets the gradient of the shape.
     *
     * @return The gradient of the shape.
     * @note The default gradient of the shape is NULL.
     */
    Gradient* getGradient() const;

    /**
     * @brief Adds a shape to the composite group.
     * @param element The shape to be added to the composite group.
     * @note This function is used for composite design pattern
     * @note This function is virtual and can be overridden by derived classes.
     */
    virtual void addElement(SVGElement* element);

protected:
    /**
     * @brief Constructs a Shape object
     * @note This constructor is protected because Shape is an abstract class
     * that cannot be instantiated.
     */
    SVGElement();

    /**
     * @brief Constructs a Shape object
     * @param fill The fill color of the shape
     * @param stroke The outline color of the shape
     * @param stroke_width The outline thickness of the shape
     * @note This constructor is protected because Shape is an abstract class
     * that cannot be instantiated.
     */
    SVGElement(const mColor& fill, const mColor& stroke, float stroke_width);

    /**
     * @brief Constructs a Shape object
     * @param fill The fill color of the shape
     * @param stroke The outline color of the shape
     * @param stroke_width The outline thickness of the shape
     * @param position The position of the shape
     * @note This constructor is protected because Shape is an abstract class
     * that cannot be instantiated.
     */
    SVGElement(const mColor& fill, const mColor& stroke, float stroke_width,
               const Vector2Df& position);

    SVGElement* parent;  ///< Pointer to the group that contains the shape

private:
    mColor fill;         ///< Fill color
    mColor stroke;       ///< Outline color
    float stroke_width;  ///< Thickness of the shape's outline
    Vector2Df position;  ///< Position of the shape
    std::vector< std::string > transforms;  ///< List of transformations
    Gradient* gradient;  ///< Pointer to the gradient that contains the shape
};

#endif  // SVG_ELEMENT_HPP_