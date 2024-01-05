#include "LinearGradient.hpp"

LinearGradient::LinearGradient(std::vector< Stop > stops,
                               std::pair< Vector2Df, Vector2Df > points,
                               std::string units)
    : Gradient(stops, points, units) {}

std::string LinearGradient::getClass() const { return "LinearGradient"; }