#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../external/rapidxml/rapidxml.hpp"
#include "Graphics.hpp"

typedef std::vector< std::pair< std::string, std::string > > Attributes;

/**
 * @brief To manipulate and parse an SVG file.
 *
 * The Parser class is a singleton class that is used to parse an SVG file and
 * create a tree of SVGElements.
 */
class Parser {
public:
    static Parser* getInstance(
        const std::string&
            file_name);  ///< Gets the singleton instance of the Parser class.

    Parser(const Parser&) =
        delete;  ///< Deleted copy constructor to enforce the singleton pattern.

    ~Parser();  ///< Destructor.

    Group* getRoot();  ///< Gets the root of the SVG file.

    /**
     * @brief Prints the data of the shapes.
     *
     * @note This function is used for debugging.
     */
    void printShapesData();

private:
    /**
     * @brief Construct a new Parser object.
     *
     * @param file_name The name of the file to be parsed.
     */
    Parser(const std::string& file_name);

    /**
     * @brief Parses the SVG file and creates a tree of SVGElements.
     *
     * @param file_name The name of the file to be parsed.
     *
     * @return The root of the tree of SVGElements.
     */
    SVGElement* parseElements(std::string file_name);

    /**
     * @brief Gets the attributes of a node.
     *
     * @param node The node to be parsed.
     * @param name The name of tag to be parsed.
     *
     * @return The attributes of the node.
     */
    std::string getAttribute(rapidxml::xml_node<>* node, std::string name);

    /**
     * @brief Gets the floating point attributes of a node.
     *
     * @param node The node to be parsed.
     * @param name The name of tag to be parsed.
     *
     * @return The floating point attributes of the node.
     */
    float getFloatAttribute(rapidxml::xml_node<>* node, std::string name);

    /**
     * @brief Gets the gradient stops of a node.
     *
     * @param node The node to be parsed.
     * @return The gradient stops of the node.
     */
    std::vector< Stop > getGradientStops(rapidxml::xml_node<>* node);

    /**
     * @brief Gets the gradients of a node.
     *
     * @param node The node to be parsed.
     */
    void GetGradients(rapidxml::xml_node<>* node);

    /**
     * @brief Gets the gradient of a node.
     *
     * @param id The id of the gradient to be parsed.
     * @return The gradient of the node.
     */
    Gradient* parseGradient(std::string id);

    /**
     * @brief Gets the color attributes of a node.
     *
     * @param node The node to be parsed.
     * @param color The name of the color tag to be parsed.
     * @param id The id to check if the color is a reference.
     * @return The color attributes of the node.
     */
    mColor parseColor(rapidxml::xml_node<>* node, std::string color,
                      std::string& id);

    /**
     * @brief Gets the points of the element
     *
     * @param node The node to be parsed.
     * @return The points of the element
     */
    std::vector< Vector2Df > parsePoints(rapidxml::xml_node<>* node);

    /**
     * @brief Gets the points of the path element
     *
     * @param node The node to be parsed.
     * @return The points of the path element
     */
    std::vector< PathPoint > parsePathPoints(rapidxml::xml_node<>* node);

    /**
     * @brief Gets the transform order of the element
     *
     * @param node The node to be parsed.
     * @return The transform order of the element
     */
    std::vector< std::string > getTransformOrder(rapidxml::xml_node<>* node);

    /**
     * @brief Parses the line element
     *
     * @param node The node to be parsed.
     * @param stroke_color The color of the stroke
     * @param stroke_width The width of the stroke
     * @return The line element
     */
    Line* parseLine(rapidxml::xml_node<>* node, const mColor& stroke_color,
                    float stroke_width);

    /**
     * @brief Parses the rect element
     *
     * @param node The node to be parsed.
     * @param fill_color The color of the fill
     * @param stroke_color The color of the stroke
     * @param stroke_width The width of the stroke
     * @return The rect element
     */
    Rect* parseRect(rapidxml::xml_node<>* node, const mColor& fill_color,
                    const mColor& stroke_color, float stroke_width);

    /**
     * @brief Parses the polyline element
     *
     * @param node The node to be parsed.
     * @param fill_color The color of the fill
     * @param stroke_color The color of the stroke
     * @param stroke_width The width of the stroke
     * @return The polyline element
     */
    class Plyline* parsePolyline(rapidxml::xml_node<>* node,
                                 const mColor& fill_color,
                                 const mColor& stroke_color,
                                 float stroke_width);

    /**
     * @brief Parses the polygon element
     *
     * @param node The node to be parsed.
     * @param fill_color The color of the fill
     * @param stroke_color The color of the stroke
     * @param stroke_width The width of the stroke
     * @return The polygon element
     */
    class Plygon* parsePolygon(rapidxml::xml_node<>* node,
                               const mColor& fill_color,
                               const mColor& stroke_color, float stroke_width);

    /**
     * @brief Parses the circle element
     *
     * @param node The node to be parsed.
     * @param fill_color The color of the fill
     * @param stroke_color The color of the stroke
     * @param stroke_width The width of the stroke
     * @return The circle element
     */
    Circle* parseCircle(rapidxml::xml_node<>* node, const mColor& fill_color,
                        const mColor& stroke_color, float stroke_width);

    /**
     * @brief Parses the ellipse element
     *
     * @param node The node to be parsed.
     * @param fill_color The color of the fill
     * @param stroke_color The color of the stroke
     * @param stroke_width The width of the stroke
     * @return The ellipse element
     */
    class Ell* parseEllipse(rapidxml::xml_node<>* node,
                            const mColor& fill_color,
                            const mColor& stroke_color, float stroke_width);

    /**
     * @brief Parses the path element
     *
     * @param node The node to be parsed.
     * @param fill_color The color of the fill
     * @param stroke_color The color of the stroke
     * @param stroke_width The width of the stroke
     * @return The path element
     */
    Path* parsePath(rapidxml::xml_node<>* node, const mColor& fill_color,
                    const mColor& stroke_color, float stroke_width);

    /**
     * @brief Parses the text element
     * @param node The node to be parsed.
     * @param fill_color The color of the fill
     * @param stroke_color The color of the stroke
     * @param stroke_width The width of the stroke
     * @return The text element
     */
    Text* parseText(rapidxml::xml_node<>* node, const mColor& fill_color,
                    const mColor& stroke_color, float stroke_width);

    /**
     * @brief Parses the group of elements
     *
     * @param node The node to be parsed.
     * @return The group of elements
     */
    SVGElement* parseShape(rapidxml::xml_node<>* node);

private:
    static Parser* instance;  ///< The instance of the Parser.
    SVGElement* root;         ///< The root of the SVG file.
    std::map< std::string, Gradient* > gradients;  ///< The gradients of the SVG
                                                   ///< file.
    std::pair< Vector2Df, Vector2Df >
        viewBox;  ///< The viewbox of the SVG file.
};

#endif  // PARSER_HPP_