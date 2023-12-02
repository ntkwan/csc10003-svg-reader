#include "Parser.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

Parser *Parser::instance = nullptr;

namespace {
    auto getHexColor = [](std::string color) -> Color {
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
            return Color(r, g, b, 255);
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
            return Color(r, g, b, a);
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
                return Color(r, g, b, a);
            }
            return Color(r, g, b, 255);
        }
    };

    auto getRgbColor = [](std::string color) -> Color {
        int r, g, b;
        float a = 1;
        sscanf(color.c_str(), "rgb(%d,%d,%d,%f)", &r, &g, &b, &a);
        return Color(r, g, b, 255 * a);
    };

    std::string removeExtraSpaces(const std::string &input) {
        std::string result;
        bool spaceDetected = false;
        bool firstSpace = true;
        for (char ch : input) {
            if (ch == ' ') {
                if (!spaceDetected) {
                    if (!firstSpace)
                        result.push_back(ch);
                    else
                        firstSpace = false;
                    spaceDetected = true;
                }
            } else {
                result.push_back(ch);
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

void Parser::printObjects() {
    for (auto object : objects) {
        std::cout << object.first << std::endl;
        for (auto attribute : object.second) {
            std::cout << "    " << attribute.first << " = " << attribute.second
                      << std::endl;
        }
    }
}

std::string Parser::parseSVG(const std::string &file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_name << std::endl;
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
        name.erase(std::remove(name.begin(), name.end(), '\t'), name.end());
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
        if (start_pos == std::string::npos) {
            continue;
        }
        if (line[start_pos + 1] == '!') {
            while (line.find("-->") == std::string::npos) {
                std::getline(ss, line);
            }
            continue;
        }
        if (line.find("text") != std::string::npos) {
            while (line.find("/text") == std::string::npos) {
                std::string next_line;
                std::getline(ss, next_line);
                line += " " + next_line;
            }
        }
        int end_pos = line.find(">");
        while (end_pos == std::string::npos) {
            std::string next_line;
            std::getline(ss, next_line);
            line += " " + next_line;
            end_pos = line.find(">");
        }
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
                text = removeExtraSpaces(text);
                attributes += " text=\"" + text + "\"";
            }
            tags.push_back(std::make_pair(name, attributes));
        }
    }
    return tags;
}

SVGElement *Parser::parseElements(std::string file_name) {
    std::string svg = parseSVG(file_name);
    Tags tags = parseTags(svg);

    for (auto tag : tags) {
        std::vector< std::pair< std::string, std::string > > attributes =
            parseAttributes(tag.second);
        objects.push_back(std::make_pair(tag.first, attributes));
    }

    SVGElement *root = new Group();
    SVGElement *current = root;

    for (auto &object : objects) {
        std::string name = object.first;
        Attributes attributes = object.second;
        if (name == "g") {
            Group *group = dynamic_cast< Group * >(current);
            for (auto group_attribute : group->getAttributes()) {
                bool found = false;
                for (auto &attribute : attributes) {
                    if (attribute.first == group_attribute.first) {
                        if (group_attribute.first == "opacity") {
                            attribute.second = std::to_string(
                                std::stof(attribute.second) *
                                std::stof(group_attribute.second));
                        } else if (group_attribute.first == "transform") {
                            attribute.second =
                                group_attribute.second + " " + attribute.second;
                        }
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    attributes.push_back(group_attribute);
                }
            }
            Group *new_group = new Group(attributes);
            current->addElement(new_group);
            current = new_group;
        } else if (name == "/g") {
            current = current->getParent();
        } else {
            Group *group = dynamic_cast< Group * >(current);
            for (auto group_attribute : group->getAttributes()) {
                bool found = false;
                for (auto &attribute : attributes) {
                    if (attribute.first == group_attribute.first) {
                        if (group_attribute.first == "opacity") {
                            attribute.second = std::to_string(
                                std::stof(attribute.second) *
                                std::stof(group_attribute.second));
                        } else if (group_attribute.first == "transform") {
                            attribute.second =
                                group_attribute.second + " " + attribute.second;
                        }
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    attributes.push_back(group_attribute);
                }
            }
            SVGElement *shape = parseShape(name, attributes);
            current->addElement(shape);
        }
    }
    return root;
}

SVGElement *Parser::parseShape(std::string shape, Attributes attributes) {
    if (shape == "line") {
        return parseLine(attributes);
    } else if (shape == "rect") {
        return parseRect(attributes);
    } else if (shape == "circle") {
        return parseCircle(attributes);
    } else if (shape == "ellipse") {
        return parseEllipse(attributes);
    } else if (shape == "polygon") {
        return parsePolygon(attributes);
    } else if (shape == "polyline") {
        return parsePolyline(attributes);
    } else if (shape == "text") {
        return parseText(attributes);
    } else if (shape == "path") {
        return parsePath(attributes);
    }
    return NULL;
}

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
        if (comma != ',') {
            std::string y_str = comma + std::to_string(y);
            y = std::stof(y_str);
        }
        points.push_back(Vector2Df(x, y));
    }

    return points;
}

