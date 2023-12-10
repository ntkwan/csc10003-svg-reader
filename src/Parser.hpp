#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../external/rapidxml/rapidxml.hpp"
#include "graphics/Circle.hpp"
#include "graphics/Color.hpp"
#include "graphics/Ellipse.hpp"
#include "graphics/Group.hpp"
#include "graphics/Line.hpp"
#include "graphics/Path.hpp"
#include "graphics/Polygon.hpp"
#include "graphics/Polyline.hpp"
#include "graphics/Rect.hpp"
#include "graphics/Text.hpp"

using namespace rapidxml;

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
    std::string getAttribute(xml_node<>* node, std::string name);

    /**
     * @brief Gets the floating point attributes of a node.
     *
     * @param node The node to be parsed.
     * @param name The name of tag to be parsed.
     *
     * @return The floating point attributes of the node.
     */
    float getFloatAttribute(xml_node<>* node, std::string name);

    /**
     * @brief Gets the color attributes of a node.
     *
     * @param node The node to be parsed.
     * @param color The name of the color tag to be parsed.
     *
     * @return The color attributes of the node.
     */
    mColor parseColor(xml_node<>* node, std::string color);

    /**
     * @brief Gets the points of the element
     *
     * @param node The node to be parsed.
     * @return The points of the element
     */
    std::vector< Vector2Df > parsePoints(xml_node<>* node);

    /**
     * @brief Gets the points of the path element
     *
     * @param node The node to be parsed.
     * @return The points of the path element
     */
    std::vector< PathPoint > parsePathPoints(xml_node<>* node);

    /**
     * @brief Gets the transform order of the element
     *
     * @param node The node to be parsed.
     * @return The transform order of the element
     */
    std::vector< std::string > getTransformOrder(xml_node<>* node);

    /**
     * @brief Parses the line element
     *
     * @param node The node to be parsed.
     * @param stroke_color The color of the stroke
     * @param stroke_width The width of the stroke
     * @return The line element
     */
    Line* parseLine(xml_node<>* node, const mColor& stroke_color,
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
    Rect* parseRect(xml_node<>* node, const mColor& fill_color,
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
    class Plyline* parsePolyline(xml_node<>* node, const mColor& fill_color,
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
    class Plygon* parsePolygon(xml_node<>* node, const mColor& fill_color,
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
    Circle* parseCircle(xml_node<>* node, const mColor& fill_color,
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
    class Ell* parseEllipse(xml_node<>* node, const mColor& fill_color,
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
    Path* parsePath(xml_node<>* node, const mColor& fill_color,
                    const mColor& stroke_color, float stroke_width);

    /**
     * @brief Parses the text element
     * @param node The node to be parsed.
     * @param fill_color The color of the fill
     * @param stroke_color The color of the stroke
     * @param stroke_width The width of the stroke
     * @return The text element
     */
    Text* parseText(xml_node<>* node, const mColor& fill_color,
                    const mColor& stroke_color, float stroke_width);

    /**
     * @brief Parses the group of elements
     *
     * @param node The node to be parsed.
     * @return The group of elements
     */
    SVGElement* parseShape(xml_node<>* node);

private:
    static Parser* instance;  ///< The instance of the Parser.
    SVGElement* root;         ///< The root of the SVG file.
};

#endif  // PARSER_HPP_