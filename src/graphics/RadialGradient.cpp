#include "RadialGradient.hpp"

RadialGradient::RadialGradient(std::vector< Stop > stops,
                               std::pair< Vector2Df, Vector2Df > points,
                               Vector2Df radius, std::string units)
    : Gradient(stops, points, units) {
    this->radius = radius;
}

std::string RadialGradient::getClass() const { return "RadialGradient"; }