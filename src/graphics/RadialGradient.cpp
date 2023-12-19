#include "RadialGradient.hpp"

RadialGradient::RadialGradient(std::vector< Stop > stops,
                               std::pair< Vector2Df, Vector2Df > points,
                               Vector2Df radius)
    : Gradient(stops, points) {
    this->radius = radius;
}

std::string RadialGradient::getClass() const { return "RadialGradient"; }