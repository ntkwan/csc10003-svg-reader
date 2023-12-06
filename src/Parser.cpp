#include "Parser.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

Parser *Parser::instance = nullptr;

namespace {
    auto getHexColor = [](std::string color) -> mColor {
        std::stringstream ss;
        int pos = color.find("#");
        // handle 3 digit hex color
        if (color.size() < 5 || color[pos + 4] == ' ') {
            ss << std::hex << color.substr(pos + 1, 1) << " "
               << color.substr(pos + 2, 1) << " " << color.substr(pos + 3, 1);
            int r, g, b;
            ss >> r >> g >> b;
            r = r * 16 + r;
            g = g * 16 + g;
            b = b * 16 + b;
            return mColor(r, g, b, 255);
        } else if (color.size() < 6 || color[pos + 5] == ' ') {
            ss << std::hex << color.substr(pos + 1, 1) << " "
               << color.substr(pos + 2, 1) << " " << color.substr(pos + 3, 1)
               << color.substr(pos + 4, 1);
            int r, g, b, a;
            ss >> r >> g >> b >> a;
            r = r * 16 + r;
            g = g * 16 + g;
            b = b * 16 + b;
            a = a * 16 + a;
            return mColor(r, g, b, a);
        } else {
            ss << std::hex << color.substr(pos + 1, 2) << " "
               << color.substr(pos + 3, 2) << " " << color.substr(pos + 5, 2);
            int r, g, b;
            ss >> r >> g >> b;
            if (color[pos + 7] != '\0' && color[pos + 7] != ' ') {
                std::stringstream ss;
                ss << std::hex << color.substr(pos + 7, 2);
                int a;
                ss >> a;
                return mColor(r, g, b, a);
            }
            return mColor(r, g, b, 255);
        }
    };

    auto getRgbColor = [](std::string color) -> mColor {
        int r, g, b;
        float a = 1;
        sscanf(color.c_str(), "rgb(%d,%d,%d,%f)", &r, &g, &b, &a);
        return mColor(r, g, b, 255 * a);
    };

    std::string removeExtraSpaces(std::string input) {
        input.erase(std::remove(input.begin(), input.end(), '\t'), input.end());
        input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
        std::string result;
        bool spaceDetected = false;
        bool firstSpace = true;
        for (int i = 0; i < input.size(); i++) {
            if (input[i] == ' ') {
                if (!spaceDetected) {
                    if (!firstSpace)
                        result.push_back(input[i]);
                    else
                        firstSpace = false;
                    spaceDetected = true;
                }
            } else {
                result.push_back(input[i]);
                firstSpace = false;
                spaceDetected = false;
            }
        }

        if (!result.empty() && result.back() == ' ') {
            result.pop_back();
        }

        return result;
    }

    void removeRedundantSpaces(std::string &svgPathString) {
        int index = 0;
        while (index < svgPathString.size()) {
            if ((index == 0 || index == svgPathString.size() - 1) &&
                svgPathString[index] == ' ') {
                svgPathString.erase(index, 1);
            } else if (svgPathString[index] == ' ' &&
                       svgPathString[index - 1] == ' ') {
                svgPathString.erase(index, 1);
            } else {
                index++;
            }
        }
    }

    void insertSpaceBeforeEachLetter(std::string &svgPathString) {
        std::string result;
        for (int index = 0; index < svgPathString.size(); index++) {
            if (std::isalpha(svgPathString[index])) {
                result += " ";
                result += svgPathString[index];
                result += " ";
            } else if (svgPathString[index] == '-') {
                result += " ";
                result += svgPathString[index];
            } else {
                result += svgPathString[index];
            }
        }
        svgPathString = result;
    }

    void formatSvgPathString(std::string &svgPathString) {
        std::replace(svgPathString.begin(), svgPathString.end(), '\t', ' ');
        std::replace(svgPathString.begin(), svgPathString.end(), '\n', ' ');
        insertSpaceBeforeEachLetter(svgPathString);
        std::replace(svgPathString.begin(), svgPathString.end(), ',', ' ');
        removeRedundantSpaces(svgPathString);
    }
}  // namespace

