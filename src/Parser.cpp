#include "Parser.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>

Parser *Parser::instance = nullptr;

Parser *Parser::getInstance(const std::string &file_name) {
    if (instance == nullptr) {
        instance = new Parser(file_name);
    }
    return instance;
}

Parser::Parser(const std::string &file_name) {
    parseObjects(file_name);
    parseSVG();
}

std::string Parser::parseSVG(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }
    std::string line;
    std::string svg;
    bool svg_found = false;
    while (std::getline(file, line)) {
        if (line.find("</svg>") != std::string::npos) {
            svg_found = false;
        }
        if (svg_found) {
            svg += line + "\n";
        }
        if (line.find("svg") != std::string::npos) {
            svg_found = true;
        }
    }
    file.close();
    return svg;
}

Attributes Parser::parseAttributes(std::string attributes) {
    std::vector< std::pair< std::string, std::string > > attributes_vector;
    std::string name;
    std::string value;
    std::stringstream ss(attributes);
    while (std::getline(ss, name, '=')) {
        char c = '0';
        ss >> c;
        if (c != '"') {
            break;
        }
        getline(ss, value, c);
        name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
        attributes_vector.push_back(std::make_pair(name, value));
    }
    return attributes_vector;
}

Tags Parser::parseTags(std::string svg) {
    std::vector< std::pair< std::string, std::string > > tags;
    std::string line;
    std::stringstream ss(svg);
    while (std::getline(ss, line)) {
        int start_pos = line.find("<");
        int end_pos = line.find(">");
        std::string tag = line.substr(start_pos + 1, end_pos - start_pos - 1);
        if (tag.size() > 0) {
            int space_pos = tag.find(" ");
            std::string name = tag.substr(0, space_pos);
            std::string attributes = tag.substr(space_pos + 1, tag.size() - 1);
            if (attributes[attributes.size() - 1] == ' ' ||
                attributes[attributes.size() - 1] == '/') {
                attributes = attributes.substr(0, attributes.size() - 1);
            }
            if (name == "text") {
                std::string text;
                std::stringstream ss2(line);
                std::getline(ss2, text, '>');
                std::getline(ss2, text, '<');
                attributes += " text=\"" + text + "\"";
            }
            tags.push_back(std::make_pair(name, attributes));
        }
    }
    return tags;
}

void Parser::parseObjects(std::string filePath) {
    std::string svg = parseSVG(filePath);
    Tags tags = parseTags(svg);

    for (auto tag : tags) {
        std::vector< std::pair< std::string, std::string > > attributes =
            parseAttributes(tag.second);
        objects.push_back(std::make_pair(tag.first, attributes));
    }

    int group = 0;
    std::deque< Attributes > group_attributes;
    for (auto &object : objects) {
        std::string name = object.first;
        if (name == "/g") {
            group--;
            group_attributes.pop_front();
        }
        if (group > 0 && name != "g" && name != "/g") {
            for (Attributes attributes : group_attributes) {
                for (auto attribute : attributes) {
                    bool flag = false;
                    for (auto object_attribute : object.second) {
                        if (attribute.first == object_attribute.first) {
                            flag = true;
                            break;
                        }
                    }
                    if (!flag) {
                        object.second.push_back(attribute);
                    } else {
                        if (attribute.first == "transform") {
                            for (auto &object_attribute : object.second) {
                                if (object_attribute.first == "transform") {
                                    object_attribute.second =
                                        attribute.second + " " +
                                        object_attribute.second;
                                }
                            }
                        }
                        if (attribute.first == "opacity") {
                            for (auto &object_attribute : object.second) {
                                if (object_attribute.first == "opacity") {
                                    object_attribute.second = std::to_string(
                                        std::stof(attribute.second) *
                                        std::stof(object_attribute.second));
                                }
                            }
                        }
                    }
                }
            }
        }
        if (name == "g") {
            group++;
            group_attributes.push_back(object.second);
        }
    }
}

