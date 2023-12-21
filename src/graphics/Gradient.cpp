#include "Gradient.hpp"

Gradient::Gradient(std::vector< Stop > stops,
                   std::pair< Vector2Df, Vector2Df > points, std::string units)
    : stops(stops), points(points), units(units) {}

std::vector< Stop > Gradient::getStops() const { return stops; }

std::pair< Vector2Df, Vector2Df > Gradient::getPoints() const { return points; }

void Gradient::setUnits(std::string units) { this->units = units; }

std::string Gradient::getUnits() const { return units; }

void Gradient::setTransforms(std::vector< std::string > transforms) {
    this->transforms = transforms;
}

std::vector< std::string > Gradient::getTransforms() const {
    return transforms;
}