Parser *Parser::getInstance(const std::string &file_name) {
    if (instance == nullptr) {
        instance = new Parser(file_name);
    }
    return instance;
}

Parser::Parser(const std::string &file_name) {
    root = parseElements(file_name);
}

Group *Parser::getRoot() { return dynamic_cast< Group * >(root); }

Attributes xmlToString(xml_attribute<> *attribute) {
    Attributes attributes;
    while (attribute) {
        attributes.push_back(
            std::make_pair(attribute->name(), attribute->value()));
        attribute = attribute->next_attribute();
    }
    return attributes;
}

SVGElement *Parser::parseElements(std::string file_name) {
    xml_document<> doc;
    std::ifstream file(file_name);
    std::vector< char > buffer((std::istreambuf_iterator< char >(file)),
                               std::istreambuf_iterator< char >());
    buffer.push_back('\0');
    doc.parse< 0 >(&buffer[0]);

    xml_node<> *svg = doc.first_node();
    xml_node<> *node = svg->first_node();
    xml_node<> *prev = NULL;

    SVGElement *root = new Group();
    SVGElement *current = root;

    while (node) {
        if (std::string(node->name()) == "g") {
            Group *group = dynamic_cast< Group * >(current);
            for (auto group_attribute : group->getAttributes()) {
                bool found = false;
                for (auto attribute = node->first_attribute(); attribute;
                     attribute = attribute->next_attribute()) {
                    if (std::string(attribute->name()) ==
                        group_attribute.first) {
                        if (group_attribute.first == "opacity") {
                            std::string opacity = std::to_string(
                                std::stof(attribute->value()) *
                                std::stof(group_attribute.second));
                            char *value = doc.allocate_string(opacity.c_str());
                            attribute->value(value);
                        } else if (group_attribute.first == "transform") {
                            std::string transform = group_attribute.second +
                                                    " " + attribute->value();
                            char *value =
                                doc.allocate_string(transform.c_str());
                            attribute->value(value);
                        }
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    char *name =
                        doc.allocate_string(group_attribute.first.c_str());
                    char *value =
                        doc.allocate_string(group_attribute.second.c_str());
                    xml_attribute<> *new_attribute =
                        doc.allocate_attribute(name, value);
                    node->append_attribute(new_attribute);
                }
            }
            Group *new_group = new Group(xmlToString(node->first_attribute()));
            current->addElement(new_group);
            current = new_group;
            prev = node;
            node = node->first_node();
        } else {
            Group *group = dynamic_cast< Group * >(current);
            for (auto group_attribute : group->getAttributes()) {
                bool found = false;
                for (auto attribute = node->first_attribute(); attribute;
                     attribute = attribute->next_attribute()) {
                    if (std::string(attribute->name()) ==
                        group_attribute.first) {
                        if (group_attribute.first == "opacity") {
                            std::string opacity = std::to_string(
                                std::stof(attribute->value()) *
                                std::stof(group_attribute.second));
                            char *value = doc.allocate_string(opacity.c_str());
                            attribute->value(value);
                        } else if (group_attribute.first == "transform") {
                            std::string transform = group_attribute.second +
                                                    " " + attribute->value();
                            char *value =
                                doc.allocate_string(transform.c_str());
                            attribute->value(value);
                        }
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    char *name =
                        doc.allocate_string(group_attribute.first.c_str());
                    char *value =
                        doc.allocate_string(group_attribute.second.c_str());
                    xml_attribute<> *new_attribute =
                        doc.allocate_attribute(name, value);
                    node->append_attribute(new_attribute);
                }
            }
            SVGElement *shape = parseShape(node);
            if (shape != NULL) current->addElement(shape);
            prev = node;
            node = node->next_sibling();
        }
        if (node == NULL && current != root) {
            while (prev->parent()->next_sibling() == NULL) {
                current = current->getParent();
                prev = prev->parent();
                if (prev == svg) {
                    break;
                }
            }
            if (prev == svg) {
                break;
            }
            current = current->getParent();
            node = prev->parent()->next_sibling();
        }
    }
    return root;
}

SVGElement *Parser::parseShape(xml_node<> *node) {
    std::string shape = node->name();
    if (shape == "line") {
        return parseLine(node);
    } else if (shape == "rect") {
        return parseRect(node);
    } else if (shape == "circle") {
        return parseCircle(node);
    } else if (shape == "ellipse") {
        return parseEllipse(node);
    } else if (shape == "polygon") {
        return parsePolygon(node);
    } else if (shape == "polyline") {
        return parsePolyline(node);
    } else if (shape == "text") {
        return parseText(node);
    } else if (shape == "path") {
        return parsePath(node);
    }
    return NULL;
}

std::string Parser::getAttribute(xml_node<> *node, std::string name) {
    if (name == "text") return removeExtraSpaces(node->value());
    std::string result;
    if (node->first_attribute(name.c_str()) == NULL) {
        if (name == "fill")
            result = "black";
        else if (name == "stroke" || name == "transform" || name == "rotate" ||
                 name == "font-style")
            result = "none";
        else if (name == "text-anchor")
            result = "start";
        else if (name == "fill-rule")
            result = "nonzero";
    } else {
        result = node->first_attribute(name.c_str())->value();
    }
    return result;
}

float Parser::getFloatAttribute(xml_node<> *node, std::string name) {
    float result;
    if (node->first_attribute(name.c_str()) == NULL) {
        if (name == "stroke-width" || name == "stroke-opacity" ||
            name == "fill-opacity" || name == "opacity")
            result = 1;
        else
            result = 0;
    } else {
        result = std::stof(node->first_attribute(name.c_str())->value());
    }
    return result;
}

mColor Parser::parseColor(xml_node<> *node, std::string name) {
    std::string color = getAttribute(node, name);
    color.erase(std::remove(color.begin(), color.end(), ' '), color.end());
    for (auto &c : color) c = tolower(c);
    if (color == "none")
        return mColor::Transparent;
    else {
        mColor result;
        if (color.find("#") != std::string::npos) {
            result = getHexColor(color);
        } else if (color.find("rgb") != std::string::npos) {
            result = getRgbColor(color);
        } else {
            auto color_code = color_map.find(color);
            if (color_code == color_map.end()) {
                std::cout << "Color " << color << " not found" << std::endl;
                exit(-1);
            }
            result = color_code->second;
        }

        result.a = result.a * getFloatAttribute(node, name + "-opacity") *
                   getFloatAttribute(node, "opacity");
        return result;
    }
}

std::vector< Vector2Df > Parser::parsePoints(xml_node<> *node) {
    std::vector< Vector2Df > points;
    std::string points_string = getAttribute(node, "points");

    std::stringstream ss(points_string);
    float x, y;

    while (ss >> x) {
        if (ss.peek() == ',') ss.ignore();
        ss >> y;
        points.push_back(Vector2Df(x, y));
    }

    return points;
}

std::vector< PathPoint > Parser::parsePathPoints(xml_node<> *node) {
    std::vector< PathPoint > points;
    std::string path_string = getAttribute(node, "d");

    formatSvgPathString(path_string);
    std::stringstream ss(path_string);
    std::string element;
    PathPoint pPoint{{0, 0}, 'M'};

    while (ss >> element) {
        if (std::isalpha(element[0])) {
            pPoint.TC = element[0];
            if (tolower(pPoint.TC) == 'm' || tolower(pPoint.TC) == 'l' ||
                tolower(pPoint.TC) == 'c')
                ss >> pPoint.Point.x >> pPoint.Point.y;
            else if (tolower(pPoint.TC) == 'h') {
                ss >> pPoint.Point.x;
                pPoint.Point.y = 0;
            } else if (tolower(pPoint.TC) == 'v') {
                ss >> pPoint.Point.y;
                pPoint.Point.x = 0;
            }
        } else {
            if (tolower(pPoint.TC) == 'm' || tolower(pPoint.TC) == 'l' ||
                tolower(pPoint.TC) == 'c') {
                if (tolower(pPoint.TC) == 'm') pPoint.TC = 'L';
                pPoint.Point.x = std::stof(element);
                ss >> pPoint.Point.y;
            } else if (tolower(pPoint.TC) == 'h') {
                pPoint.Point.x = std::stof(element);
                pPoint.Point.y = 0;
            } else if (tolower(pPoint.TC) == 'v') {
                pPoint.Point.y = std::stof(element);
                pPoint.Point.x = 0;
            }
        }
        points.push_back(pPoint);
    }

    return points;
}

std::vector< std::string > Parser::getTransformOrder(xml_node<> *node) {
    std::string transform_tag = getAttribute(node, "transform");
    std::vector< std::string > order;
    std::stringstream ss(transform_tag);
    std::string type;
    while (ss >> type) {
        if (type.find("translate") != std::string::npos ||
            type.find("scale") != std::string::npos ||
            type.find("rotate") != std::string::npos) {
            while (type.find(")") == std::string::npos) {
                std::string temp;
                ss >> temp;
                type += " " + temp;
            }
            std::string temp = type.substr(0, type.find("(") + 1);
            temp.erase(std::remove(temp.begin(), temp.end(), ' '), temp.end());
            type.erase(0, type.find("(") + 1);
            type = temp + type;
            order.push_back(type);
        }
    }
    return order;
}

Line *Parser::parseLine(xml_node<> *node) {
    mColor stroke_color = parseColor(node, "stroke");
    float stroke_width = getFloatAttribute(node, "stroke-width");
    Line *shape = new Line(
        Vector2Df(getFloatAttribute(node, "x1"), getFloatAttribute(node, "y1")),
        Vector2Df(getFloatAttribute(node, "x2"), getFloatAttribute(node, "y2")),
        stroke_color, stroke_width);
    shape->setTransforms(getTransformOrder(node));
    return shape;
}

Rect *Parser::parseRect(xml_node<> *node) {
    mColor stroke_color = parseColor(node, "stroke");
    mColor fill_color = parseColor(node, "fill");
    float stroke_width = getFloatAttribute(node, "stroke-width");
    float x = getFloatAttribute(node, "x");
    float y = getFloatAttribute(node, "y");
    float rx = getFloatAttribute(node, "rx");
    float ry = getFloatAttribute(node, "ry");
    Rect *shape =
        new Rect(getFloatAttribute(node, "width"),
                 getFloatAttribute(node, "height"), Vector2Df(x, y),
                 Vector2Df(rx, ry), fill_color, stroke_color, stroke_width);
    shape->setTransforms(getTransformOrder(node));
    return shape;
}

Circle *Parser::parseCircle(xml_node<> *node) {
    mColor stroke_color = parseColor(node, "stroke");
    mColor fill_color = parseColor(node, "fill");
    float stroke_width = getFloatAttribute(node, "stroke-width");
    float cx = getFloatAttribute(node, "cx");
    float cy = getFloatAttribute(node, "cy");
    float radius = getFloatAttribute(node, "r");
    Circle *shape = new Circle(radius, Vector2Df(cx, cy), fill_color,
                               stroke_color, stroke_width);
    shape->setTransforms(getTransformOrder(node));
    return shape;
}

Ell *Parser::parseEllipse(xml_node<> *node) {
    mColor stroke_color = parseColor(node, "stroke");
    mColor fill_color = parseColor(node, "fill");
    float stroke_width = getFloatAttribute(node, "stroke-width");
    float radius_x = getFloatAttribute(node, "rx");
    float radius_y = getFloatAttribute(node, "ry");
    float cx = getFloatAttribute(node, "cx");
    float cy = getFloatAttribute(node, "cy");
    Ell *shape = new Ell(Vector2Df(radius_x, radius_y), Vector2Df(cx, cy),
                         fill_color, stroke_color, stroke_width);
    shape->setTransforms(getTransformOrder(node));
    return shape;
}

Plygon *Parser::parsePolygon(xml_node<> *node) {
    mColor stroke_color = parseColor(node, "stroke");
    mColor fill_color = parseColor(node, "fill");
    float stroke_width = getFloatAttribute(node, "stroke-width");
    Plygon *shape = new Plygon(fill_color, stroke_color, stroke_width);
    std::vector< Vector2Df > points = parsePoints(node);
    for (auto point : points) {
        shape->addPoint(point);
    }
    std::string fill_rule = getAttribute(node, "fill-rule");
    fill_rule.erase(std::remove(fill_rule.begin(), fill_rule.end(), ' '),
                    fill_rule.end());
    shape->setFillRule(fill_rule);
    shape->setTransforms(getTransformOrder(node));
    return shape;
}

Plyline *Parser::parsePolyline(xml_node<> *node) {
    mColor stroke_color = parseColor(node, "stroke");
    mColor fill_color = parseColor(node, "fill");
    float stroke_width = getFloatAttribute(node, "stroke-width");
    Plyline *shape = new Plyline(fill_color, stroke_color, stroke_width);
    std::vector< Vector2Df > points = parsePoints(node);
    for (auto point : points) {
        shape->addPoint(point);
    }
    std::string fill_rule = getAttribute(node, "fill-rule");
    fill_rule.erase(std::remove(fill_rule.begin(), fill_rule.end(), ' '),
                    fill_rule.end());
    shape->setFillRule(fill_rule);
    shape->setTransforms(getTransformOrder(node));
    return shape;
}

Text *Parser::parseText(xml_node<> *node) {
    mColor stroke_color = parseColor(node, "stroke");
    mColor fill_color = parseColor(node, "fill");
    float stroke_width = getFloatAttribute(node, "stroke-width");
    float x = getFloatAttribute(node, "x");
    float y = getFloatAttribute(node, "y");
    float font_size = getFloatAttribute(node, "font-size");
    std::string text = getAttribute(node, "text");
    Text *shape = new Text(Vector2Df(x - 7, y - font_size + 5), text, font_size,
                           fill_color, stroke_color, stroke_width);
    std::string anchor = getAttribute(node, "text-anchor");
    anchor.erase(std::remove(anchor.begin(), anchor.end(), ' '), anchor.end());
    shape->setAnchor(anchor);
    std::string style = getAttribute(node, "font-style");
    style.erase(std::remove(style.begin(), style.end(), ' '), style.end());
    shape->setFontStyle(style);
    float dx = getFloatAttribute(node, "dx");
    float dy = getFloatAttribute(node, "dy");
    std::string transform =
        "translate(" + std::to_string(dx) + " " + std::to_string(dy) + ")";
    std::vector< std::string > transform_order = getTransformOrder(node);
    transform_order.push_back(transform);
    shape->setTransforms(transform_order);
    return shape;
}

Path *Parser::parsePath(xml_node<> *node) {
    mColor stroke_color = parseColor(node, "stroke");
    mColor fill_color = parseColor(node, "fill");
    float stroke_width = getFloatAttribute(node, "stroke-width");
    Path *shape = new Path(fill_color, stroke_color, stroke_width);
    std::vector< PathPoint > points = parsePathPoints(node);
    for (auto point : points) {
        shape->addPoint(point);
    }
    std::string fill_rule = getAttribute(node, "fill-rule");
    fill_rule.erase(std::remove(fill_rule.begin(), fill_rule.end(), ' '),
                    fill_rule.end());
    shape->setFillRule(fill_rule);
    shape->setTransforms(getTransformOrder(node));
    return shape;
}

Parser::~Parser() { delete root; }

void Parser::printShapesData() { root->printData(); }