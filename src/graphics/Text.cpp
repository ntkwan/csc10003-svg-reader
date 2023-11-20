#include "Text.hpp"

#include <SFML/Graphics.hpp>

sf::Font Text::font;

Text::Text(Vector2Df pos, sf::String TEXT, sf::Color fill_color,
           float font_size) {
    text.setPosition(sf::Vector2f(pos.x, pos.y));
    text.setFont(font);
    text.setCharacterSize(font_size);
    text.setFillColor(fill_color);
    text.setString(TEXT);
}
std::size_t Text::getPointCount() const { return 0; }

Vector2Df Text::getPoint(std::size_t index) const { return Vector2Df(0, 0); }

void Text::draw(Renderer& target) const { target.window.draw(this->text); }