std::vector< PathPoint > Parser::parsePathPoints(Attributes attributes) {
    std::vector< PathPoint > points;
    std::string path_string = getAttribute(attributes, "d");

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

Line *Parser::parseLine(Attributes attributes) {
    Color stroke_color = parseColor(attributes, "stroke");
    float stroke_width = getFloatAttribute(attributes, "stroke-width");
    Line *shape = new Line(Vector2Df(getFloatAttribute(attributes, "x1"),
                                     getFloatAttribute(attributes, "y1")),
                           Vector2Df(getFloatAttribute(attributes, "x2"),
                                     getFloatAttribute(attributes, "y2")),
                           stroke_color, stroke_width);
    shape->setTransforms(getTransformOrder(attributes));
    return shape;
}

Rect *Parser::parseRect(Attributes attributes) {
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
    shape->setTransforms(getTransformOrder(attributes));
    return shape;
}

Circle *Parser::parseCircle(Attributes attributes) {
    Color stroke_color = parseColor(attributes, "stroke");
    Color fill_color = parseColor(attributes, "fill");
    float stroke_width = getFloatAttribute(attributes, "stroke-width");
    float cx = getFloatAttribute(attributes, "cx");
    float cy = getFloatAttribute(attributes, "cy");
    float radius = getFloatAttribute(attributes, "r");
    Circle *shape = new Circle(radius, Vector2Df(cx, cy), fill_color,
                               stroke_color, stroke_width);
    shape->setTransforms(getTransformOrder(attributes));
    return shape;
}

Ellipse *Parser::parseEllipse(Attributes attributes) {
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
    shape->setTransforms(getTransformOrder(attributes));
    return shape;
}

Polygon *Parser::parsePolygon(Attributes attributes) {
    Color stroke_color = parseColor(attributes, "stroke");
    Color fill_color = parseColor(attributes, "fill");
    float stroke_width = getFloatAttribute(attributes, "stroke-width");
    Polygon *shape = new Polygon(fill_color, stroke_color, stroke_width);
    std::vector< Vector2Df > points = parsePoints(attributes);
    for (auto point : points) {
        shape->addPoint(point);
    }
    shape->setTransforms(getTransformOrder(attributes));
    return shape;
}

Polyline *Parser::parsePolyline(Attributes attributes) {
    Color stroke_color = parseColor(attributes, "stroke");
    Color fill_color = parseColor(attributes, "fill");
    float stroke_width = getFloatAttribute(attributes, "stroke-width");
    Polyline *shape = new Polyline(fill_color, stroke_color, stroke_width);
    std::vector< Vector2Df > points = parsePoints(attributes);
    for (auto point : points) {
        shape->addPoint(point);
    }
    shape->setTransforms(getTransformOrder(attributes));
    return shape;
}

Text *Parser::parseText(Attributes attributes) {
    Color stroke_color = parseColor(attributes, "stroke");
    Color fill_color = parseColor(attributes, "fill");
    float stroke_width = getFloatAttribute(attributes, "stroke-width");
    float x = getFloatAttribute(attributes, "x");
    float y = getFloatAttribute(attributes, "y");
    float font_size = getFloatAttribute(attributes, "font-size");
    std::string text = getAttribute(attributes, "text");
    Text *shape = new Text(Vector2Df(x, y - font_size), text, font_size,
                           fill_color, stroke_color, stroke_width);
    shape->setTransforms(getTransformOrder(attributes));
    return shape;
}

Path *Parser::parsePath(Attributes attributes) {
    Color stroke_color = parseColor(attributes, "stroke");
    Color fill_color = parseColor(attributes, "fill");
    float stroke_width = getFloatAttribute(attributes, "stroke-width");
    Path *shape = new Path(fill_color, stroke_color, stroke_width);
    std::vector< PathPoint > points = parsePathPoints(attributes);
    for (auto point : points) {
        shape->addPoint(point);
    }
    shape->setTransforms(getTransformOrder(attributes));
    return shape;
}

Parser::~Parser() {
    for (auto object : objects) {
        object.second.clear();
    }
    objects.clear();
    delete root;
    // delete instance;
}

void Parser::printShapesData() { root->printData(); }