#include "Text.hpp"

Text::Text(Vector2Df pos, std::string TEXT, const Color &fill, float font_size)
    : content(TEXT), font_size(font_size) {
    setOutlineColor(fill);
    setPosition(pos);
}

std::string Text::getClass() const { return "Text"; }

void Text::setFontSize(float font_size) { this->font_size = font_size; }

float Text::getFontSize() const { return font_size; }

void Text::setContent(std::string content) { this->content = content; }

std::string Text::getContent() const { return content; }

void Text::printData() const {
    Shape::printData();
    std::cout << "Content: " << getContent() << std::endl;
    std::cout << "Font size: " << getFontSize() << std::endl;
}