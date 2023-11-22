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

public:
    virtual std::string getClass() const = 0;
    /**
     * @brief Adds a vertex to the shape.
     *
     * This pure virtual function must be implemented by derived classes to add
     * a vertex to the shape.
     *
     * @param point The position of the vertex to be added.
     */
    virtual void addPoint(const Vector2Df &point) = 0;

    /**
     * @brief Updates the shape.
     *
     * This pure virtual function must be implemented by derived classes to
     * update the shape. This method is provided for consistency with other
     * shapes but does not introduce any additional behavior for polygons.
     */
    virtual void updateShape() = 0;

    /**
     * @brief Gets the total number of vertices representing the shape.
     *
     * @return The number of vertices representing the shape.
     */
    std::vector< Vector2Df > getPoints() const { return points; }
};

#endif  // POLYSHAPE_HPP_