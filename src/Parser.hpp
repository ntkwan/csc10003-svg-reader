#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "Graphics.hpp"
#include "pugixml.hpp"

/**
 * @brief Represents a parser for parsing SVG files.
 *
 * The Parser class is responsible for parsing SVG files and rendering the
 * shapes in the SVG file.
 */
class Parser {
public:
    /**
     * @brief Get the instance of the Parser.
     *
     * @param file_name The name of the file to be parsed.
     *
     * @return The instance of the Parser.
     */
    static Parser* getInstance(const std::string& file_name);

    /**
     * @brief Deleted copy constructor.
     *
     * This function is deleted to prevent copying of the Parser.
     *
     * @param parser The Parser to be copied.
     *
     * @return The instance of the Parser.
     *
     */
    Parser(const Parser&) = delete;

    /**
     * @brief Get attribute of the node.
     *
     * @param node The node to be parsed.
     * @param name The name of the attribute.
     *
     * @return The value of the attribute.
     */
    std::string getAttribute(pugi::xml_node node, std::string name);

    /**
     * @brief Parse color from the node.
     *
     * @param node The node to be parsed.
     * @param name The name of the attribute.
     *
     * @return The color of the attribute.
     */
    sf::Color parseColor(pugi::xml_node node, std::string name);

    /**
     * @brief Parse points from the node.
     *
     * This function is used to parse points from the polygon and polyline
     *
     * @param node The node to be parsed.
     *
     * @return The points of the attribute.
     */
    std::vector< sf::Vector2f > parsePoints(pugi::xml_node node);

    /**
     * @brief Parse transform from the node.
     *
     * @param node The node to be parsed.
     *
     * @return The transform of the attribute.
     */
    void parseSVG();

    /**
     * @brief Render the SVG.
     *
     * This function is used to render the all the SVG shapes.
     *
     * @param window The window to be rendered.
     */
    void renderSVG(sf::RenderWindow& window);

    /**
     * @brief Destructor of the Parser.
     *
     * This function is used to delete all the shapes.
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