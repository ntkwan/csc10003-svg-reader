#include "Text.hpp"

Text::Text(Vector2Df pos, std::string text, float font_size, const mColor &fill,
           const mColor &stroke, float stroke_width)
    : SVGElement(fill, stroke, stroke_width, pos), content(text),
      font_size(font_size) {}

std::string Text::getClass() const { return "Text"; }

void Text::setFontSize(float font_size) { this->font_size = font_size; }

float Text::getFontSize() const { return font_size; }

void Text::setContent(std::string content) { this->content = content; }

std::string Text::getContent() const { return content; }

void Text::setAnchor(std::string anchor) { this->anchor = anchor; }

std::string Text::getAnchor() const { return anchor; }

void Text::setFontStyle(std::string font_style) { this->style = font_style; }

std::string Text::getFontStyle() const { return style; }

void Text::printData() const {
    SVGElement::printData();
    std::cout << "Content: " << getContent() << std::endl;
    std::cout << "Font size: " << getFontSize() << std::endl;
}