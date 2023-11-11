#ifndef SHAPE_HPP_
#define SHAPE_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

class Shape : public sf::Transformable {
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

    virtual void draw(sf::RenderWindow& target, sf::RenderStates states) const;

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
};

#endif  // SHAPE_HPP_