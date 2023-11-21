#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include "Vector2D.hpp"

/**
 * @brief Define a 2D transform matrix
 *
 * A Transform specifies how to translate, rotate, scale,
 * shear, project, whatever things. In mathematical terms, it defines
 * how to transform a coordinate system into another.
 *
 * For example, if you apply a rotation transform to a sprite, the
 * result will be a rotated sprite. And anything that is transformed
 * by this rotation transform will be rotated the same way, according
 * to its initial position.
 *
 * Transforms are typically used for drawing. But they can also be
 * used for any computation that requires to transform points between
 * the local and global coordinate systems of an entity (like collision
 * detection).
 */

class Transform {
public:
    /**
     * @brief Default constructor
     *
     * Creates an identity transform (a transform that does nothing).
     */
    Transform();

    /**
     * @brief Construct a transform from a 3x3 matrix
     *
     * @param a00 Element (0, 0) of the matrix
     * @param a01 Element (0, 1) of the matrix
     * @param a02 Element (0, 2) of the matrix
     * @param a10 Element (1, 0) of the matrix
     * @param a11 Element (1, 1) of the matrix
     * @param a12 Element (1, 2) of the matrix
     * @param a20 Element (2, 0) of the matrix
     * @param a21 Element (2, 1) of the matrix
     * @param a22 Element (2, 2) of the matrix
     */
    Transform(float a00, float a01, float a02, float a10, float a11, float a12,
              float a20, float a21, float a22);

    /**
     * @brief Return the transform as a 4x4 matrix
     *
     * This function returns a pointer to an array of 16 floats
     * containing the transform elements as a 4x4 matrix, which
     * is directly compatible with OpenGL functions.
     *
     * @return Pointer to a 4x4 matrix
     */
    const float* getMatrix() const;

    /**
     * @brief Return the inverse of the transform
     *
     * If the inverse cannot be computed, an identity transform
     * is returned.
     *
     * @return A new transform which is the inverse of self
     */
    Transform getInverse() const;

    /**
     * @brief Transform a 2D point
     *
     * @param x X coordinate of the point to transform
     * @param y Y coordinate of the point to transform
     *
     * @return Transformed point
     */
    Vector2Df transformPoint(float x, float y) const;

    /**
     * @brief Transform a 2D point
     *
     * @param point Point to transform
     *
     * @return Transformed point
     */
    Vector2Df transformPoint(const Vector2Df& point) const;

    /**
     * @brief Combine the current transform with another one
     * @param transform Transform to combine with this transform
     *
     * @return Reference to *this
     */
    Transform& combine(const Transform& transform);

    /**
     * @brief Combine the current transform with a translation
     *
     * This function returns a reference to *this, so that calls
     * can be chained.
     *
     * @param x Offset to apply on X axis
     * @param y Offset to apply on Y axis
     *
     * @return Reference to *this
     */
    Transform& translate(float x, float y);

    /**
     * @brief Combine the current transform with a translation
     *
     * This function returns a reference to *this, so that calls
     * can be chained.
     *
     * @param offset Translation offset to apply
     *
     * @return Reference to *this
     */
    Transform& translate(const Vector2Df& offset);

    /**
     * @brief Combine the current transform with a rotation
     *
     * This function returns a reference to *this, so that calls
     * can be chained.
     *
     * @param angle Rotation angle, in degrees
     *
     * @return Reference to *this
     */
    Transform& rotate(float angle);

    /**
     * @brief Combine the current transform with a rotation
     *
     * The center of rotation is provided for convenience as a second
     * argument, so that you can build rotations around arbitrary points
     * more easily (and efficiently) than the usual
     * translate(-center).rotate(angle).translate(center).
     *
     * This function returns a reference to *this, so that calls
     * can be chained.
     *
     * @param angle Rotation angle, in degrees
     * @param centerX X coordinate of the center of rotation
     * @param centerY Y coordinate of the center of rotation
     *
     * @return Reference to *this
     */
    Transform& rotate(float angle, float centerX, float centerY);

    /**
     * @brief Combine the current transform with a rotation
     *
     * The center of rotation is provided for convenience as a second
     * argument, so that you can build rotations around arbitrary points
     * more easily (and efficiently) than the usual
     * translate(-center).rotate(angle).translate(center).
     *
     * This function returns a reference to *this, so that calls
     * can be chained.
     *
     * @param angle Rotation angle, in degrees
     * @param center Center of rotation
     *
     * @return Reference to *this
     */
    Transform& rotate(float angle, const Vector2Df& center);

    /**
     * @brief Combine the current transform with a scaling
     *
     * This function returns a reference to *this, so that calls
     * can be chained.
     *
     * @param scaleX Scaling factor on the X axis
     * @param scaleY Scaling factor on the Y axis
     *
     * @return Reference to *this
     */
    Transform& scale(float scaleX, float scaleY);
    /**
     * @brief Combine the current transform with a scaling
     *
     * The center of scaling is provided for convenience as a second
     * argument, so that you can build scaling around arbitrary points
     * more easily (and efficiently) than the usual
     * translate(-center).scale(factors).translate(center).
     *
     * This function returns a reference to *this, so that calls
     * can be chained.
     *
     * @param scaleX Scaling factor on X axis
     * @param scaleY Scaling factor on Y axis
     * @param centerX X coordinate of the center of scaling
     * @param centerY Y coordinate of the center of scaling
     *
     * @return Reference to *this
     */
    Transform& scale(float scaleX, float scaleY, float centerX, float centerY);

    /**
     * @brief Combine the current transform with a scaling
     *
     * This function returns a reference to *this, so that calls
     * can be chained.
     *
     * @param factors Scaling factors
     *
     * @return Reference to *this
     */
    Transform& scale(const Vector2Df& factors);

    /**
     * @brief Combine the current transform with a scaling
     *
     * The center of scaling is provided for convenience as a second
     * argument, so that you can build scaling around arbitrary points
     * more easily (and efficiently) than the usual
     * translate(-center).scale(factors).translate(center).
     *
     * This function returns a reference to *this, so that calls
     * can be chained.
     *
     * @param factors Scaling factors
     * @param center Center of scaling
     *
     * @return Reference to *this
     */
    Transform& scale(const Vector2Df& factors, const Vector2Df& center);

    static const Transform Identity;  ///< The identity transform (does nothing)

private:
    float mat[16];  ///< 4x4 matrix defining the transformation
};

#endif  // TRANSFORM_HPP_
