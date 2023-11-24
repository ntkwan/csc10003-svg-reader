#include "Color.hpp"

const Color Color::Black(0, 0, 0);
const Color Color::White(255, 255, 255);
const Color Color::Red(255, 0, 0);
const Color Color::Green(0, 255, 0);
const Color Color::Blue(0, 0, 255);
const Color Color::Yellow(255, 255, 0);
const Color Color::Magenta(255, 0, 255);
const Color Color::Cyan(0, 255, 255);
const Color Color::Transparent(0, 0, 0, 0);

Color::Color() : r(0), g(0), b(0), a(255) {}

Color::Color(int red, int green, int blue, int alpha)
    : r(red), g(green), b(blue), a(alpha) {}

Color::Color(int color)
    : r(static_cast< int >((color & 0xff000000) >> 24)),
      g(static_cast< int >((color & 0x00ff0000) >> 16)),
      b((color & 0x0000ff00) >> 8), a((color & 0x000000ff) >> 0) {}

std::ostream& operator<<(std::ostream& os, const Color& color) {
    os << "Color(" << color.r << ", " << color.g << ", " << color.b << ", "
       << color.a << ")";
    return os;
}