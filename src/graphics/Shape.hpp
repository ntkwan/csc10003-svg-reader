#ifndef SHAPE_HPP_
#define SHAPE_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

/**
 * @brief Represents a shape in 2D space
 * @note This class is abstract and cannot be instantiated.
 * @note This class is applied Abstract Factory design pattern and used as
 * interface for other shapes.
 */
class Shape {
public:
    /*! @brief Virtual constructor
     */

    virtual ~Shape() = default;

    /**
     * @brief Sets the fill color of the shape.
     *
     * @param color The new fill color of the shape.
     */
    void setFillColor(const sf::Color& color);

    /**
     * @brief Sets the outline color of the shape.
     *
     * @param color The new outline color of the shape.
     */
    void setOutlineColor(const sf::Color& color);

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
     * @brief Gets the fill color of the shape.
     * @return The fill color of the shape.
     * @note The default fill color is white.
     */
    const sf::Color& getFillColor() const;

    /**
     * @brief Gets the outline color of the shape.
     * @return The outline color of the shape.
     * @note The default outline color is white.
     */
    const sf::Color& getOutlineColor() const;

    /**
     * @brief Gets the outline thickness of the shape.
     * @return The outline thickness of the shape.
     * @note The default outline thickness is 0.
     */
    float getOutlineThickness() const;

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
     * @param position The new position of the shape (sf::Vector2f is a typedef
     * of coordination in SFML)
     * @note The default position of the shape is (0, 0).
     * @note The position of the shape is relative to its origin.
     */
    void setPosition(const sf::Vector2f& position);

    /**
     * @brief Virtual method: Get the number of point of the shape (for Polygon
     * shape)
     * @return The number of points of the shape
     * @note This is a pure virtual method, so it has to be implemented by the
     * derived class to define how the shape should be drawn.
     */
    virtual std::size_t getPointCount() const = 0;

    /**
     * @brief Virtual method: Get the position of the point on the shape (for
     * Polygon shape)
     * @param index The index of the point
     * @return The position of the specified point on the shape
     * @note The returned point is in local coordinates, that is, the shape's
     * transforms (position, rotation, scale) are not taken into account.
     * @note The result is undefined if index is out of the valid range.
     * @note The number of points of the shape is defined by the concrete
     * implementation.
     * @note The returned vector is constant, which means that you can't modify
     * its coordinates when you retrieve it.
     * @note This is a pure virtual method, so it has to be implemented by the
     * derived class.
     */
    virtual sf::Vector2f getPoint(std::size_t index) const = 0;

    /**
     * @brief Virtual method: Draw the shape on the specified render target
     * @param target The render target (sf::Renderwindow is a typedef of SFML
     * drawing window)
     * @param states The render states to apply (default is
     * sf::RenderStates::Default)
     */
    virtual void draw(
        sf::RenderWindow& target,
        sf::RenderStates states = sf::RenderStates::Default) const;

    /**
     * @brief Gets the shape transform
     * @return The shape transform (sf::Transform is a typedef of SFML)
     * @note This function returns the combined transform of the object.
     * @note The transform is a combination of the position, rotation, and scale
     * of the object.
     */
    const sf::Transform& getTransform() const;

    /**
     * @brief Gets the inverse shape transform
     * @return The inverse shape transform (sf::Transform is a typedef of SFML)
     * @note This function returns the inverse of the combined transform of the
     * object.
     */
    const sf::Transform& getInverseTransform() const;

protected:
    /**
     * @brief Constructs a Shape object
     * @note This constructor is protected because Shape is an abstract class
     * that cannot be instantiated.
     */
    Shape();

    /**
     * @brief Sets the texture of the shape
     * @param texture The new texture of the shape
     * @note The texture is not copied, it is referenced by the shape.
     * @note The default texture is NULL.
     */
    void update();

private:
    /**
     * @brief Updates the fill colors of the shape
     * @note This method call the update() method.
     */
    void updateFillColors();

    /**
     * @brief Updates the outline of the shape
     * @note This method call the update() method.
     */
    void updateOutline();

    /**
     * @brief Updates the outline colors of the shape
     * @note This method call the update() method.
     */
    void updateOutlineColors();

private:
    const sf::Texture* texture;  ///< Texture of the shape
    sf::Color fill_color;        ///< Fill color
    sf::Color outline_color;     ///< Outline color
    float outline_thickness;     ///< Thickness of the shape's outline
    sf::VertexArray vertices;    ///< Vertex array containing the fill geometry
    sf::VertexArray
        outline_vertices;  ///< Vertex array containing the outline geometry
    sf::FloatRect inside_bounds;  ///< Bounding rectangle of the inside (fill)
    sf::FloatRect
        bounds;  ///< Bounding rectangle of the whole shape (outline + fill)


    sf::Vector2f
        origin;  ///< Origin of translation/rotation/scaling of the object
    sf::Vector2f position;  ///< Position of the object in the 2D world
    float rotation;         ///< Orientation of the object, in degrees
    sf::Vector2f scale;     ///< Scale of the object

    mutable sf::Transform transform;  ///< Combined transformation of the object
    mutable bool
        transform_need_update;  ///< Does the transform need to be recomputed?
    mutable sf::Transform
        inverse_transform;  ///< Combined transformation of the object
    mutable bool
        inverse_transform_need_update;  ///< Same as transform but for inverse
};

#endif  // SHAPE_HPP_