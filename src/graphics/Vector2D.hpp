#ifndef VECTOR2D_HPP_
#define VECTOR2D_HPP_

/**
 * @brief Utility template class for manipulating 2-dimensional vectors
 *
 * Vector2D is a simple class that defines a mathematical
 * vector with two coordinates (x and y). It can be used to
 * represent anything that has two dimensions: a size, a point,
 * a velocity, etc.
 *
 * The template parameter T is the type of the coordinates. It
 * can be any type that supports arithmetic operations (+, -, /, *)
 * and comparisons (==, !=), for example int or float.
 */
template< typename T >
class Vector2D {
public:
    /**
     * @brief Default constructor
     *
     * Creates a Vector2(0, 0).
     */
    Vector2D();

    /**
     * @brief Construct the vector from its coordinates
     *
     * @param X X coordinate
     * @param Y Y coordinate
     */
    Vector2D(T X, T Y);

    /**
     * @brief Construct the vector from another type of vector
     *
     * This constructor doesn't replace the copy constructor,
     * it's called only when U != T.
     * A call to this constructor will fail to compile if U
     * is not convertible to T.
     */
    template< typename U >
    explicit Vector2D(const Vector2D< U >& vector);

    T x;  ///< X coordinate of the vector
    T y;  ///< Y coordinate of the vector
};

/**
 * @brief Overload of unary operator -
 *
 * @param right Vector to negate
 * @return Memberwise opposite of the vector
 */
template< typename T >
Vector2D< T > operator-(const Vector2D< T >& right);

/**
 * @brief Overload of binary operator +=
 *
 * This operator performs a memberwise addition of both vectors,
 * and assigns the result to left.
 *
 * @param left Left operand (a vector)
 * @param right Right operand (a vector)
 *
 * @return Reference to left
 */
template< typename T >
Vector2D< T >& operator+=(Vector2D< T >& left, const Vector2D< T >& right);

/**
 * @brief Overload of binary operator -=
 *
 * This operator performs a memberwise subtraction of both vectors,
 * and assigns the result to left.
 *
 * @param left Left operand (a vector)
 * @param right Right operand (a vector)
 *
 * @return Reference to left
 */
template< typename T >
Vector2D< T >& operator-=(Vector2D< T >& left, const Vector2D< T >& right);

/**
 * @brief Overload of binary operator +
 *
 * @param left Left operand (a vector)
 * @param right Right operand (a vector)
 *
 *
 * @return Memberwise addition of both vectors
 */
template< typename T >
Vector2D< T > operator+(const Vector2D< T >& left, const Vector2D< T >& right);

/**
 * @brief Overload of binary operator -
 *
 * @param left Left operand (a vector)
 * @param right Right operand (a vector)
 *
 * @return Memberwise subtraction of both vectors
 */
template< typename T >
Vector2D< T > operator-(const Vector2D< T >& left, const Vector2D< T >& right);

/**
 * @brief Overload of binary operator *
 *
 * @param left Left operand (a vector)
 * @param right Right operand (a scalar value)
 *
 * @return Memberwise multiplication by right
 */
template< typename T >
Vector2D< T > operator*(const Vector2D< T >& left, T right);

/**
 * @brief Overload of binary operator *
 *
 * @param left Left operand (a scalar value)
 * @param right Right operand (a vector)
 *
 * @return Memberwise multiplication by left
 */
template< typename T >
Vector2D< T > operator*(T left, const Vector2D< T >& right);

/**
 * @brief Overload of binary operator *=
 *
 * This operator performs a memberwise multiplication by right,
 * and assigns the result to left.
 *
 * @param left Left operand (a vector)
 * @param right Right operand (a scalar value)
 *
 * @return Reference to left
 */
template< typename T >
Vector2D< T >& operator*=(Vector2D< T >& left, T right);

/**
 * @brief Overload of binary operator /
 *
 * @param left Left operand (a vector)
 * @param right Right operand (a scalar value)
 *
 * @return Memberwise division by right
 */
