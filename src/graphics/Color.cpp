#include "Color.hpp"

#include <algorithm>
const mColor mColor::Black(0, 0, 0);
const mColor mColor::White(255, 255, 255);
const mColor mColor::Red(255, 0, 0);
const mColor mColor::Green(0, 255, 0);
const mColor mColor::Blue(0, 0, 255);
const mColor mColor::Yellow(255, 255, 0);
const mColor mColor::Magenta(255, 0, 255);
const mColor mColor::Cyan(0, 255, 255);
const mColor mColor::Transparent(0, 0, 0, 0);

mColor::mColor() : r(0), g(0), b(0), a(255) {}

mColor::mColor(int red, int green, int blue, int alpha)
    : r(red), g(green), b(blue), a(alpha) {
    r = std::clamp(r, 0, 255);
    g = std::clamp(g, 0, 255);
    b = std::clamp(b, 0, 255);
    a = std::clamp(a, 0, 255);
}

mColor::mColor(int color)
    : r(static_cast< int >((color & 0xff000000) >> 24)),
      g(static_cast< int >((color & 0x00ff0000) >> 16)),
      b((color & 0x0000ff00) >> 8), a((color & 0x000000ff) >> 0) {}

std::ostream& operator<<(std::ostream& os, const mColor& color) {
    os << "Color(" << color.r << ", " << color.g << ", " << color.b << ", "
       << color.a << ")";
    return os;
}