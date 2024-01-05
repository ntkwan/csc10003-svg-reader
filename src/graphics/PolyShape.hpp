#ifndef POLYSHAPE_HPP_
#define POLYSHAPE_HPP_

#include "SVGElement.hpp"

/**
 * @brief Abstract base class for polygon and polyline shapes in 2D space.
 *
 * The PolyShape class is derived from the SVGElement class and defines a common
 * interface for polyline and polygon shapes.
 */
class PolyShape : public SVGElement {
protected:
    std::vector< Vector2Df > points;  ///< Vertices of the polyshape
    std::string fill_rule;            ///< Fill rule of the polyshape

    /**
     * @brief Constructs a PolyShape object.
     *
     * @param fill Fill color of the polyshape (default is
     * sf::Color::Transparent).
     * @param stroke Outline color of the polyshape (default is
     * sf::Color::White).
     * @param stroke_width Thickness of the polyshape outline (default is 0).
     */
    PolyShape(const mColor &fill, const mColor &stroke, float stroke_width);

public:
    /**
     * @brief Gets the type of the shape.
     *
     * @note This function is pure virtual and must be implemented by derived
     * classes.
     */
    std::string getClass() const = 0;

    /**
     * @brief Adds a vertex to the shape.
     *
     * @param point The position of the vertex to be added.
     */
    virtual void addPoint(const Vector2Df &point);

    /**
     * @brief Gets the total number of vertices representing the shape.
     *
     * @return The number of vertices representing the shape.
     */
    const std::vector< Vector2Df > &getPoints() const;

    /**
     * @brief Sets the fill rule of the polyshape.
     *
     * @param fill_rule The new fill rule of the polyshape.
     */
    void setFillRule(std::string fill_rule);

    /**
     * @brief Gets the fill rule of the polyshape.
     *
     * @return The fill rule of the polyshape.
     */
    std::string getFillRule() const;

    /**
     * @brief Gets the minimum bounding box of the shape.
     *
     * @return The minimum bounding box of the shape.
     */
    Vector2Df getMinBound() const override;

    /**
     * @brief Gets the maximum bounding box of the shape.
     *
     * @return The maximum bounding box of the shape.
     */
    Vector2Df getMaxBound() const override;

    /**
     * @brief Prints the data of the shape.
     *
     * @note This function is used for debugging purposes.
     */
    void printData() const override;
};

#endif  // POLYSHAPE_HPP_