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

    void setTexture(const sf::Texture* texture, bool resetRect = false);

    void setTextureRect(const sf::IntRect& rect);

    void setFillColor(const sf::Color& color);

    void setOutlineColor(const sf::Color& color);

    void setOutlineThickness(float thickness);

    const sf::Texture* getTexture() const;

    const sf::IntRect& getTextureRect() const;

    const sf::Color& getFillColor() const;

    const sf::Color& getOutlineColor() const;

    float getOutlineThickness() const;

    virtual std::size_t getPointCount() const = 0;

    virtual sf::Vector2f getPoint(std::size_t index) const = 0;

    sf::FloatRect getLocalBounds() const;

    sf::FloatRect getGlobalBounds() const;

    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f& position);

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

    void updateTexCoords();

    void updateOutline();

    void updateOutlineColors();

private:
    const sf::Texture* m_texture;  //!< Texture of the shape
    sf::IntRect m_textureRect;   //!< Rectangle defining the area of the source
                                 //!< texture to display
    sf::Color m_fillColor;       //!< Fill color
    sf::Color m_outlineColor;    //!< Outline color
    float m_outlineThickness;    //!< Thickness of the shape's outline
    sf::VertexArray m_vertices;  //!< Vertex array containing the fill geometry
    sf::VertexArray m_outlineVertices;  //!< Vertex array containing the outline
                                        //!< geometry
    sf::FloatRect m_insideBounds;  //!< Bounding rectangle of the inside (fill)
    sf::FloatRect m_bounds;  //!< Bounding rectangle of the whole shape (outline
                             //!< + fill)

    sf::Vector2f
        m_origin;  //!< Origin of translation/rotation/scaling of the object
    sf::Vector2f m_position;  //!< Position of the object in the 2D world
    float m_rotation;         //!< Orientation of the object, in degrees
    sf::Vector2f m_scale;     //!< Scale of the object
    mutable sf::Transform
        m_transform;  //!< Combined transformation of the object
    mutable bool
        m_transformNeedUpdate;  //!< Does the transform need to be recomputed?
    mutable sf::Transform
        m_inverseTransform;  //!< Combined transformation of the object
    mutable bool m_inverseTransformNeedUpdate;  //!< Does the transform need to
                                                //!< be recomputed?
};

#endif  // SHAPE_HPP_