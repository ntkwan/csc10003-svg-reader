#ifndef SHAPE_HPP_
#define SHAPE_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

/**
 * @brief Represents a drawable shape.
 *
 * The Shape class is an abstract class that represents a drawable shape.
 */
class Shape {
public:
    /**
     * @brief Virtual destructor.
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
     * @brief Sets the thickness of the shape's outline.
     *
     * @param thickness The new outline thickness of the shape.
     */
    void setOutlineThickness(float thickness);

    /**
     * @brief Get fill color of the shape.
     *
     * @return Fill color of the shape.
     */
    const sf::Color& getFillColor() const;

    /**
     * @brief Get outline color of the shape.
     *
     * @return Outline color of the shape.
     */
    const sf::Color& getOutlineColor() const;

    /**
     * @brief Get outline thickness of the shape.
     *
     * @return Outline thickness of the shape.
     */
    float getOutlineThickness() const;

    /**
     * @brief Sets the local origin of the shape.
     *
     * The origin of an object defines the center point for all transformations
     *
     * @param x The X-coordinate of the origin.
     * @param y The Y-coordinate of the origin.
     */
    void setPosition(float x, float y);

    /**
     * @brief Sets the local origin of the shape.
     *
     * The origin of an object defines the center point for all transformations
     *
     * @param position The new origin of the shape.
     */
    void setPosition(const sf::Vector2f& position);

    /**
     * @brief Get the total number of points of the shape.
     *
     * @return Number of points of the shape.
     */
    virtual std::size_t getPointCount() const = 0;

    /**
     * @brief Get a point of the shape.
     *
     * The returned point is in local coordinates, that is, the shape's
     * transforms (position, rotation, scale) are not taken into account.
     * The result is undefined if index is out of the valid range.
     *
     * @param index Index of the point to get, in range [0 .. getPointCount() -
     * 1]
     *
     * @return index-th point of the shape
     */
    virtual sf::Vector2f getPoint(std::size_t index) const = 0;

    /**
     * @brief Draw the shape to a render target.
     *
     * @param target Render target to draw to
     * @param states Current render states
     */
    virtual void draw(
        sf::RenderWindow& target,
        sf::RenderStates states = sf::RenderStates::Default) const;

    /**
     * @brief Get the combined transform of the object.
     *
     * @return Transform combining the position/rotation/scale/origin of the
     * object
     */
    const sf::Transform& getTransform() const;

    /**
     * @brief Get the inverse of the combined transform of the object.
     *
     * @return Inverse of the combined transformations of the object
     */
    const sf::Transform& getInverseTransform() const;

protected:
    /**
     * @brief Default constructor.
     */
    Shape();

    /**
     * @brief Recompute the internal geometry of the shape.
     *
     * This function must be called by the derived class everytime
     * the shape's points change (i.e. the result of either getPointCount
     * or getPoint is different).
     */
    void update();

private:
    /**
     * @brief Update the fill vertices' color.
     */
    void updateFillColors();

    /**
     * @brief Update the outline vertices' position.
     */
    void updateOutline();

    /**
     * @brief Update the outline vertices' color.
     */
    void updateOutlineColors();

private:
    const sf::Texture* texture;  //!< Texture of the shape
    sf::Color fill_color;        //!< Fill color
    sf::Color outline_color;     //!< Outline color
    float outline_thickness;     //!< Thickness of the shape's outline
    sf::VertexArray vertices;    //!< Vertex array containing the fill geometry
    sf::VertexArray outline_vertices;  //!< Vertex array containing the outline
                                       //!< geometry
    sf::FloatRect inside_bounds;  //!< Bounding rectangle of the inside (fill)
    sf::FloatRect bounds;  //!< Bounding rectangle of the whole shape (outline
                           //!< + fill)

    sf::Vector2f
        origin;  //!< Origin of translation/rotation/scaling of the object
    sf::Vector2f position;  //!< Position of the object in the 2D world
    float rotation;         //!< Orientation of the object, in degrees
    sf::Vector2f scale;     //!< Scale of the object
    mutable sf::Transform transform;  //!< Combined transformation of the object
    mutable bool
        transform_need_update;  //!< Does the transform need to be recomputed?
    mutable sf::Transform
        inverse_transform;  //!< Combined transformation of the object
    mutable bool inverse_transform_need_update;  //!< Does the transform need to
                                                 //!< be recomputed?
};

#endif  // SHAPE_HPP_