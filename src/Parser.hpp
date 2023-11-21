#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "Graphics.hpp"
#include "Renderer.hpp"
#include "pugixml.hpp"

/**
 * @brief Represents a parser for SVG files.
 *
 * The parser class is responsible for parsing XML attributes (SVG files) and
 * passing this information to create shapes and render them.
 */
class Parser {
public:
    /**
     * @brief Gets the singleton instance of Parser.
     * @param file_name The name of the SVG file to parse.
     * @return The singleton instance of Parser.
     * @note This function is thread-safe.
     */
    static Parser* getInstance(const std::string& file_name);

    /**
     * @brief Deleted assignment operator to prevent copying of Parser
     * instances.
     * @return The Parser instance.
     * @note This function is deleted because Parser is a singleton class.
     */
    Parser(const Parser&) = delete;

    /**
     * @brief Get the Attribute object which is parsed from the XML file.
     * @param node The node of the XML file (pugi::xml_node is a typedef of
     * pugixml)
     * @param name The name of the attribute.
     * @return The attribute which is parsed from the XML file.
     * @note This function is private because it is only used by the Parser
     * class.
     */
    std::string getAttribute(pugi::xml_node node, std::string name);

    /**
     * @brief Parse the color from the XML file.
     * @param node The node of the XML file (pugi::xml_node is a typedef of
     * pugixml)
     * @param name The name of the attribute.
     * @return The color which is parsed from the XML file.
     * @note This function is private because it is only used by the Parser
     * class.
     * @note The color is represented by sf::Color (SFML)
     * @note The color is parsed from the XML file in the format of
     * "rgb(r, g, b)".
     */
    Color parseColor(pugi::xml_node node, std::string name);

    /**
     * @brief Parse the points from the XML file.
     * @param node The node of the XML file (pugi::xml_node is a typedef of
     * pugixml)
     * @return The points which are parsed from the XML file.
     * @note This function is private because it is only used by the Parser
     * class.
     * @note The points are represented by std::vector< sf::Vector2f > (SFML)
     * @note The points are parsed from the XML file in the format of
     * "x1,y1 x2,y2 x3,y3 ...".
     */
    std::vector< Vector2Df > parsePoints(pugi::xml_node node);

    /**
     * @brief Parse the XML file.
     * @param window The window to render the shapes on.
     * @note This function is private because it is only used by the Parser
     * class.
     * @note The shapes are rendered on the window by calling the draw() method
     *  of the Shape class.
     * @note The shapes are rendered on the window in the order of the XML file.
     */
    void parseSVG();

    /**
     * @brief Render the shapes on the window.
     * @param window The window to render the shapes on.
     * @note This function is private because it is only used by the Parser
     * class.
     * @note The shapes are rendered on the window by calling the draw() method
     * of the Shape class.
     * @note The shapes are rendered on the window in the order of the XML file.
     * @note Apply Polymorphism to render the shapes on the window.
     */
    void renderSVG(Renderer& renderer);

    /**
     * @brief Get the Translate information (x, y) which is parsed from the XML
     * file.
     *
     * @param node The node of the XML file (pugi::xml_node is a typedef of
     * pugixml)
     * @param name The name of the attribute.
     * @return The translate which is parsed from the XML file.
     */
    std::pair< float, float > getTranslate(pugi::xml_node node,
                                           std::string name = "transform");

    /**
     * @brief Get the Rotate information (degree) which is parsed from the XML
     * file.
     *
     * @param node The node of the XML file (pugi::xml_node is a typedef of
     * pugixml)
     * @param name The name of the attribute.
     * @return The rotate which is parsed from the XML file.
     */
    float getRotate(pugi::xml_node node, std::string name = "transform");

    /**
     * @brief Destructor of Parser.
     */
    ~Parser();

private:
    /**
     * @brief Construct a new Parser object.
     *
     * @param file_name The name of the file to be parsed.
     */
    Parser(const std::string& file_name);

    static Parser* instance;       ///< The instance of the Parser.
    pugi::xml_node svg;            ///< The node of the SVG.
    std::vector< Shape* > shapes;  ///< The vector of the shapes.
};
#endif