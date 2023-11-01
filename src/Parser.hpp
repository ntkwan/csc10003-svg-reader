#ifndef PARSER_H_
#define PARSER_H_
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "Circle.hpp"
#include "pugixml.hpp"

using namespace std;

namespace parser {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./sample/sample.svg");
    pugi::xml_node svg = doc.child("svg");

    auto get_attribute = [&](pugi::xml_node node, string name) {
        auto attr = node.attribute(name.c_str());
        return attr.value();
    };

    vector< sf::Shape > parseSVG() {
        vector< sf::Shape > shapes;
        for (pugi::xml_node tool = svg.first_child(); tool;
             tool = tool.next_sibling()) {
            if (tool.name() == "circle") {
                unsigned int r = 0, g = 0, b = 0, a;
                string stroke = get_attribute(tool, "stroke");
                sscanf(stroke.c_str(), "rgb(%u,%u,%u)", &r, &g, &b);
                a = stoi(get_attribute(tool, "stroke-opacity")) * 255;
                sf::Color stroke_color(r, g, b, a);

                string fill = get_attribute(tool, "fill");
                sscanf(fill.c_str(), "rgb(%u,%u,%u)", &r, &g, &b);
                a = stoi(get_attribute(tool, "fill-opacity")) * 255;
                sf::Color fill_color(r, g, b, a);

                Circle shape(stof(get_attribute(tool, "r")),
                             stof(get_attribute(tool, "cx")),
                             stof(get_attribute(tool, "cy")), fill_color,
                             stroke_color,
                             stof(get_attribute(tool, "stroke-width")));
                shapes.push_back(shape);
            }
        }
    }

}  // namespace parser
#endif