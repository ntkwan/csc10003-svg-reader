#include "Transform.hpp"

#include <cmath>

const Transform Transform::Identity;

Transform::Transform() {
    // Identity mat
    mat[0] = 1.f;
    mat[4] = 0.f;
    mat[8] = 0.f;
    mat[12] = 0.f;
    mat[1] = 0.f;
    mat[5] = 1.f;
    mat[9] = 0.f;
    mat[13] = 0.f;
    mat[2] = 0.f;
    mat[6] = 0.f;
    mat[10] = 1.f;
    mat[14] = 0.f;
    mat[3] = 0.f;
    mat[7] = 0.f;
    mat[11] = 0.f;
    mat[15] = 1.f;
}

Transform::Transform(float a00, float a01, float a02, float a10, float a11,
                     float a12, float a20, float a21, float a22) {
    mat[0] = a00;
    mat[4] = a01;
    mat[8] = 0.f;
    mat[12] = a02;
    mat[1] = a10;
    mat[5] = a11;
    mat[9] = 0.f;
    mat[13] = a12;
    mat[2] = 0.f;
    mat[6] = 0.f;
    mat[10] = 1.f;
    mat[14] = 0.f;
    mat[3] = a20;
    mat[7] = a21;
    mat[11] = 0.f;
    mat[15] = a22;
}

const float* Transform::getMatrix() const { return mat; }

Transform Transform::getInverse() const {
    // Compute the determinant
    float det = mat[0] * (mat[15] * mat[5] - mat[7] * mat[13]) -
                mat[1] * (mat[15] * mat[4] - mat[7] * mat[12]) +
                mat[3] * (mat[13] * mat[4] - mat[5] * mat[12]);

    // Compute the inverse if the determinant is not zero
    // (don't use an epsilon because the determinant may *really* be tiny)
    if (det != 0.f) {
        return Transform((mat[15] * mat[5] - mat[7] * mat[13]) / det,
                         -(mat[15] * mat[4] - mat[7] * mat[12]) / det,
                         (mat[13] * mat[4] - mat[5] * mat[12]) / det,
                         -(mat[15] * mat[1] - mat[3] * mat[13]) / det,
                         (mat[15] * mat[0] - mat[3] * mat[12]) / det,
                         -(mat[13] * mat[0] - mat[1] * mat[12]) / det,
                         (mat[7] * mat[1] - mat[3] * mat[5]) / det,
                         -(mat[7] * mat[0] - mat[3] * mat[4]) / det,
                         (mat[5] * mat[0] - mat[1] * mat[4]) / det);
    } else {
        return Identity;
    }
}

Vector2Df Transform::transformPoint(float x, float y) const {
    return Vector2Df(mat[0] * x + mat[4] * y + mat[12],
                     mat[1] * x + mat[5] * y + mat[13]);
}

Vector2Df Transform::transformPoint(const Vector2Df& point) const {
    return transformPoint(point.x, point.y);
}

Transform& Transform::combine(const Transform& transform) {
    const float* a = mat;
    const float* b = transform.mat;

    *this = Transform(a[0] * b[0] + a[4] * b[1] + a[12] * b[3],
                      a[0] * b[4] + a[4] * b[5] + a[12] * b[7],
                      a[0] * b[12] + a[4] * b[13] + a[12] * b[15],
                      a[1] * b[0] + a[5] * b[1] + a[13] * b[3],
                      a[1] * b[4] + a[5] * b[5] + a[13] * b[7],
                      a[1] * b[12] + a[5] * b[13] + a[13] * b[15],
                      a[3] * b[0] + a[7] * b[1] + a[15] * b[3],
                      a[3] * b[4] + a[7] * b[5] + a[15] * b[7],
                      a[3] * b[12] + a[7] * b[13] + a[15] * b[15]);

    return *this;
}

Transform& Transform::translate(float x, float y) {
    Transform translation(1, 0, x, 0, 1, y, 0, 0, 1);

    return combine(translation);
}

Transform& Transform::translate(const Vector2Df& offset) {
    return translate(offset.x, offset.y);
}

Transform& Transform::rotate(float angle) {
    float rad = angle * 3.141592654f / 180.f;
    float cos = std::cos(rad);
    float sin = std::sin(rad);

    Transform rotation(cos, -sin, 0, sin, cos, 0, 0, 0, 1);

    return combine(rotation);
}

Transform& Transform::rotate(float angle, float centerX, float centerY) {
    float rad = angle * 3.141592654f / 180.f;
    float cos = std::cos(rad);
    float sin = std::sin(rad);

    Transform rotation(cos, -sin, centerX * (1 - cos) + centerY * sin, sin, cos,
                       centerY * (1 - cos) - centerX * sin, 0, 0, 1);

    return combine(rotation);
}

Transform& Transform::rotate(float angle, const Vector2Df& center) {
    return rotate(angle, center.x, center.y);
}

Transform& Transform::scale(float scaleX, float scaleY) {
    Transform scaling(scaleX, 0, 0, 0, scaleY, 0, 0, 0, 1);

    return combine(scaling);
}

Transform& Transform::scale(float scaleX, float scaleY, float centerX,
                            float centerY) {
    Transform scaling(scaleX, 0, centerX * (1 - scaleX), 0, scaleY,
                      centerY * (1 - scaleY), 0, 0, 1);

    return combine(scaling);
}

Transform& Transform::scale(const Vector2Df& factors) {
    return scale(factors.x, factors.y);
}

Transform& Transform::scale(const Vector2Df& factors, const Vector2Df& center) {
    return scale(factors.x, factors.y, center.x, center.y);
}
