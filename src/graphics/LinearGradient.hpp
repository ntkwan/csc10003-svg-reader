#ifndef LINEAR_GRADIENT_HPP_
#define LINEAR_GRADIENT_HPP_

#include "Gradient.hpp"

/**
 * @brief A class that represents a linear gradient.
 *
 * The LinearGradient class is derived from the Gradient class and represents a
 * linear gradient. It contains a vector of Stop objects that represent the the
 * stops of the gradient. It also contains a pair of Vector2D objects that
 * represent the start and end points of the gradient.
 */
class LinearGradient : public Gradient {
public:
    /**
     * @brief Constructs a LinearGradient object.
     *
     * @param stops The stops of the gradient.
     * @param points The start and end points of the gradient.
     */
    LinearGradient(std::vector< Stop > stops,
                   std::pair< Vector2Df, Vector2Df > points);

    /**
     * @brief Gets the type of the gradient.
     *
     * @return The string "LinearGradient".
     * @note This function is used for determining the type of the gradient.
     */
    std::string getClass() const override;
};

#endif