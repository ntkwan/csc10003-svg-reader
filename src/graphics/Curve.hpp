#ifndef CURVE_HPP_
#define CURVE_HPP_

#include <cmath>
#include <vector>

#include "Polyline.hpp"
#include "Shape.hpp"

class Curve : public PolyShape {
public:
    Curve(const Color& fill, const Color& stroke, float stroke_width);

    std::string getClass() const override;
};

#endif