#ifndef TEXT_HPP_
#define TEXT_HPP_

#include "Shape.hpp"

/**
 * @brief Represents text in 2D space.
 *
 * The Text class is derived from the Shape class and defines a text element
 * with a specified position, string, fill color, and font size.
 */
class Text : public Shape {
private:
    std::string text;  ///< Text element
    Vector2Df position;

public:
    /**
     * @brief Constructs a Text object.
     *
     * @param pos The position of the text.
     * @param TEXT The string of the text.
     * @param fill_color The fill color of the text (default is
     * sf::Color::Black).
     * @param font_size The font size of the text (default is 1).
     */
    Text(Vector2Df pos, std::string TEXT, Color fill_color = Color::Black,
         float font_size = 1);

    /**
     * @brief Gets the total number of points representing the text.
     *
     * Since text is not represented by points, this method always returns 0.
     *
     * @return The number of points representing the text.
     */
    virtual std::size_t getPointCount() const;

    /**
     * @brief Gets a dummy point for compatibility with Shape interface.
     *
     * Since text is not represented by points, this method always returns (0,
     * 0).
     *
     * @param index The index of the dummy point (ignored).
     * @return A dummy point for compatibility.
     */
    virtual Vector2Df getPoint(std::size_t index) const;

    /**
     * @brief Draws the text on the specified render target.
     *
     * @param target The render target to draw on.
     * @param states The render states to apply (default is
     * sf::RenderStates::Default).
     */
    void draw(Renderer& target) const;
};

#endif  // TEXT_HPP_