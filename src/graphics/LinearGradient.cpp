#include "LinearGradient.hpp"

LinearGradient::LinearGradient(std::vector< Stop > stops,
                               std::pair< Vector2Df, Vector2Df > points)
    : Gradient(stops, points) {}

std::string LinearGradient::getClass() const { return "LinearGradient"; }