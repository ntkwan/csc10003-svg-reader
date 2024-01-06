#ifndef STOP_HPP_
#define STOP_HPP_

#include "ColorShape.hpp"

/**
 * @brief A class that represents a stop.
 *
 * The Stop class represents a stop. It contains a color and an offset.
 */
class Stop {
public:
    /**
     * @brief Constructs a Stop object.
     *
     * @param color The color of the stop.
     * @param offset The offset of the stop.
     */
    Stop(const ColorShape& color, float offset);

    /**
     * @brief Gets the color of the stop.
     *
     * @return The color of the stop.
     */
    ColorShape getColor() const;

    /**
     * @brief Gets the offset of the stop.
     *
     * @return The offset of the stop.
     */
    float getOffset() const;

private:
    ColorShape color;  ///< The color of the stop.
    float offset;      ///< The offset of the stop.
};

#endif