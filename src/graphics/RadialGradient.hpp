#ifndef RADIAL_GRADIENT_HPP_
#define RADIAL_GRADIENT_HPP_

#include "Gradient.hpp"

/**
 * @brief A class that represents a radial gradient.
 *
 * The RadialGradient class is derived from the Gradient class and represents a
 * radial gradient. It contains a vector of Stop objects that represent the the
 * stops of the gradient. It also contains a pair of Vector2D objects that
 * represent the start and end points of the gradient.
 */
class RadialGradient : public Gradient {
public:
    /**
     * @brief Constructs a RadialGradient object.
     *
     * @param stops The stops of the gradient.
     * @param points The start and end points of the gradient.
     * @param radius The radius of the gradient.
     */
    RadialGradient(std::vector< Stop > stops,
                   std::pair< Vector2Df, Vector2Df > points, Vector2Df radius);

    /**
     * @brief Gets the type of the gradient.
     *
     * @return The string "RadialGradient".
     * @note This function is used for determining the type of the gradient.
     */
    std::string getClass() const override;

private:
    Vector2Df radius;  ///< The radius of the gradient.
};

#endif