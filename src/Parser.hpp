#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "Graphics.hpp"
#include "pugixml.hpp"

class Parser {
public:
    static Parser* getInstance(const std::string& file_name);
    Parser(const Parser&) = delete;
    std::string getAttribute(pugi::xml_node node, std::string name);
    sf::Color parseColor(pugi::xml_node node, std::string name);
    std::vector< sf::Vector2f > parsePoints(pugi::xml_node node);
    void parseSVG();
    void renderSVG(sf::RenderWindow& window);
    ~Parser();

private:
    Parser(const std::string& file_name);
    static Parser* instance;
    pugi::xml_node svg;
    std::vector< Shape* > shapes;
};
#endif