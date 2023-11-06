#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "Graphics.hpp"
#include "pugixml.hpp"

class Parser {
public:
    Parser(const std::string& file_name);
    std::string getAttribute(pugi::xml_node node, std::string name);
    sf::Color parseColor(pugi::xml_node node, std::string name);
    void parseSVG();
    void renderSVG(sf::RenderWindow& window);
    ~Parser();

private:
    pugi::xml_node svg;
    std::vector< Shape* > shapes;
};
#endif