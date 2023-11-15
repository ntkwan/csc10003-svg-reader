#include "Parser.hpp"

#include <iostream>
#include <sstream>
#include <vector>

Parser* Parser::instance = nullptr;

Parser* Parser::getInstance(const std::string& file_name) {
    if (instance == nullptr) {
        instance = new Parser(file_name);
        instance->parseSVG();
    }
    return instance;
}

Parser::Parser(const std::string& file_name) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(file_name.c_str());
    if (!result) EXIT_FAILURE;
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
                 name == "fill-opacity" || name == "opacity")
            return "1";
        else if (name == "r" || name == "cx" || name == "cy" || name == "x" ||
                 name == "y" || name == "width" || name == "height")
            return "0";
    }
    return attr.value();
};

sf::Color Parser::parseColor(pugi::xml_node node, std::string name) {
    auto getRgbColor = [](std::string color) -> sf::Color {
        int r, g, b;
        float a = 1;
        sscanf(color.c_str(), "rgb(%d,%d,%d,%f)", &r, &g, &b, &a);
        return sf::Color(r, g, b, 255 * a);
    };

    auto getHexColor = [](std::string color) -> sf::Color {
        std::stringstream ss;
        ss << std::hex << color.substr(1, 2) << " " << color.substr(3, 2) << " "
           << color.substr(5, 2);
        int r, g, b;
        ss >> r >> g >> b;
        if (color.size() > 7) {
            std::stringstream ss;
            ss << std::hex << color.substr(7, 2);
            int a;
            ss >> a;
            return sf::Color(r, g, b, a);
        }
        return sf::Color(r, g, b, 255);
    };

    std::string color = getAttribute(node, name);
    for (auto& c : color) c = tolower(c);
    if (color == "none")
        return sf::Color::Transparent;
    else {
        sf::Color result;
        if (color[0] == '#') {
            result = getHexColor(color);
        } else if (color.find("rgb") == std::string::npos) {
            auto color_code = color_map.find(color);
            if (color_code == color_map.end()) exit(-1);
            result = color_code->second;
        } else
            result = getRgbColor(color);

        result.a = result.a / 255.f *
                   std::stof(getAttribute(node, name + "-opacity")) *
                   std::stof(getAttribute(node, "opacity")) * 255;
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
            sf::String text = tool.text().get();
            Text* shape = new Text(sf::Vector2f(x, y - font_size), text,
                                   fill_color, font_size);
            shapes.push_back(shape);
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
                new Polygon(fill_color, stroke_color, stroke_width);
            std::vector< sf::Vector2f > points = parsePoints(tool);
            for (auto point : points) {
                shape->addPoint(point);
            }
            shape->polygonUpdate();
            shapes.push_back(shape);
        } else if (tool.name() == std::string("polyline")) {
            Polyline* shape =
                new Polyline(stroke_width, stroke_color, fill_color);
            std::vector< sf::Vector2f > points = parsePoints(tool);
            for (auto point : points) {
                shape->addPoint(point);
            }
            shape->polylineUpdate();
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