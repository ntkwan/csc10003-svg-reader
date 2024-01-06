#include "Stop.hpp"

Stop::Stop(const ColorShape& color, float offset)
    : color(color), offset(offset) {}

ColorShape Stop::getColor() const { return color; }

float Stop::getOffset() const { return offset; }