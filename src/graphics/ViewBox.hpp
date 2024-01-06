#ifndef VIEWBOX_HPP_
#define VIEWBOX_HPP_

class ViewBox {
public:
    /**
     * @brief Default constructor
     *
     * Creates a ViewBox(0, 0, 0, 0).
     */
    ViewBox();

    /**
     * @brief Construct the ViewBox from its coordinates
     *
     * @param X X coordinate
     * @param Y Y coordinate
     * @param W Width
     * @param H Height
     */
    ViewBox(float X, float Y, float W, float H);

    /**
     * @brief Get the X coordinate of the ViewBox
     *
     * @return X coordinate of the ViewBox
     */
    float getX() const;

    /**
     * @brief Get the Y coordinate of the ViewBox
     *
     * @return Y coordinate of the ViewBox
     */
    float getY() const;

    /**
     * @brief Get the width of the ViewBox
     *
     * @return Width of the ViewBox
     */
    float getWidth() const;

    /**
     * @brief Get the height of the ViewBox
     *
     * @return Height of the ViewBox
     */
    float getHeight() const;

private:
    float x;  ///< X coordinate of the ViewBox
    float y;  ///< Y coordinate of the ViewBox
    float w;  ///< Width of the ViewBox
    float h;  ///< Height of the ViewBox
};

#endif  // VIEWBOX_HPP_