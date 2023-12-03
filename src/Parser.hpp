#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <string>
#include <vector>

#include "../external/rapidxml/rapidxml.hpp"
#include "Graphics.hpp"

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

    Color parseColor(xml_node<>* node, std::string color);

    std::vector< Vector2Df > parsePoints(xml_node<>* node);

    std::vector< PathPoint > parsePathPoints(xml_node<>* node);

    std::vector< std::string > getTransformOrder(xml_node<>* node);

    Line* parseLine(xml_node<>* node);

    Rect* parseRect(xml_node<>* node);

    class Polyline* parsePolyline(xml_node<>* node);

    class Polygon* parsePolygon(xml_node<>* node);

    Circle* parseCircle(xml_node<>* node);

    class Ellipse* parseEllipse(xml_node<>* node);

    Path* parsePath(xml_node<>* node);

    Text* parseText(xml_node<>* node);

    SVGElement* parseShape(xml_node<>* node);

private:
    static Parser* instance;  ///< The instance of the Parser.
    SVGElement* root;         ///< The root of the SVG file.
};

#endif  // PARSER_HPP_