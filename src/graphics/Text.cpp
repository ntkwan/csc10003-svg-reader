#include "Text.hpp"

Text::Text(Vector2Df pos, std::string TEXT, Color fill_color, float font_size)
    : content(TEXT), position(pos) {
    setOutlineColor(fill_color);
    setOutlineThickness(font_size);
}
std::size_t Text::getPointCount() const { return 0; }

Vector2Df Text::getPoint(std::size_t index) const { return Vector2Df(0, 0); }

std::string Text::getContent() const { return content; }

Vector2Df Text::getPosition() const { return position; }