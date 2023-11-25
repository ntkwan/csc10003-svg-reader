#ifndef PATH_HPP_
#define PATH_HPP_

#include <algorithm>
#include <sstream>
#include <vector>

#include "Curve.hpp"
#include "Line.hpp"
#include "Shape.hpp"

struct PathPoint {
    Vector2Df Point;
    char TC;
};

class Path : public Shape {
private:
    std::vector< PathPoint > points;

public:
    Path(const Color& fill, const Color& stroke, float stroke_width);

    std::string getClass() const override;

    void addPoint(PathPoint point);

    std::vector< PathPoint > getPoints() const;
};

#endif