#ifndef TEXT_HPP_
#define TEXT_HPP_

#include "SVGElement.hpp"

/**
 * @brief Represents text in 2D space.
 *
 * The Text class is derived from the SVGElement class and defines a text
 * element with a specified position, string, fill color, and font size.
 */
class Text : public SVGElement {
private:
    std::string content;  ///< Text element
    float font_size;      ///< Font size of the text
    std::string anchor;   ///< Anchor of the text
    std::string style;    ///< Style of the text

public:
    /**
     * @brief Constructs a Text object.
     *
     * @param pos The position of the text.
     * @param text The string of the text.
     * @param fill The fill color of the text
     * @param font_size The font size of the text (default is 1).
     */
    Text(Vector2Df pos, std::string text, float font_size,
         const ColorShape &fill, const ColorShape &stroke, float stroke_width);

    /**
     * @brief Gets the type of the shape.
     *
     * @return The string "Text".
     */
    std::string getClass() const override;

    /**
     * @brief Sets the string of the text.
     *
     * @param content The new string of the text.
     */
    void setContent(std::string content);

    /**
     * @brief Gets the string of the text.
     *
     * @return The string of the text.
     */
    std::string getContent() const;

    /**
     * @brief Sets the font size of the text.
     *
     * @param font_size The new font size of the text.
     */
    void setFontSize(float font_size);

    /**
     * @brief Gets the font size of the text.
     *
     * @return The font size of the text.
     */
    float getFontSize() const;

    /**
     * @brief Sets the anchor of the text.
     *
     * @param anchor The new anchor of the text.
     */
    void setAnchor(std::string anchor);

    /**
     * @brief Gets the anchor of the text.
     *
     * @return The anchor of the text.
     */
    std::string getAnchor() const;

    /**
     * @brief Sets the style of the text.
     *
     * @param style The new style of the text.
     */
    void setFontStyle(std::string style);

    /**
     * @brief Gets the style of the text.
     *
     * @return The style of the text.
     */
    std::string getFontStyle() const;

    /**
     * @brief Prints the data of the text.
     */
    void printData() const override;
};

#endif  // TEXT_HPP_