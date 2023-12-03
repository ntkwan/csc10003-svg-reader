#ifndef PATH_HPP_
#define PATH_HPP_

#include "SVGElement.hpp"

struct PathPoint {
    Vector2Df Point;
    char TC;
};

class Path : public SVGElement {
private:
    std::vector< PathPoint > points;

public:
    Path(const Color& fill, const Color& stroke, float stroke_width);

    std::string getClass() const override;

    // void render(Renderer& renderer) const override;

    void addPoint(PathPoint point);

    std::vector< PathPoint > getPoints() const;

    void printData() const override;
};

#endif