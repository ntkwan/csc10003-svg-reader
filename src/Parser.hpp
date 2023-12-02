#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <string>
#include <vector>

#include "Graphics.hpp"

typedef std::vector< std::pair< std::string, std::string > > Attributes, Tags;
typedef std::vector< std::pair< std::string, Attributes > > Objects;

class Parser {
public:
    static Parser* getInstance(const std::string& file_name);

    Parser(const Parser&) = delete;

    ~Parser();

    void printObjects();

    void printShapesData();

    Group* getRoot();

private:
    /**
     * @brief Construct a new Parser object.
     *
     * @param file_name The name of the file to be parsed.
     */
    Parser(const std::string& file_name);

    std::string parseSVG(const std::string& file_name);

    Attributes parseAttributes(std::string attributes);

    Tags parseTags(std::string svg);

    SVGElement* parseElements(std::string file_name);

    std::string getAttribute(Attributes attributes, std::string name);

    float getFloatAttribute(Attributes attributes, std::string name);

    Color parseColor(Attributes attributes, std::string color);

    std::vector< Vector2Df > parsePoints(Attributes attributes);

    std::vector< PathPoint > parsePathPoints(Attributes attributes);

    std::vector< std::string > getTransformOrder(Attributes attributes);

    Line* parseLine(Attributes attributes);

    Rect* parseRect(Attributes attributes);

    class Polyline* parsePolyline(Attributes attributes);

    class Polygon* parsePolygon(Attributes attributes);

    Circle* parseCircle(Attributes attributes);

    class Ellipse* parseEllipse(Attributes attributes);

    Path* parsePath(Attributes attributes);

    Text* parseText(Attributes attributes);

    SVGElement* parseShape(std::string shape, Attributes attributes);

private:
    static Parser* instance;  ///< The instance of the Parser.
    Objects objects;          ///< The objects in the SVG file.
    SVGElement* root;         ///< The root of the SVG file.
};

#endif  // PARSER_HPP_