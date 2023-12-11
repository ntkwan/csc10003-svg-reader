#ifndef mColor_HPP_
#define mColor_HPP_

#include <iostream>
#include <map>
#include <string>

/**
 * @brief Utility class for manipulating RGBA mColors
 */
class mColor {
public:
    /**
     * @brief Default constructor
     *
     * Constructs an opaque black mColor. It is equivalent to
     * mColor(0, 0, 0, 255).
     */
    mColor();

    /**
     * @brief Construct the mColor from its 4 RGBA components
     *
     * @param red Red component (in the range [0, 255])
     * @param green Green component (in the range [0, 255])
     * @param blue Blue component (in the range [0, 255])
     * @param alpha Alpha (opacity) component (in the range [0, 255])
     */
    mColor(int red, int green, int blue, int alpha = 255);

    /**
     * @brief Construct the color from 32-bit unsigned integer
     *
     * @param color Number containing the RGBA components (in that order)
     */
    explicit mColor(int color);

    /**
     * @brief Prints the color
     *
     * @param os output stream
     * @param color color to be printed
     *
     * @return output stream
     *
     * @note This function is used for printing the color.
     */
    friend std::ostream& operator<<(std::ostream& os, const mColor& color);

    static const mColor Black;        ///< Black predefined color
    static const mColor White;        ///< White predefined color
    static const mColor Red;          ///< Red predefined color
    static const mColor Green;        ///< Green predefined color
    static const mColor Blue;         ///< Blue predefined color
    static const mColor Yellow;       ///< Yellow predefined color
    static const mColor Magenta;      ///< Magenta predefined color
    static const mColor Cyan;         ///< Cyan predefined color
    static const mColor Transparent;  ///< Transparent (black) predefined color

    int r;  ///< Red component
    int g;  ///< Green component
    int b;  ///< Blue component
    int a;  ///< Alpha (opacity) component
};

/**
 * @brief Represents a set of named colors.
 *
 * The Color class provides a map of named colors using the SFML Color
 * class.
 */
