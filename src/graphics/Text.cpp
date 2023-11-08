#include "Text.hpp"

sf::Font Text::font;

Text::Text(sf::Vector2f pos, sf::String TEXT, sf::Color fill_color,
           unsigned int font_size) {
    text.setPosition(pos);
    text.setFont(font);
    text.setCharacterSize(font_size);
    text.setFillColor(fill_color);
    text.setString(TEXT);
}
std::size_t Text::getPointCount() const { return 0; }

sf::Vector2f Text::getPoint(std::size_t index) const {
    return sf::Vector2f(0, 0);
}

void Text::draw(sf::RenderWindow& window) { window.draw(this->text); }