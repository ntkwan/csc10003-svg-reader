#ifndef ColorShape_HPP_
#define ColorShape_HPP_

#include <iostream>
#include <map>
#include <string>

/**
 * @brief Utility class for manipulating RGBA ColorShapes
 */
class ColorShape {
public:
    /**
     * @brief Default constructor
     *
     * Constructs an opaque black ColorShape. It is equivalent to
     * ColorShape(0, 0, 0, 255).
     */
    ColorShape();

    /**
     * @brief Construct the ColorShape from its 4 RGBA components
     *
     * @param red Red component (in the range [0, 255])
     * @param green Green component (in the range [0, 255])
     * @param blue Blue component (in the range [0, 255])
     * @param alpha Alpha (opacity) component (in the range [0, 255])
     */
    ColorShape(int red, int green, int blue, int alpha = 255);

    /**
     * @brief Construct the color from 32-bit unsigned integer
     *
     * @param color Number containing the RGBA components (in that order)
     */
    explicit ColorShape(int color);

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
    friend std::ostream& operator<<(std::ostream& os, const ColorShape& color);

    static const ColorShape Black;    ///< Black predefined color
    static const ColorShape White;    ///< White predefined color
    static const ColorShape Red;      ///< Red predefined color
    static const ColorShape Green;    ///< Green predefined color
    static const ColorShape Blue;     ///< Blue predefined color
    static const ColorShape Yellow;   ///< Yellow predefined color
    static const ColorShape Magenta;  ///< Magenta predefined color
    static const ColorShape Cyan;     ///< Cyan predefined color
    static const ColorShape
        Transparent;  ///< Transparent (black) predefined color

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
static const std::map< std::string, ColorShape > color_map = {
    {"aliceblue", ColorShape(240, 248, 255)},
    {"antiquewhite", ColorShape(250, 235, 215)},
    {"aqua", ColorShape(0, 255, 255)},
    {"aquamarine", ColorShape(127, 255, 212)},
    {"azure", ColorShape(240, 255, 255)},
    {"beige", ColorShape(245, 245, 220)},
    {"bisque", ColorShape(255, 228, 196)},
    {"black", ColorShape(0, 0, 0)},
    {"blanchedalmond", ColorShape(255, 235, 205)},
    {"blue", ColorShape(0, 0, 255)},
    {"blueviolet", ColorShape(138, 43, 226)},
    {"brown", ColorShape(165, 42, 42)},
    {"burlywood", ColorShape(222, 184, 135)},
    {"cadetblue", ColorShape(95, 158, 160)},
    {"chartreuse", ColorShape(127, 255, 0)},
    {"chocolate", ColorShape(210, 105, 30)},
    {"coral", ColorShape(255, 127, 80)},
    {"cornflowerblue", ColorShape(100, 149, 237)},
    {"cornsilk", ColorShape(255, 248, 220)},
    {"crimson", ColorShape(220, 20, 60)},
    {"cyan", ColorShape(0, 255, 255)},
    {"darkblue", ColorShape(0, 0, 139)},
    {"darkcyan", ColorShape(0, 139, 139)},
    {"darkgoldenrod", ColorShape(184, 134, 11)},
    {"darkgray", ColorShape(169, 169, 169)},
    {"darkgreen", ColorShape(0, 100, 0)},
    {"darkgrey", ColorShape(169, 169, 169)},
    {"darkkhaki", ColorShape(189, 183, 107)},
    {"darkmagenta", ColorShape(139, 0, 139)},
    {"darkolivegreen", ColorShape(85, 107, 47)},
    {"darkorange", ColorShape(255, 140, 0)},
    {"darkorchid", ColorShape(153, 50, 204)},
    {"darkred", ColorShape(139, 0, 0)},
    {"darksalmon", ColorShape(233, 150, 122)},
    {"darkseagreen", ColorShape(143, 188, 143)},
    {"darkslateblue", ColorShape(72, 61, 139)},
    {"darkslategray", ColorShape(47, 79, 79)},
    {"darkslategrey", ColorShape(47, 79, 79)},
    {"darkturquoise", ColorShape(0, 206, 209)},
    {"darkviolet", ColorShape(148, 0, 211)},
    {"deeppink", ColorShape(255, 20, 147)},
    {"deepskyblue", ColorShape(0, 191, 255)},
    {"dimgray", ColorShape(105, 105, 105)},
    {"dimgrey", ColorShape(105, 105, 105)},
    {"dodgerblue", ColorShape(30, 144, 255)},
    {"firebrick", ColorShape(178, 34, 34)},
    {"floralwhite", ColorShape(255, 250, 240)},
    {"forestgreen", ColorShape(34, 139, 34)},
    {"fuchsia", ColorShape(255, 0, 255)},
    {"gainsboro", ColorShape(220, 220, 220)},
    {"ghostwhite", ColorShape(248, 248, 255)},
    {"gold", ColorShape(255, 215, 0)},
    {"goldenrod", ColorShape(218, 165, 32)},
    {"gray", ColorShape(128, 128, 128)},
    {"green", ColorShape(0, 128, 0)},
    {"greenyellow", ColorShape(173, 255, 47)},
    {"grey", ColorShape(128, 128, 128)},
    {"honeydew", ColorShape(240, 255, 240)},
    {"hotpink", ColorShape(255, 105, 180)},
    {"indianred", ColorShape(205, 92, 92)},
    {"indigo", ColorShape(75, 0, 130)},
    {"ivory", ColorShape(255, 255, 240)},
    {"khaki", ColorShape(240, 230, 140)},
    {"lavender", ColorShape(230, 230, 250)},
    {"lavenderblush", ColorShape(255, 240, 245)},
    {"lawngreen", ColorShape(124, 252, 0)},
    {"lemonchiffon", ColorShape(255, 250, 205)},
    {"lightblue", ColorShape(173, 216, 230)},
    {"lightcoral", ColorShape(240, 128, 128)},
    {"lightcyan", ColorShape(224, 255, 255)},
    {"lightgoldenrodyellow", ColorShape(250, 250, 210)},
    {"lightgray", ColorShape(211, 211, 211)},
    {"lightgreen", ColorShape(144, 238, 144)},
    {"lightgrey", ColorShape(211, 211, 211)},
    {"lightpink", ColorShape(255, 182, 193)},
    {"lightsalmon", ColorShape(255, 160, 122)},
    {"lightseagreen", ColorShape(32, 178, 170)},
    {"lightskyblue", ColorShape(135, 206, 250)},
    {"lightslategray", ColorShape(119, 136, 153)},
    {"lightslategrey", ColorShape(119, 136, 153)},
    {"lightsteelblue", ColorShape(176, 196, 222)},
    {"lightyellow", ColorShape(255, 255, 224)},
    {"lime", ColorShape(0, 255, 0)},
    {"limegreen", ColorShape(50, 205, 50)},
    {"linen", ColorShape(250, 240, 230)},
    {"magenta", ColorShape(255, 0, 255)},
    {"maroon", ColorShape(128, 0, 0)},
    {"mediumaquamarine", ColorShape(102, 205, 170)},
    {"mediumblue", ColorShape(0, 0, 205)},
    {"mediumorchid", ColorShape(186, 85, 211)},
    {"mediumpurple", ColorShape(147, 112, 219)},
    {"mediumseagreen", ColorShape(60, 179, 113)},
    {"mediumslateblue", ColorShape(123, 104, 238)},
    {"mediumspringgreen", ColorShape(0, 250, 154)},
    {"mediumturquoise", ColorShape(72, 209, 204)},
    {"mediumvioletred", ColorShape(199, 21, 133)},
    {"midnightblue", ColorShape(25, 25, 112)},
    {"mintcream", ColorShape(245, 255, 250)},
    {"mistyrose", ColorShape(255, 228, 225)},
    {"moccasin", ColorShape(255, 228, 181)},
    {"navajowhite", ColorShape(255, 222, 173)},
    {"navy", ColorShape(0, 0, 128)},
    {"oldlace", ColorShape(253, 245, 230)},
    {"olive", ColorShape(128, 128, 0)},
    {"olivedrab", ColorShape(107, 142, 35)},
    {"orange", ColorShape(255, 165, 0)},
    {"orangered", ColorShape(255, 69, 0)},
    {"orchid", ColorShape(218, 112, 214)},
    {"palegoldenrod", ColorShape(238, 232, 170)},
    {"palegreen", ColorShape(152, 251, 152)},
    {"paleturquoise", ColorShape(175, 238, 238)},
    {"palevioletred", ColorShape(219, 112, 147)},
    {"papayawhip", ColorShape(255, 239, 213)},
    {"peachpuff", ColorShape(255, 218, 185)},
    {"peru", ColorShape(205, 133, 63)},
    {"pink", ColorShape(255, 192, 203)},
    {"plum", ColorShape(221, 160, 221)},
    {"powderblue", ColorShape(176, 224, 230)},
    {"purple", ColorShape(128, 0, 128)},
    {"red", ColorShape(255, 0, 0)},
    {"rosybrown", ColorShape(188, 143, 143)},
    {"royalblue", ColorShape(65, 105, 225)},
    {"saddlebrown", ColorShape(139, 69, 19)},
    {"salmon", ColorShape(250, 128, 114)},
    {"sandybrown", ColorShape(244, 164, 96)},
    {"seagreen", ColorShape(46, 139, 87)},
    {"seashell", ColorShape(255, 245, 238)},
    {"sienna", ColorShape(160, 82, 45)},
    {"silver", ColorShape(192, 192, 192)},
    {"skyblue", ColorShape(135, 206, 235)},
    {"slateblue", ColorShape(106, 90, 205)},
    {"slategray", ColorShape(112, 128, 144)},
    {"slategrey", ColorShape(112, 128, 144)},
    {"snow", ColorShape(255, 250, 250)},
    {"springgreen", ColorShape(0, 255, 127)},
    {"steelblue", ColorShape(70, 130, 180)},
    {"tan", ColorShape(210, 180, 140)},
    {"teal", ColorShape(0, 128, 128)},
    {"thistle", ColorShape(216, 191, 216)},
    {"tomato", ColorShape(255, 99, 71)},
    {"transparent", ColorShape(0, 0, 0, 0)},
    {"turquoise", ColorShape(64, 224, 208)},
    {"violet", ColorShape(238, 130, 238)},
    {"wheat", ColorShape(245, 222, 179)},
    {"white", ColorShape(255, 255, 255)},
    {"whitesmoke", ColorShape(245, 245, 245)},
    {"yellow", ColorShape(255, 255, 0)},
    {"yellowgreen", ColorShape(154, 205, 50)}};

#endif  // ColorShape_HPP_