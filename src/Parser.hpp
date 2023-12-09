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

class Parser {
public:
    static Parser* getInstance(const std::string& file_name);

    Parser(const Parser&) = delete;

    ~Parser();

    Group* getRoot();

    void printShapesData();

private:
    /**
     * @brief Construct a new Parser object.
     *
     * @param file_name The name of the file to be parsed.
     */
    Parser(const std::string& file_name);

    SVGElement* parseElements(std::string file_name);

    std::string getAttribute(xml_node<>* node, std::string name);

    float getFloatAttribute(xml_node<>* node, std::string name);

    mColor parseColor(xml_node<>* node, std::string color);

    std::vector< Vector2Df > parsePoints(xml_node<>* node);

    std::vector< PathPoint > parsePathPoints(xml_node<>* node);

    std::vector< std::string > getTransformOrder(xml_node<>* node);

    Line* parseLine(xml_node<>* node, const mColor& stroke_color,
                    float stroke_width);

    Rect* parseRect(xml_node<>* node, const mColor& fill_color,
                    const mColor& stroke_color, float stroke_width);

    class Plyline* parsePolyline(xml_node<>* node, const mColor& fill_color,
                                 const mColor& stroke_color,
                                 float stroke_width);

    class Plygon* parsePolygon(xml_node<>* node, const mColor& fill_color,
                               const mColor& stroke_color, float stroke_width);

    Circle* parseCircle(xml_node<>* node, const mColor& fill_color,
                        const mColor& stroke_color, float stroke_width);

    class Ell* parseEllipse(xml_node<>* node, const mColor& fill_color,
                            const mColor& stroke_color, float stroke_width);

    Path* parsePath(xml_node<>* node, const mColor& fill_color,
                    const mColor& stroke_color, float stroke_width);

    Text* parseText(xml_node<>* node, const mColor& fill_color,
                    const mColor& stroke_color, float stroke_width);

    SVGElement* parseShape(xml_node<>* node);

private:
    static Parser* instance;  ///< The instance of the Parser.
    SVGElement* root;         ///< The root of the SVG file.
};

#endif  // PARSER_HPP_