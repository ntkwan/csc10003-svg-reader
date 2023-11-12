#ifndef SHAPE_HPP_
#define SHAPE_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

class Shape {
public:
    virtual ~Shape() = default;

    void setFillColor(const sf::Color& color);

    void setOutlineColor(const sf::Color& color);

    void setOutlineThickness(float thickness);

    const sf::Color& getFillColor() const;

    const sf::Color& getOutlineColor() const;

    float getOutlineThickness() const;

    void setPosition(float x, float y);

    void setPosition(const sf::Vector2f& position);

    virtual std::size_t getPointCount() const = 0;

    virtual sf::Vector2f getPoint(std::size_t index) const = 0;

    virtual void draw(
        sf::RenderWindow& target,
        sf::RenderStates states = sf::RenderStates::Default) const;

    const sf::Transform& getTransform() const;

    const sf::Transform& getInverseTransform() const;

protected:
    Shape();

    void update();

private:
    void updateFillColors();

    void updateOutline();

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