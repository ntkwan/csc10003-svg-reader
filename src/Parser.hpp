#ifndef PARSER_H_
#define PARSER_H_
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Circle.hpp"
#include "Ellipse.hpp"
#include "pugixml.hpp"

namespace parser {
    pugi::xml_node loadFile(std::string path) {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(path.c_str());
        if (!result) {
            std::cout << "Error: " << result.description() << std::endl;
            exit(-1);
        }
        pugi::xml_node svg = doc.child("svg");
        return svg;
    }

    std::string getAttribute(pugi::xml_node node, std::string name) {
        auto attr = node.attribute(name.c_str());
        if (!attr) {
            if (name == "fill")
                return "black";
            else if (name == "stroke")
                return "none";
            else if (name == "stroke-width" || name == "stroke-opacity" ||
                     name == "fill-opacity")
                return "1";
            else if (name == "r" || name == "cx" || name == "cy" ||
                     name == "x" || name == "y" || name == "width" ||
                     name == "height")
                return "0";
        }
        return attr.value();
    };

    sf::Color parseColor(pugi::xml_node node, std::string name) {
        std::string color = getAttribute(node, name);
        for (auto& c : color) c = tolower(c);
        if (color == "none")
            return sf::Color::Transparent;
        else if (color.find("rgb") == std::string::npos) {
            switch (color[0]) {
                case 'b':
                    if (color[2] == 'a')
                        return sf::Color::Black;
                    else
                        return sf::Color::Blue;
                case 'g':
                    return sf::Color::Green;
                case 'r':
                    return sf::Color::Red;
                case 'y':
                    return sf::Color::Yellow;
                case 'c':
                    return sf::Color::Cyan;
                case 'm':
                    return sf::Color::Magenta;
                case 'w':
                    return sf::Color::White;
                default:
                    return sf::Color::Transparent;
            }
        } else {
            unsigned int r = 0, g = 0, b = 0, a;
            sscanf(color.c_str(), "rgb(%u,%u,%u)", &r, &g, &b);
            a = stof(getAttribute(node, name + "-opacity")) * 255;
            return sf::Color(r, g, b, a);
        }
    }

    std::vector< sf::Shape* > parseSVG(std::string path) {
        pugi::xml_node svg = loadFile(path);
        std::vector< sf::Shape* > shapes;
        for (pugi::xml_node tool = svg.first_child(); tool;
             tool = tool.next_sibling()) {
            sf::Color stroke_color = parseColor(tool, "stroke");
            sf::Color fill_color = parseColor(tool, "fill");
            float stroke_width = stof(getAttribute(tool, "stroke-width"));

            if (tool.name() == std::string("rect")) {         // rect
            } else if (tool.name() == std::string("line")) {  // line
            } else if (tool.name() == std::string("text")) {  // text
            } else if (tool.name() == std::string("circle")) {
                Circle* shape =
                    new Circle(stof(getAttribute(tool, "r")),
                               stof(getAttribute(tool, "cx")),
                               stof(getAttribute(tool, "cy")), fill_color,
                               stroke_color, stroke_width);
                shapes.push_back(shape);
            } else if (tool.name() == std::string("ellipse")) {
                Ellipse* shape =
                    new Ellipse(sf::Vector2f(stof(getAttribute(tool, "rx")),
                                             stof(getAttribute(tool, "ry"))),
                                stof(getAttribute(tool, "cx")),
                                stof(getAttribute(tool, "cy")), fill_color,
                                stroke_color, stroke_width);
                shapes.push_back(shape);
            } else if (tool.name() == std::string("polygon")) {   // polygon
            } else if (tool.name() == std::string("polyline")) {  // polyline
            } else if (tool.name() == std::string("path")) {      // path
            }
        }
        return shapes;
    }

    void deleteShapes(std::vector< sf::Shape* >& shapes) {
        for (auto shape : shapes) {
            delete shape;
        }
        shapes.clear();
    }
}  // namespace parser
#endif