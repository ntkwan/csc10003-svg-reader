#include "Stop.hpp"

Stop::Stop(const mColor& color, float offset) : color(color), offset(offset) {}

mColor Stop::getColor() const { return color; }

float Stop::getOffset() const { return offset; }