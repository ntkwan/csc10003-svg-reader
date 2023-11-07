#include "Parser.hpp"

#include <vector>

Parser::Parser(const std::string& file_name) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(file_name.c_str());
    if (!result) exit(-1);
    svg = doc.child("svg");
}

std::string Parser::getAttribute(pugi::xml_node node, std::string name) {
    pugi::xml_attribute attr = node.attribute(name.c_str());
    if (!attr) {
        if (name == "fill")
            return "black";
        else if (name == "stroke")
            return "none";
        else if (name == "stroke-width" || name == "stroke-opacity" ||
                 name == "fill-opacity")
            return "1";
        else if (name == "r" || name == "cx" || name == "cy" || name == "x" ||
                 name == "y" || name == "width" || name == "height")
            return "0";
    }
    return attr.value();
};

sf::Color Parser::parseColor(pugi::xml_node node, std::string name) {
    auto getAtribColor = [](std::string color) -> sf::Color {
        std::string c = "";
        std::vector< int > rgb;
        for (int i = 4; i < (int)color.size(); ++i) {
            if ('0' <= color[i] && color[i] <= '9') {
                c += color[i];
            } else {
                if (c.size() > 0) {
                    rgb.push_back(std::stoi(c));
                    c.clear();
                }
            }
        }

        sf::Color result = sf::Color(rgb[0], rgb[1], rgb[2]);
        return result;
    };

    std::string color = getAttribute(node, name);
    for (auto& c : color) c = tolower(c);
    if (color == "none")
        return sf::Color::Transparent;
    else {
        sf::Color result;
        if (color.find("rgb") == std::string::npos) {
            auto color_code = color_map.find(color);
            if (color_code == color_map.end()) exit(-1);
            result = color_code->second;
        } else
            result = getAtribColor(color);

        result.a = std::stof(getAttribute(node, name + "-opacity")) * 255;
        return result;
    }
}

void Parser::parseSVG() {
    for (pugi::xml_node tool = svg.first_child(); tool;
         tool = tool.next_sibling()) {
        sf::Color stroke_color = parseColor(tool, "stroke");
        sf::Color fill_color = parseColor(tool, "fill");

        float stroke_width = std::stof(getAttribute(tool, "stroke-width"));

        if (tool.name() == std::string("rect")) {
            Rect* shape = new Rect(std::stof(getAttribute(tool, "width")),
                                   std::stof(getAttribute(tool, "height")),
                                   std::stof(getAttribute(tool, "x")),
                                   std::stof(getAttribute(tool, "y")),
                                   fill_color, stroke_color, stroke_width);
            shapes.push_back(shape);
        } else if (tool.name() == std::string("line")) {
            Line* shape =
                new Line(sf::Vector2f(std::stof(getAttribute(tool, "x1")),
                                      std::stof(getAttribute(tool, "y1"))),
                         sf::Vector2f(std::stof(getAttribute(tool, "x2")),
                                      std::stof(getAttribute(tool, "y2"))),
                         stroke_color, stroke_width);
            shapes.push_back(shape);
        } else if (tool.name() == std::string("text")) {
            /*

            TEXT

            */
        } else if (tool.name() == std::string("circle")) {
            Circle* shape =
                new Circle(std::stof(getAttribute(tool, "r")),
                           sf::Vector2f(std::stof(getAttribute(tool, "cx")),
                                        std::stof(getAttribute(tool, "cy"))),
                           fill_color, stroke_color, stroke_width);
            shapes.push_back(shape);
        } else if (tool.name() == std::string("ellipse")) {
            Ellipse* shape =
                new Ellipse(sf::Vector2f(std::stof(getAttribute(tool, "rx")),
                                         std::stof(getAttribute(tool, "ry"))),
                            sf::Vector2f(std::stof(getAttribute(tool, "cx")),
                                         std::stof(getAttribute(tool, "cy"))),
                            fill_color, stroke_color, stroke_width);
            shapes.push_back(shape);
        } else if (tool.name() == std::string("polygon")) {
            /*

            POLYGON

            */

        } else if (tool.name() == std::string("polyline")) {
            /*

            POLYLINE

            */
        } else if (tool.name() == std::string("path")) {
            /*

            PATH

            */
        } else {
            continue;
        }
    }
}

void Parser::renderSVG(sf::RenderWindow& window) {
    for (auto shape : shapes) {
        window.draw(*shape);
    }
}

Parser::~Parser() {
    for (auto shape : shapes) {
        delete shape;
    }
    shapes.clear();
}