static const std::map< std::string, mColor > color_map = {
    {"aliceblue", mColor(240, 248, 255)},
    {"antiquewhite", mColor(250, 235, 215)},
    {"aqua", mColor(0, 255, 255)},
    {"aquamarine", mColor(127, 255, 212)},
    {"azure", mColor(240, 255, 255)},
    {"beige", mColor(245, 245, 220)},
    {"bisque", mColor(255, 228, 196)},
    {"black", mColor(0, 0, 0)},
    {"blanchedalmond", mColor(255, 235, 205)},
    {"blue", mColor(0, 0, 255)},
    {"blueviolet", mColor(138, 43, 226)},
    {"brown", mColor(165, 42, 42)},
    {"burlywood", mColor(222, 184, 135)},
    {"cadetblue", mColor(95, 158, 160)},
    {"chartreuse", mColor(127, 255, 0)},
    {"chocolate", mColor(210, 105, 30)},
    {"coral", mColor(255, 127, 80)},
    {"cornflowerblue", mColor(100, 149, 237)},
    {"cornsilk", mColor(255, 248, 220)},
    {"crimson", mColor(220, 20, 60)},
    {"cyan", mColor(0, 255, 255)},
    {"darkblue", mColor(0, 0, 139)},
    {"darkcyan", mColor(0, 139, 139)},
    {"darkgoldenrod", mColor(184, 134, 11)},
    {"darkgray", mColor(169, 169, 169)},
    {"darkgreen", mColor(0, 100, 0)},
    {"darkgrey", mColor(169, 169, 169)},
    {"darkkhaki", mColor(189, 183, 107)},
    {"darkmagenta", mColor(139, 0, 139)},
    {"darkolivegreen", mColor(85, 107, 47)},
    {"darkorange", mColor(255, 140, 0)},
    {"darkorchid", mColor(153, 50, 204)},
    {"darkred", mColor(139, 0, 0)},
    {"darksalmon", mColor(233, 150, 122)},
    {"darkseagreen", mColor(143, 188, 143)},
    {"darkslateblue", mColor(72, 61, 139)},
    {"darkslategray", mColor(47, 79, 79)},
    {"darkslategrey", mColor(47, 79, 79)},
    {"darkturquoise", mColor(0, 206, 209)},
    {"darkviolet", mColor(148, 0, 211)},
    {"deeppink", mColor(255, 20, 147)},
    {"deepskyblue", mColor(0, 191, 255)},
    {"dimgray", mColor(105, 105, 105)},
    {"dimgrey", mColor(105, 105, 105)},
    {"dodgerblue", mColor(30, 144, 255)},
    {"firebrick", mColor(178, 34, 34)},
    {"floralwhite", mColor(255, 250, 240)},
    {"forestgreen", mColor(34, 139, 34)},
    {"fuchsia", mColor(255, 0, 255)},
    {"gainsboro", mColor(220, 220, 220)},
    {"ghostwhite", mColor(248, 248, 255)},
    {"gold", mColor(255, 215, 0)},
    {"goldenrod", mColor(218, 165, 32)},
    {"gray", mColor(128, 128, 128)},
    {"green", mColor(0, 128, 0)},
    {"greenyellow", mColor(173, 255, 47)},
    {"grey", mColor(128, 128, 128)},
    {"honeydew", mColor(240, 255, 240)},
    {"hotpink", mColor(255, 105, 180)},
    {"indianred", mColor(205, 92, 92)},
    {"indigo", mColor(75, 0, 130)},
    {"ivory", mColor(255, 255, 240)},
    {"khaki", mColor(240, 230, 140)},
    {"lavender", mColor(230, 230, 250)},
    {"lavenderblush", mColor(255, 240, 245)},
    {"lawngreen", mColor(124, 252, 0)},
    {"lemonchiffon", mColor(255, 250, 205)},
    {"lightblue", mColor(173, 216, 230)},
    {"lightcoral", mColor(240, 128, 128)},
    {"lightcyan", mColor(224, 255, 255)},
    {"lightgoldenrodyellow", mColor(250, 250, 210)},
    {"lightgray", mColor(211, 211, 211)},
    {"lightgreen", mColor(144, 238, 144)},
    {"lightgrey", mColor(211, 211, 211)},
    {"lightpink", mColor(255, 182, 193)},
    {"lightsalmon", mColor(255, 160, 122)},
    {"lightseagreen", mColor(32, 178, 170)},
    {"lightskyblue", mColor(135, 206, 250)},
    {"lightslategray", mColor(119, 136, 153)},
    {"lightslategrey", mColor(119, 136, 153)},
    {"lightsteelblue", mColor(176, 196, 222)},
    {"lightyellow", mColor(255, 255, 224)},
    {"lime", mColor(0, 255, 0)},
    {"limegreen", mColor(50, 205, 50)},
    {"linen", mColor(250, 240, 230)},
    {"magenta", mColor(255, 0, 255)},
    {"maroon", mColor(128, 0, 0)},
    {"mediumaquamarine", mColor(102, 205, 170)},
    {"mediumblue", mColor(0, 0, 205)},
    {"mediumorchid", mColor(186, 85, 211)},
    {"mediumpurple", mColor(147, 112, 219)},
    {"mediumseagreen", mColor(60, 179, 113)},
    {"mediumslateblue", mColor(123, 104, 238)},
    {"mediumspringgreen", mColor(0, 250, 154)},
    {"mediumturquoise", mColor(72, 209, 204)},
    {"mediumvioletred", mColor(199, 21, 133)},
    {"midnightblue", mColor(25, 25, 112)},
    {"mintcream", mColor(245, 255, 250)},
    {"mistyrose", mColor(255, 228, 225)},
    {"moccasin", mColor(255, 228, 181)},
    {"navajowhite", mColor(255, 222, 173)},
    {"navy", mColor(0, 0, 128)},
    {"oldlace", mColor(253, 245, 230)},
    {"olive", mColor(128, 128, 0)},
    {"olivedrab", mColor(107, 142, 35)},
    {"orange", mColor(255, 165, 0)},
    {"orangered", mColor(255, 69, 0)},
    {"orchid", mColor(218, 112, 214)},
    {"palegoldenrod", mColor(238, 232, 170)},
    {"palegreen", mColor(152, 251, 152)},
    {"paleturquoise", mColor(175, 238, 238)},
    {"palevioletred", mColor(219, 112, 147)},
    {"papayawhip", mColor(255, 239, 213)},
    {"peachpuff", mColor(255, 218, 185)},
    {"peru", mColor(205, 133, 63)},
    {"pink", mColor(255, 192, 203)},
    {"plum", mColor(221, 160, 221)},
    {"powderblue", mColor(176, 224, 230)},
    {"purple", mColor(128, 0, 128)},
    {"red", mColor(255, 0, 0)},
    {"rosybrown", mColor(188, 143, 143)},
    {"royalblue", mColor(65, 105, 225)},
    {"saddlebrown", mColor(139, 69, 19)},
    {"salmon", mColor(250, 128, 114)},
    {"sandybrown", mColor(244, 164, 96)},
    {"seagreen", mColor(46, 139, 87)},
    {"seashell", mColor(255, 245, 238)},
    {"sienna", mColor(160, 82, 45)},
    {"silver", mColor(192, 192, 192)},
    {"skyblue", mColor(135, 206, 235)},
    {"slateblue", mColor(106, 90, 205)},
    {"slategray", mColor(112, 128, 144)},
    {"slategrey", mColor(112, 128, 144)},
    {"snow", mColor(255, 250, 250)},
    {"springgreen", mColor(0, 255, 127)},
    {"steelblue", mColor(70, 130, 180)},
    {"tan", mColor(210, 180, 140)},
    {"teal", mColor(0, 128, 128)},
    {"thistle", mColor(216, 191, 216)},
    {"tomato", mColor(255, 99, 71)},
    {"transparent", mColor(0, 0, 0, 0)},
    {"turquoise", mColor(64, 224, 208)},
    {"violet", mColor(238, 130, 238)},
    {"wheat", mColor(245, 222, 179)},
    {"white", mColor(255, 255, 255)},
    {"whitesmoke", mColor(245, 245, 245)},
    {"yellow", mColor(255, 255, 0)},
    {"yellowgreen", mColor(154, 205, 50)}};

#endif