#include "Text.hpp"

#include <SFML/Graphics.hpp>

Text::Text(Vector2Df pos, std::string TEXT, Color fill_color, float font_size)
    : text(TEXT), position(pos) {
    setOutlineColor(fill_color);
    setOutlineThickness(font_size);
    update();
}
std::size_t Text::getPointCount() const { return 0; }

Vector2Df Text::getPoint(std::size_t index) const { return Vector2Df(0, 0); }

void Text::draw(Renderer& target) const {
    sf::Text render_text;
    sf::Font font;
    font.loadFromFile("external/font/Arial.ttf");
    render_text.setString(text);
    render_text.setFont(font);
    render_text.setPosition(position.x, position.y);
    render_text.setCharacterSize(getOutlineThickness());
    Color outline_color = getOutlineColor();
    render_text.setFillColor(sf::Color(outline_color.r, outline_color.g,
                                       outline_color.b, outline_color.a));
    target.window.draw(render_text);
}