void Parser::printObjects() {
    for (auto object : objects) {
        std::cout << object.first << std::endl;
        for (auto attribute : object.second) {
            std::cout << "    " << attribute.first << " = " << attribute.second
                      << std::endl;
        }
    }
}

namespace {
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

std::string Parser::getAttribute(Attributes attributes, std::string name) {
    std::string result;
    for (auto attribute : attributes) {
        if (attribute.first == name) {
            result = attribute.second;
            break;
        } else {
            if (name == "fill")
                result = "black";
            else if (name == "stroke" || name == "transform" ||
                     name == "rotate")
                result = "none";
        }
    }
    return result;
}

float Parser::getFloatAttribute(Attributes attributes, std::string name) {
    float result;
    for (auto attribute : attributes) {
        if (attribute.first == name) {
            result = std::stof(attribute.second);
            break;
        } else {
            if (name == "stroke-width" || name == "stroke-opacity" ||
                name == "fill-opacity" || name == "opacity")
                result = 1;
        }
    }
    return result;
}

Color Parser::parseColor(Attributes attributes, std::string name) {
    std::string color = getAttribute(attributes, name);
    color.erase(std::remove(color.begin(), color.end(), ' '), color.end());
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

        result.a = result.a * getFloatAttribute(attributes, name + "-opacity") *
                   getFloatAttribute(attributes, "opacity");
        return result;
    }
}

std::vector< Vector2Df > Parser::parsePoints(Attributes attributes) {
    std::vector< Vector2Df > points;
    std::string points_string = getAttribute(attributes, "points");

    std::stringstream ss(points_string);
    float x, y;
    char comma;

    while (ss >> x >> comma >> y) {
        points.push_back(Vector2Df(x, y));
    }

    return points;
}

