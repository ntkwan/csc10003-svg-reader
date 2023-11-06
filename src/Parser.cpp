#include "Parser.hpp"

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
    std::string color = getAttribute(node, name);
    for (auto& c : color) c = tolower(c);
    if (color == "none")
        return sf::Color::Transparent;
    else if (color.find("rgb") == std::string::npos) {
        auto color_code = color_map.find(color);
        if (color_code == color_map.end()) exit(-1);
        return color_code->second;
    } else {
        unsigned int r = 0, g = 0, b = 0, a;
        sscanf(color.c_str(), "rgb(%u,%u,%u)", &r, &g, &b);
        a = stof(getAttribute(node, name + "-opacity")) * 255;
        return sf::Color(r, g, b, a);
    }
}

void Parser::parseSVG() {
    for (pugi::xml_node tool = svg.first_child(); tool;
         tool = tool.next_sibling()) {
        sf::Color stroke_color = parseColor(tool, "stroke");
        sf::Color fill_color = parseColor(tool, "fill");
        float stroke_width = stof(getAttribute(tool, "stroke-width"));

        if (tool.name() == std::string("rect")) {
            /*

            RECTANGLE

             */
        } else if (tool.name() == std::string("line")) {
            /*

            LINE

            */
        } else if (tool.name() == std::string("text")) {
            /*

            TEXT

            */
        } else if (tool.name() == std::string("circle")) {
            Circle* shape =
                new Circle(stof(getAttribute(tool, "r")),
                           sf::Vector2f(stof(getAttribute(tool, "cx")),
                                        stof(getAttribute(tool, "cy"))),
                           fill_color, stroke_color, stroke_width);
            shapes.push_back(shape);
        } else if (tool.name() == std::string("ellipse")) {
            Ellipse* shape =
                new Ellipse(sf::Vector2f(stof(getAttribute(tool, "rx")),
                                         stof(getAttribute(tool, "ry"))),
                            sf::Vector2f(stof(getAttribute(tool, "cx")),
                                         stof(getAttribute(tool, "cy"))),
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