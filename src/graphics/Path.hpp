#ifndef PATH_HPP_
#define PATH_HPP_

#include "SVGElement.hpp"

struct PathPoint {
    Vector2Df Point;
    char TC;
};

class Path : public SVGElement {
public:
    Path(const mColor& fill, const mColor& stroke, float stroke_width);

    std::string getClass() const override;

    // void render(Renderer& renderer) const override;

    void addPoint(PathPoint point);

    std::vector< PathPoint > getPoints() const;

    void setFillRule(std::string fill_rule);

    std::string getFillRule() const;

    void printData() const override;

private:
    std::vector< PathPoint > points;
    std::string fill_rule;
};

#endif