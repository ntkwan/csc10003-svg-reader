#include "Parser.hpp"

#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

namespace {
    std::vector< std::string > getTransformOrder(const pugi::xml_node &node) {
        std::string transform_tag = node.attribute("transform").value();

        transform_tag.erase(std::remove_if(transform_tag.begin(),
                                           transform_tag.end(), ::isspace),
                            transform_tag.end());

        size_t start_pos = transform_tag.find(")");
        while (start_pos != std::string::npos) {
            transform_tag.insert(start_pos + 1, " ");
            start_pos = transform_tag.find(")", start_pos + 1);
        }

        std::vector< std::string > order;
        std::stringstream ss(transform_tag);
        std::string type;
        while (ss >> type) {
            order.push_back(type);
        }

        return order;
    }

    auto getHexColor = [](std::string color) -> Color {
        std::stringstream ss;
        int pos = color.find("#");
        ss << std::hex << color.substr(pos + 1, 2) << " "
           << color.substr(pos + 3, 2) << " " << color.substr(pos + 5, 2);
        int r, g, b;
        ss >> r >> g >> b;
        if (color[pos + 7] != '\0' && color[pos + 7] != ' ') {
            std::stringstream ss;
            ss << std::hex << color.substr(pos + 7, 2);
            int a;
            ss >> a;
            return Color(r, g, b, a);
        }
        return Color(r, g, b, 255);
    };

    auto getRgbColor = [](std::string color) -> Color {
        int r, g, b;
        float a = 1;
        sscanf(color.c_str(), "rgb(%d,%d,%d,%f)", &r, &g, &b, &a);
        return Color(r, g, b, 255 * a);
    };
}  // namespace

Parser *Parser::instance = nullptr;

Parser *Parser::getInstance(const std::string &file_name) {
    if (instance == nullptr) {
        instance = new Parser(file_name);
        instance->parseSVG(instance->svg);
    }
    return instance;
}

Parser::Parser(const std::string &file_name) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(file_name.c_str());
    if (!result) exit(EXIT_FAILURE);
    svg = doc.child("svg");
}

std::string Parser::getAttribute(const pugi::xml_node &node, std::string name) {
    pugi::xml_attribute attr = node.attribute(name.c_str());
    if (!attr) {
        if (name == "fill")
            return "black";
        else if (name == "stroke" || name == "transform" || name == "rotate")
            return "none";
    }
    return attr.as_string();
};

float Parser::getFloatAttribute(const pugi::xml_node &node, std::string name) {
    pugi::xml_attribute attr = node.attribute(name.c_str());
    if (!attr) {
        if (name == "stroke-width" || name == "stroke-opacity" ||
            name == "fill-opacity" || name == "opacity")
            return 1;
    }
    return attr.as_float();
}

Color Parser::parseColor(const pugi::xml_node &node, std::string name) {
    std::string color = getAttribute(node, name);
    for (auto &c : color) c = tolower(c);
    if (color == "none")
        return Color::Transparent;
    else {
        Color result;
        if (color.find("#") != std::string::npos) {
            result = getHexColor(color);
        } else if (color.find("rgb") == std::string::npos) {
            auto color_code = color_map.find(color);
            if (color_code == color_map.end()) exit(-1);
            result = color_code->second;
        } else
            result = getRgbColor(color);

        result.a = result.a * getFloatAttribute(node, name + "-opacity") *
                   getFloatAttribute(node, "opacity");
        return result;
    }
}

std::vector< Vector2Df > Parser::parsePoints(const pugi::xml_node &node) {
    std::vector< Vector2Df > points;
    std::string points_string = getAttribute(node, "points");

    std::stringstream ss(points_string);
    float x, y;
    char comma;

    while (ss >> x >> comma >> y) {
        points.push_back(Vector2Df(x, y));
    }

    return points;
}

std::pair< float, float > Parser::getTranslate(std::string transform_value) {
    float trans_x = 0, trans_y = 0;
    sscanf(transform_value.c_str(), "translate(%f, %f)", &trans_x, &trans_y);
    return std::pair< float, float >(trans_x, trans_y);
}

float Parser::getRotate(std::string transform_value) {
    float degree = 0;
    sscanf(transform_value.c_str(), "rotate(%f)", &degree);
    return degree;
}

void Parser::applyTransform(Shape *shape,
                            const std::vector< std::string > &transform_order) {
    for (auto type : transform_order) {
        if (type.find("translate") != std::string::npos) {
            float trans_x = getTranslate(type).first,
                  trans_y = getTranslate(type).second;
            shape->translate(trans_x, trans_y);
        } else if (type.find("rotate") != std::string::npos) {
            float degree = getRotate(type);
            shape->rotate(degree);
        }
    }
};

