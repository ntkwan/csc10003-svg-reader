#ifndef PARSER2_HPP_
#define PARSER2_HPP_

#include <string>
#include <vector>

#include "Graphics.hpp"
#include "Renderer.hpp"

typedef std::vector< std::pair< std::string, std::string > > Attributes, Tags;
typedef std::vector< std::pair< std::string, Attributes > > Objects;

class Parser {
public:
    static Parser* getInstance(const std::string& file_name);

    Parser(const Parser&) = delete;

    ~Parser();

    void printObjects();

    void renderSVG(Renderer& renderer);

    void printShapesData();

private:
    /**
     * @brief Construct a new Parser object.
     *
     * @param file_name The name of the file to be parsed.
     */
    Parser(const std::string& file_name);

    std::string parseSVG(const std::string& filePath);

    Attributes parseAttributes(std::string attributes);

    Tags parseTags(std::string svg);

    void parseObjects(std::string filePath);

    std::string getAttribute(Attributes attributes, std::string name);

    float getFloatAttribute(Attributes attributes, std::string name);

    Color parseColor(Attributes attributes, std::string color);

    std::vector< Vector2Df > parsePoints(Attributes attributes);

    std::vector< PathPoint > parsePathPoints(Attributes attributes);

    std::vector< std::string > getTransformOrder(Attributes attributes);

    std::pair< float, float > getTranslate(std::string transform_value);

    float getRotate(std::string transform_value);

    void applyTransform(Shape* shape,
                        const std::vector< std::string >& transform_order);

    void parseLine(Attributes attributes);

    void parseRect(Attributes attributes);

    void parsePolyline(Attributes attributes);

    void parsePolygon(Attributes attributes);

    void parseCircle(Attributes attributes);

    void parseEllipse(Attributes attributes);

    void parsePath(Attributes attributes);

    void parseText(Attributes attributes);

    void parseSVG();

private:
    static Parser* instance;  ///< The instance of the Parser.
    Objects objects;
    std::vector< Shape* > shapes;
};

#endif  // PARSER2_HPP_