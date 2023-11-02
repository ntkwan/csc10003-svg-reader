#ifndef PARSER_H_
#define PARSER_H_
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "Circle.hpp"
#include "Ellipse.hpp"
#include "pugixml.hpp"

using namespace std;

namespace parser {

    pugi::xml_node loadFile(string path) {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(path.c_str());
        if (!result) {
            cout << "Error: " << result.description() << endl;
            exit(-1);
        }
        pugi::xml_node svg = doc.child("svg");
        return svg;
    }

    string getAttribute(pugi::xml_node node, string name) {
        auto attr = node.attribute(name.c_str());
        if (!attr) {
            if (name == "fill")
                return "rgb(0,0,0)";
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

    sf::Color parseColor(pugi::xml_node node, string color) {
        for (auto& c : color) c = tolower(c);
        if (color == "none")
            return sf::Color::Transparent;
        else if (color.find("rgb") == string::npos) {
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
            a = stoi(getAttribute(node, "stroke-opacity")) * 255;
            return sf::Color(r, g, b, a);
        }
    }

    vector< sf::Shape* > parseSVG() {
        pugi::xml_node svg = loadFile("../sample/sample.svg");
        vector< sf::Shape* > shapes;
        for (pugi::xml_node tool = svg.first_child(); tool;
             tool = tool.next_sibling()) {
            if (tool.name() == string("circle")) {
                sf::Color stroke_color =
                    parseColor(tool, getAttribute(tool, "stroke"));
                sf::Color fill_color =
                    parseColor(tool, getAttribute(tool, "fill"));
                float stroke_width = stof(getAttribute(tool, "stroke-width"));

                Circle* circle =
                    new Circle(stof(getAttribute(tool, "r")),
                               stof(getAttribute(tool, "cx")),
                               stof(getAttribute(tool, "cy")), fill_color,
                               stroke_color, stroke_width);
                shapes.push_back(circle);
            } else if (tool.name() == string("ellipse")) {
                sf::Color stroke_color =
                    parseColor(tool, getAttribute(tool, "stroke"));
                sf::Color fill_color =
                    parseColor(tool, getAttribute(tool, "fill"));
                float stroke_width = stof(getAttribute(tool, "stroke-width"));

                Ellipse* circle =
                    new Ellipse(sf::Vector2f(stof(getAttribute(tool, "rx")),
                                             stof(getAttribute(tool, "ry"))),
                                stof(getAttribute(tool, "cx")),
                                stof(getAttribute(tool, "cy")), fill_color,
                                stroke_color, stroke_width);
                shapes.push_back(circle);
            }
        }
        return shapes;
    }

}  // namespace parser
#endif