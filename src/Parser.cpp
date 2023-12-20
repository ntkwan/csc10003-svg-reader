#include "Parser.hpp"

Parser *Parser::instance = nullptr;

namespace {
    auto getHexColor = [](std::string color) -> mColor {
        std::stringstream ss;
        int pos = color.find("#");
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

    void removeRedundantSpaces(std::string &path_string) {
        int index = 0;
        while (index < path_string.size()) {
            if ((index == 0 || index == path_string.size() - 1) &&
                path_string[index] == ' ') {
                path_string.erase(index, 1);
            } else if (path_string[index] == ' ' &&
                       path_string[index - 1] == ' ') {
                path_string.erase(index, 1);
            } else {
                index++;
            }
        }
    }

    void insertSpaceBeforeEachLetter(std::string &path_string) {
        std::string result;
        for (int index = 0; index < path_string.size(); index++) {
            if (std::isalpha(path_string[index])) {
                result += " ";
                result += path_string[index];
                result += " ";
            } else if (path_string[index] == '-') {
                result += " ";
                result += path_string[index];
            } else {
                result += path_string[index];
            }
        }
        path_string = result;
    }

    void formatSvgPathString(std::string &path_string) {
        std::replace(path_string.begin(), path_string.end(), '\t', ' ');
        std::replace(path_string.begin(), path_string.end(), '\n', ' ');
        insertSpaceBeforeEachLetter(path_string);
        std::replace(path_string.begin(), path_string.end(), ',', ' ');
        removeRedundantSpaces(path_string);
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
    std::string viewBox = getAttribute(svg, "viewBox");
    std::stringstream ss(viewBox);
    ss >> this->viewBox.first.x >> this->viewBox.first.y >>
        this->viewBox.second.x >> this->viewBox.second.y;
    xml_node<> *node = svg->first_node();
    xml_node<> *prev = NULL;

    SVGElement *root = new Group();
    SVGElement *current = root;

    while (node) {
        if (std::string(node->name()) == "defs") {
            GetGradients(node);
            prev = node;
            node = node->next_sibling();
        } else if (std::string(node->name()) == "g") {
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
                        }
                        found = true;
                        break;
                    }
                }
                if (!found && group_attribute.first != "transform") {
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
            new_group->setTransforms(getTransformOrder(node));
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
                        }
                        found = true;
                        break;
                    }
                }
                if (!found && group_attribute.first != "transform") {
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

std::string Parser::getAttribute(xml_node<> *node, std::string name) {
    if (name == "text") return removeExtraSpaces(node->value());
    std::string result;
    if (node->first_attribute(name.c_str()) == NULL) {
        if (name == "fill" || name == "stop-color")
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
        if (std::string(node->name()).find("Gradient") != std::string::npos) {
            if (name == "x1" || name == "y1" || name == "y2" || name == "fr")
                result = 0;
            else if (name == "cx" || name == "cy")
                result = name == "cx" ? 0.5 * this->viewBox.second.x
                                      : 0.5 * this->viewBox.second.y;
            else if (name == "r")
                result = sqrt((pow(this->viewBox.second.x, 2) +
                               pow(this->viewBox.second.y, 2)) /
                              2);
            else if (name == "fx" || name == "fy")
                result = name == "fx" ? getFloatAttribute(node, "cx")
                                      : getFloatAttribute(node, "cy");
            else
                result = this->viewBox.second.x;
        } else {
            if (name == "stroke-width" || name == "stroke-opacity" ||
                name == "fill-opacity" || name == "opacity" ||
                name == "stop-opacity")
                result = 1;
            else
                result = 0;
        }
    } else {
        result = std::stof(node->first_attribute(name.c_str())->value());
    }
    return result;
}

mColor Parser::parseColor(xml_node<> *node, std::string name, std::string &id) {
    std::string color = getAttribute(node, name);
    color.erase(std::remove(color.begin(), color.end(), ' '), color.end());
    if (color.find("url") == std::string::npos) {
        for (auto &c : color) c = tolower(c);
    }
    if (color == "none")
        return mColor::Transparent;
    else {
        mColor result;
        if (color.find("url") != std::string::npos) {
            if (color.find("'") != std::string::npos) {
                id = color.substr(color.find("'") + 1);
                id.erase(id.find("'"));
                id.erase(id.find("#"), 1);
            } else {
                id = color.substr(color.find("#") + 1);
                id.erase(id.find(")"));
            }
            result = mColor::Transparent;
        } else if (color.find("#") != std::string::npos) {
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
        if (name == "stop-color")
            result.a = result.a * getFloatAttribute(node, "stop-opacity");
        else
            result.a = result.a * getFloatAttribute(node, name + "-opacity") *
                       getFloatAttribute(node, "opacity");
        return result;
    }
}

Gradient *Parser::parseGradient(std::string id) {
    if (gradients.find(id) == gradients.end()) {
        std::cout << "Gradient " << id << " not found" << std::endl;
        exit(-1);
    }
    return gradients.at(id);
}

std::vector< Stop > Parser::getGradientStops(xml_node<> *node) {
    std::vector< Stop > stops;
    xml_node<> *stop_node = node->first_node();
    while (stop_node) {
        if (std::string(stop_node->name()) == "stop") {
            std::string id = "";
            mColor color = parseColor(stop_node, "stop-color", id);
            float offset = getFloatAttribute(stop_node, "offset");
            if (offset > 1) offset /= 100;
            stops.push_back(Stop(color, offset));
        }
        stop_node = stop_node->next_sibling();
    }
    return stops;
}

void Parser::GetGradients(xml_node<> *node) {
    xml_node<> *gradient_node = node->first_node();
    while (gradient_node) {
        if (std::string(gradient_node->name()) == "linearGradient") {
            std::string id = getAttribute(gradient_node, "id");
            float x1 = getFloatAttribute(gradient_node, "x1");
            float y1 = getFloatAttribute(gradient_node, "y1");
            float x2 = getFloatAttribute(gradient_node, "x2");
            float y2 = getFloatAttribute(gradient_node, "y2");
            std::vector< Stop > stops = getGradientStops(gradient_node);
            std::pair< Vector2Df, Vector2Df > points = {{x1, y1}, {x2, y2}};
            Gradient *gradient = new LinearGradient(stops, points);
            gradient->setTransforms(getTransformOrder(gradient_node));
            if (this->gradients.find(id) == this->gradients.end())
                this->gradients[id] = gradient;
        } else if (std::string(gradient_node->name()) == "radialGradient") {
            std::string id = getAttribute(gradient_node, "id");
            float cx = getFloatAttribute(gradient_node, "cx");
            float cy = getFloatAttribute(gradient_node, "cy");
            float fx = getFloatAttribute(gradient_node, "fx");
            float fy = getFloatAttribute(gradient_node, "fy");
            float r = getFloatAttribute(gradient_node, "r");
            float fr = getFloatAttribute(gradient_node, "fr");
            std::vector< Stop > stops = getGradientStops(gradient_node);
            std::pair< Vector2Df, Vector2Df > points = {{cx, cy}, {fx, fy}};
            Vector2Df radius(r, fr);
            Gradient *gradient = new RadialGradient(stops, points, radius);
            gradient->setTransforms(getTransformOrder(gradient_node));
            if (this->gradients.find(id) == this->gradients.end())
                this->gradients[id] = gradient;
        }
        gradient_node = gradient_node->next_sibling();
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
            pPoint.tc = element[0];
            if (tolower(pPoint.tc) == 'm' || tolower(pPoint.tc) == 'l' ||
                tolower(pPoint.tc) == 'c' || tolower(pPoint.tc) == 's' ||
                tolower(pPoint.tc) == 'q' || tolower(pPoint.tc) == 't' ||
                tolower(pPoint.tc) == 'a')
                ss >> pPoint.point.x >> pPoint.point.y;
            else if (tolower(pPoint.tc) == 'h') {
                ss >> pPoint.point.x;
                pPoint.point.y = 0;
            } else if (tolower(pPoint.tc) == 'v') {
                ss >> pPoint.point.y;
                pPoint.point.x = 0;
            }
        } else {
            if (tolower(pPoint.tc) == 'm' || tolower(pPoint.tc) == 'l' ||
                tolower(pPoint.tc) == 'c' || tolower(pPoint.tc) == 's' ||
                tolower(pPoint.tc) == 'q' || tolower(pPoint.tc) == 't' ||
                tolower(pPoint.tc) == 'a') {
                if (tolower(pPoint.tc) == 'm') pPoint.tc = 'L';
                pPoint.point.x = std::stof(element);
                ss >> pPoint.point.y;
            } else if (tolower(pPoint.tc) == 'h') {
                pPoint.point.x = std::stof(element);
                pPoint.point.y = 0;
            } else if (tolower(pPoint.tc) == 'v') {
                pPoint.point.y = std::stof(element);
                pPoint.point.x = 0;
            }
        }
        points.push_back(pPoint);
    }
    std::vector< PathPoint > handle_points;

    Vector2Df firstPoint{0, 0}, curPoint{0, 0};
    int n = points.size();
    for (int i = 0; i < n; i++) {
        if (tolower(points[i].tc) == 'm') {
            firstPoint = points[i].point;
            if (points[i].tc == 'm') {
                firstPoint.x = curPoint.x + points[i].point.x;
                firstPoint.y = curPoint.y + points[i].point.y;
            }
            curPoint = firstPoint;
            handle_points.push_back({firstPoint, 'm'});
        } else if (tolower(points[i].tc) == 'l' ||
                   tolower(points[i].tc) == 't') {
            Vector2Df endPoint{curPoint.x + points[i].point.x,
                               curPoint.y + points[i].point.y};
            if (points[i].tc == 'L' || points[i].tc == 'T')
                endPoint = points[i].point;
            curPoint = endPoint;
            char TC = tolower(points[i].tc);
            handle_points.push_back({endPoint, TC});
        } else if (tolower(points[i].tc) == 'h') {
            Vector2Df endPoint{curPoint.x + points[i].point.x, curPoint.y};
            if (points[i].tc == 'H')
                endPoint = Vector2Df{points[i].point.x, curPoint.y};
            curPoint = endPoint;
            handle_points.push_back({endPoint, 'h'});
        } else if (tolower(points[i].tc) == 'v') {
            Vector2Df endPoint{curPoint.x, curPoint.y + points[i].point.y};
            if (points[i].tc == 'V')
                endPoint = Vector2Df{curPoint.x, points[i].point.y};
            curPoint = endPoint;
            handle_points.push_back({endPoint, 'v'});
        } else if (tolower(points[i].tc) == 'c') {
            if (i + 2 < n) {
                Vector2Df controlPoint1 =
                    Vector2Df{curPoint.x + points[i].point.x,
                              curPoint.y + points[i].point.y};
                Vector2Df controlPoint2 =
                    Vector2Df{curPoint.x + points[i + 1].point.x,
                              curPoint.y + points[i + 1].point.y};
                Vector2Df controlPoint3 =
                    Vector2Df{curPoint.x + points[i + 2].point.x,
                              curPoint.y + points[i + 2].point.y};
                if (points[i].tc == 'C') {
                    controlPoint1 = points[i].point;
                    controlPoint2 = points[i + 1].point;
                    controlPoint3 = points[i + 2].point;
                }
                i += 2;
                curPoint = controlPoint3;
                handle_points.push_back({controlPoint1, 'c'});
                handle_points.push_back({controlPoint2, 'c'});
                handle_points.push_back({controlPoint3, 'c'});
            }
        } else if (tolower(points[i].tc) == 'z') {
            curPoint = firstPoint;
            handle_points.push_back({firstPoint, 'z'});
        } else if (tolower(points[i].tc) == 's' ||
                   tolower(points[i].tc) == 'q') {
            if (i + 1 < n) {
                Vector2Df controlPoint1 =
                    Vector2Df{curPoint.x + points[i].point.x,
                              curPoint.y + points[i].point.y};
                Vector2Df controlPoint2 =
                    Vector2Df{curPoint.x + points[i + 1].point.x,
                              curPoint.y + points[i + 1].point.y};
                if (points[i].tc == 'S' || points[i].tc == 'Q') {
                    controlPoint1 = points[i].point;
                    controlPoint2 = points[i + 1].point;
                }
                i += 1;
                curPoint = controlPoint2;
                char TC = tolower(points[i].tc);
                handle_points.push_back({controlPoint1, TC});
                handle_points.push_back({controlPoint2, TC});
            }
        }
    }
    return handle_points;
}

std::vector< std::string > Parser::getTransformOrder(xml_node<> *node) {
    std::string transform_tag;
    if (std::string(node->name()).find("Gradient") != std::string::npos)
        transform_tag = getAttribute(node, "gradientTransform");
    else
        transform_tag = getAttribute(node, "transform");
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

SVGElement *Parser::parseShape(xml_node<> *node) {
    SVGElement *shape = NULL;
    std::string type = node->name();
    std::string id = "";
    mColor stroke_color = parseColor(node, "stroke", id);
    mColor fill_color = parseColor(node, "fill", id);
    float stroke_width = getFloatAttribute(node, "stroke-width");
    if (type == "line") {
        shape = parseLine(node, stroke_color, stroke_width);
    } else if (type == "rect") {
        shape = parseRect(node, fill_color, stroke_color, stroke_width);
    } else if (type == "circle") {
        shape = parseCircle(node, fill_color, stroke_color, stroke_width);
    } else if (type == "ellipse") {
        shape = parseEllipse(node, fill_color, stroke_color, stroke_width);
    } else if (type == "polygon") {
        shape = parsePolygon(node, fill_color, stroke_color, stroke_width);
    } else if (type == "polyline") {
        shape = parsePolyline(node, fill_color, stroke_color, stroke_width);
    } else if (type == "path") {
        shape = parsePath(node, fill_color, stroke_color, stroke_width);
    } else if (type == "text") {
        return parseText(node, fill_color, stroke_color, stroke_width);
    }
    if (shape != NULL) {
        shape->setTransforms(getTransformOrder(node));
        if (id != "") {
            shape->setGradient(parseGradient(id));
        }
    }
    return shape;
}

Line *Parser::parseLine(xml_node<> *node, const mColor &stroke_color,
                        float stroke_width) {
    Line *shape = new Line(
        Vector2Df(getFloatAttribute(node, "x1"), getFloatAttribute(node, "y1")),
        Vector2Df(getFloatAttribute(node, "x2"), getFloatAttribute(node, "y2")),
        stroke_color, stroke_width);
    return shape;
}

Rect *Parser::parseRect(xml_node<> *node, const mColor &fill_color,
                        const mColor &stroke_color, float stroke_width) {
    float x = getFloatAttribute(node, "x");
    float y = getFloatAttribute(node, "y");
    float rx = getFloatAttribute(node, "rx");
    float ry = getFloatAttribute(node, "ry");
    Rect *shape =
        new Rect(getFloatAttribute(node, "width"),
                 getFloatAttribute(node, "height"), Vector2Df(x, y),
                 Vector2Df(rx, ry), fill_color, stroke_color, stroke_width);
    return shape;
}

Circle *Parser::parseCircle(xml_node<> *node, const mColor &fill_color,
                            const mColor &stroke_color, float stroke_width) {
    float cx = getFloatAttribute(node, "cx");
    float cy = getFloatAttribute(node, "cy");
    float radius = getFloatAttribute(node, "r");
    Circle *shape = new Circle(radius, Vector2Df(cx, cy), fill_color,
                               stroke_color, stroke_width);
    return shape;
}

Ell *Parser::parseEllipse(xml_node<> *node, const mColor &fill_color,
                          const mColor &stroke_color, float stroke_width) {
    float radius_x = getFloatAttribute(node, "rx");
    float radius_y = getFloatAttribute(node, "ry");
    float cx = getFloatAttribute(node, "cx");
    float cy = getFloatAttribute(node, "cy");
    Ell *shape = new Ell(Vector2Df(radius_x, radius_y), Vector2Df(cx, cy),
                         fill_color, stroke_color, stroke_width);
    return shape;
}

Plygon *Parser::parsePolygon(xml_node<> *node, const mColor &fill_color,
                             const mColor &stroke_color, float stroke_width) {
    Plygon *shape = new Plygon(fill_color, stroke_color, stroke_width);
    std::vector< Vector2Df > points = parsePoints(node);
    for (auto point : points) {
        shape->addPoint(point);
    }
    std::string fill_rule = getAttribute(node, "fill-rule");
    fill_rule.erase(std::remove(fill_rule.begin(), fill_rule.end(), ' '),
                    fill_rule.end());
    shape->setFillRule(fill_rule);
    return shape;
}

Plyline *Parser::parsePolyline(xml_node<> *node, const mColor &fill_color,
                               const mColor &stroke_color, float stroke_width) {
    Plyline *shape = new Plyline(fill_color, stroke_color, stroke_width);
    std::vector< Vector2Df > points = parsePoints(node);
    for (auto point : points) {
        shape->addPoint(point);
    }
    std::string fill_rule = getAttribute(node, "fill-rule");
    fill_rule.erase(std::remove(fill_rule.begin(), fill_rule.end(), ' '),
                    fill_rule.end());
    shape->setFillRule(fill_rule);
    return shape;
}

Text *Parser::parseText(xml_node<> *node, const mColor &fill_color,
                        const mColor &stroke_color, float stroke_width) {
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

Path *Parser::parsePath(xml_node<> *node, const mColor &fill_color,
                        const mColor &stroke_color, float stroke_width) {
    Path *shape = new Path(fill_color, stroke_color, stroke_width);
    std::vector< PathPoint > points = parsePathPoints(node);
    for (auto point : points) {
        shape->addPoint(point);
    }
    std::string fill_rule = getAttribute(node, "fill-rule");
    fill_rule.erase(std::remove(fill_rule.begin(), fill_rule.end(), ' '),
                    fill_rule.end());
    shape->setFillRule(fill_rule);
    return shape;
}

Parser::~Parser() {
    delete root;
    for (auto gradient : gradients) {
        delete gradient.second;
    }
}

void Parser::printShapesData() { root->printData(); }