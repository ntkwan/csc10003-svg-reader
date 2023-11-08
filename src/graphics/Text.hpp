#ifndef TEXT_HPP_
#define TEXT_HPP_

#include <SFML/Graphics.hpp>

#include "Shape.hpp"
class Text : public Shape {
private:
    sf::Text text;

public:
    static sf::Font font;

    Text(sf::Vector2f pos, sf::String TEXT,
         sf::Color fill_color = sf::Color::Black, unsigned int font_size = 1);
    virtual std::size_t getPointCount() const;
    virtual sf::Vector2f getPoint(std::size_t index) const;
    void draw(sf::RenderWindow& window);
};

#endif