void Parser::parseSVG(const pugi::xml_node &node) {
    for (pugi::xml_node tool = node.first_child(); tool;
         tool = tool.next_sibling()) {
        std::cout << "Tool:" << tool.name() << std::endl;
        if (tool.name() == std::string("g")) {
            for (auto attr : tool.attributes()) {
                for (pugi::xml_node child = tool.first_child(); child;
                     child = child.next_sibling()) {
                    bool flag = false;
                    for (auto child_attr : child.attributes()) {
                        if (strcmp(child_attr.name(), attr.name()) == 0) {
                            flag = true;
                            break;
                        }
                    }
                    if (!flag)
                        child.append_copy(attr);
                    else {
                        if (strcmp(attr.name(), "transform") == 0) {
                            std::string new_transform =
                                std::string(attr.value()) + " " +
                                child.attribute("transform").value();
                            child.attribute("transform")
                                .set_value(new_transform.c_str());
                        }
                    }
                }
            }
            parseSVG(tool);
        } else if (tool.name() == std::string("line")) {
            parseLine(tool);
        } else if (tool.name() == std::string("rect")) {
            parseRect(tool);
        } else if (tool.name() == std::string("circle")) {
            parseCircle(tool);
        } else if (tool.name() == std::string("ellipse")) {
            parseEllipse(tool);
        } else if (tool.name() == std::string("polygon")) {
            parsePolygon(tool);
        } else if (tool.name() == std::string("polyline")) {
            parsePolyline(tool);
        } else if (tool.name() == std::string("text")) {
            parseText(tool);
        } else if (tool.name() == std::string("path")) {
            parsePath(tool);
        }
    }
}

void Parser::parseLine(const pugi::xml_node &node) {
    std::vector< std::string > transform_order = getTransformOrder(node);
    Color stroke_color = parseColor(node, "stroke");
    float stroke_width = node.attribute("stroke-width").as_float();
    Line *shape = new Line(Vector2Df(node.attribute("x1").as_float(),
                                     node.attribute("y1").as_float()),
                           Vector2Df(node.attribute("x2").as_float(),
                                     node.attribute("y2").as_float()),
                           stroke_color, stroke_width);
    applyTransform(shape, transform_order);
    shapes.push_back(shape);
}

void Parser::parseRect(const pugi::xml_node &node) {
    std::vector< std::string > transform_order = getTransformOrder(node);
    Color stroke_color = parseColor(node, "stroke");
    Color fill_color = parseColor(node, "fill");
    float stroke_width = node.attribute("stroke-width").as_float();
    float x = node.attribute("x").as_float();
    float y = node.attribute("y").as_float();
    Rect *shape = new Rect(node.attribute("width").as_float(),
                           node.attribute("height").as_float(), x, y,
                           fill_color, stroke_color, stroke_width);
    applyTransform(shape, transform_order);
    shapes.push_back(shape);
}

void Parser::parseCircle(const pugi::xml_node &node) {
    std::vector< std::string > transform_order = getTransformOrder(node);
    Color stroke_color = parseColor(node, "stroke");
    Color fill_color = parseColor(node, "fill");
    float stroke_width = getFloatAttribute(node, "stroke-width");
    float cx = getFloatAttribute(node, "cx");
    float cy = getFloatAttribute(node, "cy");
    float radius = getFloatAttribute(node, "r");
    Circle *shape = new Circle(radius, Vector2Df(cx, cy), fill_color,
                               stroke_color, stroke_width);
    applyTransform(shape, transform_order);
    shapes.push_back(shape);
}

void Parser::parseEllipse(const pugi::xml_node &node) {
    std::vector< std::string > transform_order = getTransformOrder(node);
    Color stroke_color = parseColor(node, "stroke");
    Color fill_color = parseColor(node, "fill");
    float stroke_width = getFloatAttribute(node, "stroke-width");
    float radius_x = getFloatAttribute(node, "rx");
    float radius_y = getFloatAttribute(node, "ry");
    float cx = getFloatAttribute(node, "cx");
    float cy = getFloatAttribute(node, "cy");
    Ellipse *shape =
        new Ellipse(Vector2Df(radius_x, radius_y), Vector2Df(cx, cy),
                    fill_color, stroke_color, stroke_width);
    applyTransform(shape, transform_order);
    shapes.push_back(shape);
}

void Parser::parsePolygon(const pugi::xml_node &node) {
    Color stroke_color = parseColor(node, "stroke");
    Color fill_color = parseColor(node, "fill");
    float stroke_width = getFloatAttribute(node, "stroke-width");
    Polygon *shape = new Polygon(fill_color, stroke_color, stroke_width);
    std::vector< Vector2Df > points = parsePoints(node);
    for (auto point : points) {
        shape->addPoint(point);
    }
    shape->updateShape();
    shapes.push_back(shape);
}

void Parser::parsePolyline(const pugi::xml_node &node) {
    Color stroke_color = parseColor(node, "stroke");
    Color fill_color = parseColor(node, "fill");
    float stroke_width = getFloatAttribute(node, "stroke-width");
    Polyline *shape = new Polyline(stroke_width, stroke_color, fill_color);
    std::vector< Vector2Df > points = parsePoints(node);
    for (auto point : points) {
        shape->addPoint(point);
    }
    shapes.push_back(shape);
}

void Parser::parseText(const pugi::xml_node &node) {
    std::vector< std::string > transform_order = getTransformOrder(node);
    Color fill_color = parseColor(node, "fill");
    float x = getFloatAttribute(node, "x");
    float y = getFloatAttribute(node, "y");
    float font_size = getFloatAttribute(node, "font-size");
    std::string text = node.text().get();
    Text *shape =
        new Text(Vector2Df(x, y - font_size), text, fill_color, font_size);
    applyTransform(shape, transform_order);
    std::cout << shape->getPosition().x << "," << shape->getPosition().y
              << std::endl;
    shapes.push_back(shape);
}

void Parser::parsePath(const pugi::xml_node &node) {
    /*

    TBD: PATH

    */
}

void Parser::renderSVG(Renderer &renderer) {
    for (auto shape : shapes) {
        renderer.draw(shape);
    }
}

Parser::~Parser() {
    for (auto shape : shapes) {
        delete shape;
    }
    shapes.clear();
}