template< typename T >
Vector2D< T > operator/(const Vector2D< T >& left, T right);

/**
 * @brief Overload of binary operator /=
 *
 * This operator performs a memberwise division by right,
 * and assigns the result to left.
 *
 * @param left Left operand (a vector)
 * @param right Right operand (a scalar value)
 *
 * @return Reference to left
 */
template< typename T >
Vector2D< T >& operator/=(Vector2D< T >& left, T right);

/**
 * @brief Overload of binary operator ==
 *
 * This operator compares strict equality between two vectors.
 *
 * @param left Left operand (a vector)
 * @param right Right operand (a vector)
 *
 * @return True if left is equal to right
 */
template< typename T >
bool operator==(const Vector2D< T >& left, const Vector2D< T >& right);

/**
 * @brief Overload of binary operator !=
 *
 * This operator compares strict difference between two vectors.
 *
 * @param left Left operand (a vector)
 * @param right Right operand (a vector)
 *
 * @return True if left is not equal to right
 */
template< typename T >
bool operator!=(const Vector2D< T >& left, const Vector2D< T >& right);

template< typename T >
inline Vector2D< T >::Vector2D() : x(0), y(0) {}

template< typename T >
inline Vector2D< T >::Vector2D(T X, T Y) : x(X), y(Y) {}

template< typename T >
template< typename U >
inline Vector2D< T >::Vector2D(const Vector2D< U >& vector)
    : x(static_cast< T >(vector.x)), y(static_cast< T >(vector.y)) {}

template< typename T >
inline Vector2D< T > operator-(const Vector2D< T >& right) {
    return Vector2D< T >(-right.x, -right.y);
}

template< typename T >
inline Vector2D< T >& operator+=(Vector2D< T >& left,
                                 const Vector2D< T >& right) {
    left.x += right.x;
    left.y += right.y;

    return left;
}

template< typename T >
inline Vector2D< T >& operator-=(Vector2D< T >& left,
                                 const Vector2D< T >& right) {
    left.x -= right.x;
    left.y -= right.y;

    return left;
}

template< typename T >
inline Vector2D< T > operator+(const Vector2D< T >& left,
                               const Vector2D< T >& right) {
    return Vector2D< T >(left.x + right.x, left.y + right.y);
}

template< typename T >
inline Vector2D< T > operator-(const Vector2D< T >& left,
                               const Vector2D< T >& right) {
    return Vector2D< T >(left.x - right.x, left.y - right.y);
}

template< typename T >
inline Vector2D< T > operator*(const Vector2D< T >& left, T right) {
    return Vector2D< T >(left.x * right, left.y * right);
}

template< typename T >
inline Vector2D< T > operator*(T left, const Vector2D< T >& right) {
    return Vector2D< T >(right.x * left, right.y * left);
}

template< typename T >
inline Vector2D< T >& operator*=(Vector2D< T >& left, T right) {
    left.x *= right;
    left.y *= right;

    return left;
}

template< typename T >
inline Vector2D< T > operator/(const Vector2D< T >& left, T right) {
    return Vector2D< T >(left.x / right, left.y / right);
}

template< typename T >
inline Vector2D< T >& operator/=(Vector2D< T >& left, T right) {
    left.x /= right;
    left.y /= right;

    return left;
}

template< typename T >
inline bool operator==(const Vector2D< T >& left, const Vector2D< T >& right) {
    return (left.x == right.x) && (left.y == right.y);
}

template< typename T >
inline bool operator!=(const Vector2D< T >& left, const Vector2D< T >& right) {
    return (left.x != right.x) || (left.y != right.y);
}

/// You generally don't have to care about the templated form (Vector2D<T>),
/// the most common specializations have special typedefs:
typedef Vector2D< int > Vector2Di;
typedef Vector2D< unsigned int > Vector2Du;
typedef Vector2D< float > Vector2Df;

#endif  // VECTOR2D_HPP_