std::vector< std::string > Parser::getTransformOrder(Attributes attributes) {
    std::string transform_tag = getAttribute(attributes, "transform");

    transform_tag.erase(
        std::remove(transform_tag.begin(), transform_tag.end(), ' '),
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
    // for (auto type : transform_order) {
    //     if (type.find("translate") != std::string::npos) {
    //         float trans_x = getTranslate(type).first,
    //               trans_y = getTranslate(type).second;
    //         shape->translate(trans_x, trans_y);
    //     } else if (type.find("rotate") != std::string::npos) {
    //         float degree = getRotate(type);
    //         shape->rotate(degree);
    //     }
    // }
}

void Parser::parseLine(Attributes attributes) {
    std::vector< std::string > transform_order = getTransformOrder(attributes);
    Color stroke_color = parseColor(attributes, "stroke");
    float stroke_width = getFloatAttribute(attributes, "stroke-width");
    Line *shape = new Line(Vector2Df(getFloatAttribute(attributes, "x1"),
                                     getFloatAttribute(attributes, "y1")),
                           Vector2Df(getFloatAttribute(attributes, "x2"),
                                     getFloatAttribute(attributes, "y2")),
                           stroke_color, stroke_width);
    applyTransform(shape, transform_order);
    shapes.push_back(shape);
}

void Parser::parseRect(Attributes attributes) {
    std::vector< std::string > transform_order = getTransformOrder(attributes);
    Color stroke_color = parseColor(attributes, "stroke");
    Color fill_color = parseColor(attributes, "fill");
    float stroke_width = getFloatAttribute(attributes, "stroke-width");
    float x = getFloatAttribute(attributes, "x");
    float y = getFloatAttribute(attributes, "y");
    float rx = getFloatAttribute(attributes, "rx");
    float ry = getFloatAttribute(attributes, "ry");
    Rect *shape =
        new Rect(getFloatAttribute(attributes, "width"),
                 getFloatAttribute(attributes, "height"), Vector2Df(x, y),
                 Vector2Df(rx, ry), fill_color, stroke_color, stroke_width);
    applyTransform(shape, transform_order);
    shapes.push_back(shape);
}

void Parser::parseCircle(Attributes attributes) {
    std::vector< std::string > transform_order = getTransformOrder(attributes);
    Color stroke_color = parseColor(attributes, "stroke");
    Color fill_color = parseColor(attributes, "fill");
    float stroke_width = getFloatAttribute(attributes, "stroke-width");
    float cx = getFloatAttribute(attributes, "cx");
    float cy = getFloatAttribute(attributes, "cy");
    float radius = getFloatAttribute(attributes, "r");
    Circle *shape = new Circle(radius, Vector2Df(cx, cy), fill_color,
                               stroke_color, stroke_width);
    applyTransform(shape, transform_order);
    shapes.push_back(shape);
}

void Parser::parseEllipse(Attributes attributes) {
    std::vector< std::string > transform_order = getTransformOrder(attributes);
    Color stroke_color = parseColor(attributes, "stroke");
    Color fill_color = parseColor(attributes, "fill");
    float stroke_width = getFloatAttribute(attributes, "stroke-width");
    float radius_x = getFloatAttribute(attributes, "rx");
    float radius_y = getFloatAttribute(attributes, "ry");
    float cx = getFloatAttribute(attributes, "cx");
    float cy = getFloatAttribute(attributes, "cy");
    Ellipse *shape =
        new Ellipse(Vector2Df(radius_x, radius_y), Vector2Df(cx, cy),
                    fill_color, stroke_color, stroke_width);
    applyTransform(shape, transform_order);
    shapes.push_back(shape);
}

void Parser::parsePolygon(Attributes attributes) {
    Color stroke_color = parseColor(attributes, "stroke");
    Color fill_color = parseColor(attributes, "fill");
    float stroke_width = getFloatAttribute(attributes, "stroke-width");
    Polygon *shape = new Polygon(fill_color, stroke_color, stroke_width);
    std::vector< Vector2Df > points = parsePoints(attributes);
    for (auto point : points) {
        shape->addPoint(point);
    }
    shapes.push_back(shape);
}

void Parser::parsePolyline(Attributes attributes) {
    Color stroke_color = parseColor(attributes, "stroke");
    Color fill_color = parseColor(attributes, "fill");
    float stroke_width = getFloatAttribute(attributes, "stroke-width");
    Polyline *shape = new Polyline(fill_color, stroke_color, stroke_width);
    std::vector< Vector2Df > points = parsePoints(attributes);
    for (auto point : points) {
        shape->addPoint(point);
    }
    shapes.push_back(shape);
}

void Parser::parseText(Attributes attributes) {
    std::vector< std::string > transform_order = getTransformOrder(attributes);
    Color fill_color = parseColor(attributes, "fill");
    float x = getFloatAttribute(attributes, "x");
    float y = getFloatAttribute(attributes, "y");
    float font_size = getFloatAttribute(attributes, "font-size");
    std::string text = getAttribute(attributes, "text");
    Text *shape =
        new Text(Vector2Df(x, y - font_size), text, fill_color, font_size);
    applyTransform(shape, transform_order);
    shapes.push_back(shape);
}

void Parser::parsePath(Attributes attributes) {
    /*

    TBD: PATH

    */
}

void Parser::parseSVG() {
    for (auto object : objects) {
        std::string name = object.first;
        Attributes attributes = object.second;
        if (name == "line") {
            parseLine(attributes);
        } else if (name == "rect") {
            parseRect(attributes);
        } else if (name == "circle") {
            parseCircle(attributes);
        } else if (name == "ellipse") {
            parseEllipse(attributes);
        } else if (name == "polygon") {
            parsePolygon(attributes);
        } else if (name == "polyline") {
            parsePolyline(attributes);
        } else if (name == "text") {
            parseText(attributes);
        } else if (name == "path") {
            parsePath(attributes);
        }
    }
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

    for (auto object : objects) {
        object.second.clear();
    }

    objects.clear();

    delete instance;
}

void Parser::printShapesData() {
    for (auto shape : shapes) {
        shape->printData();
    }
}