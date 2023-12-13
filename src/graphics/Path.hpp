#ifndef PATH_HPP_
#define PATH_HPP_

#include "SVGElement.hpp"

/**
 * @brief A struct that contains a point and a type of point.
 */

struct PathPoint {
    Vector2Df Point;
    char TC;
};

/**
 * @brief Represents a path element in 2D space.
 *
 * The Path class is derived from the SVGElement class and represents a path
 * element in 2D space. The Path class is used to draw lines, curves, arcs, and
 * other shapes. The Path class contains a vector of PathPoints that represent
 * the points in the path.
 *
 */
class Path : public SVGElement {
public:
    /**
     * @brief Constructs a Path object.
     *
     * @param fill Fill color of the path.
     * @param stroke Outline color of the path.
     * @param stroke_width Thickness of the path outline.
     */
    Path(const mColor& fill, const mColor& stroke, float stroke_width);

    /**
     * @brief Gets the type of the shape.
     *
     * @return The string "Path".
     */
    std::string getClass() const override;

    /**
     * @brief Adds a point to the path.
     *
     * @param point The point to be added to the path.
     *
     * @note This function is used for adding points to the path.
     */
    void addPoint(PathPoint point);

    /**
     * @brief Gets the vector of points in the path.
     *
     * @return The vector of points in the path.
     */
    std::vector< PathPoint > getPoints() const;

    /**
     * @brief Sets the fill rule of the path.
     *
     * @param fill_rule The new fill rule of the path.
     *
     * @note This function is used for setting the fill rule of the path.
     * @note The fill rule can be either "nonzero" or "evenodd".
     */
    void setFillRule(std::string fill_rule);

    /**
     * @brief Gets the current fill rule of the path.
     *
     * @return The current fill rule of the path.
     *
     * @note The fill rule can be either "nonzero" or "evenodd".
     * @note The default fill rule is "nonzero".
     */
    std::string getFillRule() const;

    /**
     * @brief Prints the data of the shape.
     *
     * @note This function is used for debugging purposes.
     */
    void printData() const override;

private:
    std::vector< PathPoint > points;  ///< Vector of points in the path
    std::string fill_rule;            ///< Fill rule of the path
};

#endif