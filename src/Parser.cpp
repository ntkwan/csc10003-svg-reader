#include "Parser.hpp"

#include <iostream>
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

std::vector< sf::Vector2f > Parser::parsePoints(pugi::xml_node node) {
    std::vector< sf::Vector2f > points;
    std::string points_string = getAttribute(node, "points");
    std::string point = "";
    int pos = 0;
    float x = 0, y = 0;
    for (int i = 0; i < (int)points_string.size(); i++) {
        if (points_string[i] == ' ') {
            if (point.size() > 0) {
                pos = point.find(',');
                x = std::stof(point.substr(0, pos));
                y = std::stof(point.substr(pos + 1));
                points.push_back(sf::Vector2f(x, y));
                point.clear();
            }
        } else {
            point += points_string[i];
        }
    }

    if (point.size() > 0) {
        pos = point.find(',');
        x = std::stof(point.substr(0, pos));
        y = std::stof(point.substr(pos + 1));
        points.push_back(sf::Vector2f(x, y));
    }

    return points;
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
            float x = std::stof(getAttribute(tool, "x"));
            float y = std::stof(getAttribute(tool, "y"));
            float font_size = std::stof(getAttribute(tool, "font-size"));
            std::string text = tool.text().get();
        } else if (tool.name() == std::string("circle")) {
            float radius = std::stof(getAttribute(tool, "r"));
            Circle* shape = new Circle(
                radius,
                sf::Vector2f(std::stof(getAttribute(tool, "cx")) - radius,
                             std::stof(getAttribute(tool, "cy")) - radius),
                fill_color, stroke_color, stroke_width);
            shapes.push_back(shape);
        } else if (tool.name() == std::string("ellipse")) {
            float radius_x = std::stof(getAttribute(tool, "rx"));
            float radius_y = std::stof(getAttribute(tool, "ry"));
            Ellipse* shape = new Ellipse(
                sf::Vector2f(radius_x, radius_y),
                sf::Vector2f(std::stof(getAttribute(tool, "cx")) - radius_x,
                             std::stof(getAttribute(tool, "cy")) - radius_y),
                fill_color, stroke_color, stroke_width);
            shapes.push_back(shape);
        } else if (tool.name() == std::string("polygon")) {
            Polygon* shape =
                new Polygon(fill_color, stroke_color, stroke_width, SCALE);
            std::vector< sf::Vector2f > points = parsePoints(tool);
            for (auto point : points) {
                shape->addPoint(point);
            }
            shapes.push_back(shape);
        } else if (tool.name() == std::string("polyline")) {
            PolyLine* shape =
                new PolyLine(stroke_width, stroke_color, fill_color);
            std::vector< sf::Vector2f > points = parsePoints(tool);
            for (auto point : points) {
                shape->addPoint(point);
            }
            shapes.push_back(shape);
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
        shape->draw(window);
    }
}

Parser::~Parser() {
    for (auto shape : shapes) {
        delete shape;
    }
    shapes.clear();
}