#ifndef POLYSHAPE_HPP_
#define POLYSHAPE_HPP_

#include "Shape.hpp"

/**
 * @brief Abstract base class for polygon and polyline shapes in 2D space.
 *
 * The PolyShape class is derived from the Shape class and defines a common
 * interface for polyline and polygon shapes.
 */
class PolyShape : public Shape {
protected:
    std::vector< Vector2Df > points;  ///< Vertices of the polyshape

    /**
     * @brief Constructs a PolyShape object.
     *
     * @param fill Fill color of the polyshape (default is
     * sf::Color::Transparent).
     * @param stroke Outline color of the polyshape (default is
     * sf::Color::White).
     * @param stroke_width Thickness of the polyshape outline (default is 0).
     */
    PolyShape(const Color &fill, const Color &stroke, float stroke_width);

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
};

#endif  // POLYSHAPE_HPP_