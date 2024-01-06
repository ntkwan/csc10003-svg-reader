#include "Parser.hpp"

Parser *Parser::instance = nullptr;

namespace {
    // Lambda function to convert a hex color string to ColorShape
    auto getHexColor = [](std::string color) -> ColorShape {
        std::stringstream ss;
        int pos = color.find("#");
        if (color.size() < 5 || color[pos + 4] == ' ') {
            // Parse 3-digit hex color format
            ss << std::hex << color.substr(pos + 1, 1) << " "
               << color.substr(pos + 2, 1) << " " << color.substr(pos + 3, 1);
            int r, g, b;
            ss >> r >> g >> b;
            r = r * 16 + r;
            g = g * 16 + g;
            b = b * 16 + b;
            return ColorShape(r, g, b, 255);
        } else if (color.size() < 6 || color[pos + 5] == ' ') {
            // Parse 4-digit hex color format (with alpha channel)
            ss << std::hex << color.substr(pos + 1, 1) << " "
               << color.substr(pos + 2, 1) << " " << color.substr(pos + 3, 1)
               << color.substr(pos + 4, 1);
            int r, g, b, a;
            ss >> r >> g >> b >> a;
            r = r * 16 + r;
            g = g * 16 + g;
            b = b * 16 + b;
            a = a * 16 + a;
            return ColorShape(r, g, b, a);
        } else {
            // Parse 6-digit hex color format (with optional alpha channel)
            ss << std::hex << color.substr(pos + 1, 2) << " "
               << color.substr(pos + 3, 2) << " " << color.substr(pos + 5, 2);
            int r, g, b;
            ss >> r >> g >> b;
            if (color[pos + 7] != '\0' && color[pos + 7] != ' ') {
                // Parse alpha channel if present
                std::stringstream ss;
                ss << std::hex << color.substr(pos + 7, 2);
                int a;
                ss >> a;
                return ColorShape(r, g, b, a);
            }
            return ColorShape(r, g, b, 255);
        }
    };

    // Lambda function to convert an RGB color string to ColorShape
    auto getRgbColor = [](std::string color) -> ColorShape {
        int r, g, b;
        float a = 1;
        sscanf(color.c_str(), "rgb(%d,%d,%d,%f)", &r, &g, &b, &a);
        return ColorShape(r, g, b, 255 * a);
    };

    // Remove extra spaces, tabs, and newlines from a string
    std::string removeExtraSpaces(std::string input) {
        input.erase(std::remove(input.begin(), input.end(), '\t'), input.end());
        input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
        std::string result;
        bool space_detected = false;
        bool first_space = true;
        for (int i = 0; i < input.size(); i++) {
            if (input[i] == ' ') {
                if (!space_detected) {
                    if (!first_space)
                        result.push_back(input[i]);
                    else
                        first_space = false;
                    space_detected = true;
                }
            } else {
                result.push_back(input[i]);
                first_space = false;
                space_detected = false;
            }
        }

        if (!result.empty() && result.back() == ' ') {
            result.pop_back();
        }

        return result;
    }

    // Remove redundant consecutive spaces from a string
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

    // Insert space before each letter in a string, except 'e'
    void insertSpaceBeforeEachLetter(std::string &path_string) {
        std::string result;
        for (int index = 0; index < path_string.size(); index++) {
            if (std::isalpha(path_string[index]) &&
                tolower(path_string[index]) != 'e') {
                result += " ";
                result += path_string[index];
                result += " ";
            } else if (path_string[index] == '-' && index - 1 >= 0 &&
                       tolower(path_string[index - 1]) != 'e') {
                result += " ";
                result += path_string[index];
            } else if (path_string[index] == '.') {
                if (index > 0 && path_string[index - 1] == '-')
                    result += "0";
                else if (index > 0 && isalpha(path_string[index - 1]))
                    result += " 0";
                result += path_string[index];
            } else {
                result += path_string[index];
            }
        }
        path_string = result;
    }

    // Format SVG path string by removing extra spaces and handling
    // abbreviations
    void formatSvgPathString(std::string &path_string) {
        std::replace(path_string.begin(), path_string.end(), '\t', ' ');
        std::replace(path_string.begin(), path_string.end(), '\n', ' ');
        insertSpaceBeforeEachLetter(path_string);
        std::replace(path_string.begin(), path_string.end(), ',', ' ');
        removeRedundantSpaces(path_string);

        // Check for and handle abbreviations in the SVG path string
        auto checkAbbreviation = [](const std::string &s) {
            int cnt = 0;
            for (auto c : s)
                if (c == '.') ++cnt;
            if (cnt == 2) return true;
            return false;
        };

        std::stringstream ss(path_string);
        std::string element;
        std::string result;
        while (ss >> element) {
            std::string point_x = "";
            std::string point_y = "";
            if (checkAbbreviation(element)) {
                for (int i = (int)element.size() - 1; i >= 0; --i) {
                    if (element[i] == '.') {
                        point_y = "0." + element.substr(i + 1);
                        point_x = element.substr(0, i);
                        break;
                    }
                }
                result += point_x + ' ' + point_y + ' ';
            } else
                result += element + ' ';
        }
        path_string = result;
    }
}  // namespace

// Singleton design pattern: Parser class instance creation
Parser *Parser::getInstance(const std::string &file_name) {
    if (instance == nullptr) {
        instance = new Parser(file_name);
    }
    return instance;
}

// Constructor for the Parser class
Parser::Parser(const std::string &file_name) {
    root = parseElements(file_name);
}

// Get the root element of the SVG document
Group *Parser::getRoot() { return dynamic_cast< Group * >(root); }

// Convert XML attribute to string representation
Attributes xmlToString(rapidxml::xml_attribute<> *attribute) {
    Attributes attributes;
    while (attribute) {
        attributes.push_back(
            std::make_pair(attribute->name(), attribute->value()));
        attribute = attribute->next_attribute();
    }
    return attributes;
}

// Parse SVG elements from the XML document
SVGElement *Parser::parseElements(std::string file_name) {
    rapidxml::xml_document<> doc;
    std::ifstream file(file_name);
    std::vector< char > buffer((std::istreambuf_iterator< char >(file)),
                               std::istreambuf_iterator< char >());
    buffer.push_back('\0');
    doc.parse< 0 >(&buffer[0]);

    rapidxml::xml_node<> *svg = doc.first_node();
    viewport.x = getFloatAttribute(svg, "width");
    viewport.y = getFloatAttribute(svg, "height");
    std::string viewbox = getAttribute(svg, "viewBox");
    if (viewbox != "") {
        std::stringstream ss(viewbox);
        ss >> this->viewbox.first.x >> this->viewbox.first.y >>
            this->viewbox.second.x >> this->viewbox.second.y;
    }
    rapidxml::xml_node<> *node = svg->first_node();
    rapidxml::xml_node<> *prev = NULL;

    SVGElement *root = new Group();
    SVGElement *current = root;

    // Parse SVG elements
    while (node) {
        if (std::string(node->name()) == "defs") {
            // Parse gradients
            GetGradients(node);
            prev = node;
            node = node->next_sibling();
        } else if (std::string(node->name()) == "g") {
            // Parse Group attributes
            Group *group = dynamic_cast< Group * >(current);
            for (auto group_attribute : group->getAttributes()) {
                bool found = false;
                for (auto attribute = node->first_attribute(); attribute;
                     attribute = attribute->next_attribute()) {
                    if (std::string(attribute->name()) ==
                        group_attribute.first) {
                        if (group_attribute.first == "opacity") {
                            // Adjust opacity if already present in the group
                            // and node
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
                    // Add missing attributes from the group to the node
                    char *name =
                        doc.allocate_string(group_attribute.first.c_str());
                    char *value =
                        doc.allocate_string(group_attribute.second.c_str());
                    rapidxml::xml_attribute<> *new_attribute =
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
            // Parse Shape attributes and add to current group
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
                    rapidxml::xml_attribute<> *new_attribute =
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

// Parse and retrieve the value of the specified attribute from the XML node
std::string Parser::getAttribute(rapidxml::xml_node<> *node, std::string name) {
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
        else if (name == "gradientUnits")
            result = "objectBoundingBox";
    } else {
        result = node->first_attribute(name.c_str())->value();
    }
    return result;
}

// Parse and retrieve the float value of the specified attribute from the XML
// node
float Parser::getFloatAttribute(rapidxml::xml_node<> *node, std::string name) {
    float result;
    if (node->first_attribute(name.c_str()) == NULL) {
        if (std::string(node->name()).find("Gradient") != std::string::npos) {
            // Handle gradient-specific attribute default values
            if (name == "x1" || name == "y1" || name == "fr")
                result = 0;
            else if (name == "cx" || name == "cy")
                result = name == "cx" ? 0.5 * this->viewbox.second.x
                                      : 0.5 * this->viewbox.second.y;
            else if (name == "r") {
                result = sqrt((pow(this->viewbox.second.x, 2) +
                               pow(this->viewbox.second.y, 2)) /
                              2) /
                         2;
            } else if (name == "fx" || name == "fy")
                result = name == "fx" ? getFloatAttribute(node, "cx")
                                      : getFloatAttribute(node, "cy");
            else
                result = name == "x2" ? this->viewbox.second.x
                                      : this->viewbox.second.y;
        } else {
            // Handle default float attribute values for other elements
            if (name == "stroke-width" || name == "stroke-opacity" ||
                name == "fill-opacity" || name == "opacity" ||
                name == "stop-opacity")
                result = 1;
            else
                result = 0;
        }
    } else {
        if (name == "width" || name == "height") {
            // Handle width and height attributes with percentage or point units
            std::string value = node->first_attribute(name.c_str())->value();
            if (value.find("%") != std::string::npos) {
                result = std::stof(value.substr(0, value.find("%"))) *
                         this->viewbox.second.x / 100;
            } else if (value.find("pt") != std::string::npos) {
                result = std::stof(value.substr(0, value.find("pt"))) * 1.33;
            } else {
                result = std::stof(value);
            }
        } else
            result = std::stof(node->first_attribute(name.c_str())->value());
    }
    return result;
}

// Parse and convert color attribute from the XML node
ColorShape Parser::parseColor(rapidxml::xml_node<> *node, std::string name,
                              std::string &id) {
    std::string color = getAttribute(node, name);
    color.erase(std::remove(color.begin(), color.end(), ' '), color.end());
    if (color.find("url") == std::string::npos) {
        for (auto &c : color) c = tolower(c);
    }
    if (color == "none")
        return ColorShape::Transparent;
    else {
        ColorShape result;
        if (color.find("url") != std::string::npos) {
            // Handle gradient color reference
            if (color.find("'") != std::string::npos) {
                id = color.substr(color.find("'") + 1);
                id.erase(id.find("'"));
                id.erase(id.find("#"), 1);
            } else {
                id = color.substr(color.find("#") + 1);
                id.erase(id.find(")"));
            }
            result = ColorShape::Transparent;
        } else if (color.find("#") != std::string::npos) {
            // Handle hex color representation
            result = getHexColor(color);
        } else if (color.find("rgb") != std::string::npos) {
            // Handle RGB color representation
            result = getRgbColor(color);
        } else {
            // Handle predefined color names
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

// Parse and return the Gradient object associated with the specified ID
Gradient *Parser::parseGradient(std::string id) {
    if (gradients.find(id) == gradients.end()) {
        std::cout << "Gradient " << id << " not found" << std::endl;
        exit(-1);
    }
    return gradients.at(id);
}

// Parse and return the gradient stops from the XML node
std::vector< Stop > Parser::getGradientStops(rapidxml::xml_node<> *node) {
    std::vector< Stop > stops;
    rapidxml::xml_node<> *stop_node = node->first_node();
    while (stop_node) {
        if (std::string(stop_node->name()) == "stop") {
            std::string id = "";
            ColorShape color = parseColor(stop_node, "stop-color", id);
            float offset = getFloatAttribute(stop_node, "offset");
            if (offset > 1) offset /= 100;
            stops.push_back(Stop(color, offset));
        }
        stop_node = stop_node->next_sibling();
    }
    return stops;
}

// Parse and handle gradients defined in the XML node
void Parser::GetGradients(rapidxml::xml_node<> *node) {
    rapidxml::xml_node<> *gradient_node = node->first_node();
    while (gradient_node) {
        if (std::string(gradient_node->name()).find("Gradient") !=
            std::string::npos) {
            Gradient *gradient;
            std::string id = getAttribute(gradient_node, "id");
            std::string units = getAttribute(gradient_node, "gradientUnits");
            std::vector< Stop > stops = getGradientStops(gradient_node);
            std::string href = getAttribute(gradient_node, "xlink:href");
            int pos = href.find("#");
            if (pos != std::string::npos) {
                href = href.substr(pos + 1);
            }
            if (std::string(gradient_node->name()).find("linear") !=
                std::string::npos) {
                float x1 = getFloatAttribute(gradient_node, "x1");
                float y1 = getFloatAttribute(gradient_node, "y1");
                float x2 = getFloatAttribute(gradient_node, "x2");
                float y2 = getFloatAttribute(gradient_node, "y2");
                std::pair< Vector2Df, Vector2Df > points = {{x1, y1}, {x2, y2}};
                gradient = new LinearGradient(stops, points, units);
                if (this->gradients.find(id) == this->gradients.end())
                    this->gradients[id] = gradient;
            } else if (std::string(gradient_node->name()).find("radial") !=
                       std::string::npos) {
                float cx = getFloatAttribute(gradient_node, "cx");
                float cy = getFloatAttribute(gradient_node, "cy");
                float fx = getFloatAttribute(gradient_node, "fx");
                float fy = getFloatAttribute(gradient_node, "fy");
                float r = getFloatAttribute(gradient_node, "r");
                float fr = getFloatAttribute(gradient_node, "fr");
                std::pair< Vector2Df, Vector2Df > points = {{cx, cy}, {fx, fy}};
                Vector2Df radius(r, fr);
                gradient = new RadialGradient(stops, points, radius, units);
                if (this->gradients.find(id) == this->gradients.end())
                    this->gradients[id] = gradient;
            }
            if (href != "") {
                for (auto stop : parseGradient(href)->getStops()) {
                    gradient->addStop(stop);
                }
            }
            if (gradient != NULL)
                gradient->setTransforms(getTransformOrder(gradient_node));
        }
        gradient_node = gradient_node->next_sibling();
    }
}

// Parse SVG elements from the XML document
std::vector< Vector2Df > Parser::parsePoints(rapidxml::xml_node<> *node) {
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

// Parse and convert path data into a vector of PathPoint
std::vector< PathPoint > Parser::parsePathPoints(rapidxml::xml_node<> *node) {
    std::vector< PathPoint > points;
    std::string path_string = getAttribute(node, "d");

    // Pre-processing the raw path string
    formatSvgPathString(path_string);

    // Tokenizing the path string using stringstream
    std::stringstream ss(path_string);
    std::string element;
    PathPoint pPoint{{0, 0}, 'M'};  // Default starting point and command
    while (ss >> element) {
        if (std::isalpha(element[0])) {
            pPoint.tc = element[0];
            if (tolower(pPoint.tc) == 'm' || tolower(pPoint.tc) == 'l' ||
                tolower(pPoint.tc) == 'c' || tolower(pPoint.tc) == 's' ||
                tolower(pPoint.tc) == 'q' || tolower(pPoint.tc) == 't')
                ss >> pPoint.point.x >> pPoint.point.y;
            else if (tolower(pPoint.tc) == 'h') {
                ss >> pPoint.point.x;
                pPoint.point.y = 0;
            } else if (tolower(pPoint.tc) == 'v') {
                ss >> pPoint.point.y;
                pPoint.point.x = 0;
            } else if (tolower(pPoint.tc) == 'a') {
                ss >> pPoint.radius.x >> pPoint.radius.y;
                ss >> pPoint.x_axis_rotation;
                ss >> pPoint.large_arc_flag >> pPoint.sweep_flag;
                ss >> pPoint.point.x >> pPoint.point.y;
            }
        } else {
            if (tolower(pPoint.tc) == 'm' || tolower(pPoint.tc) == 'l' ||
                tolower(pPoint.tc) == 'c' || tolower(pPoint.tc) == 's' ||
                tolower(pPoint.tc) == 'q' || tolower(pPoint.tc) == 't') {
                if (tolower(pPoint.tc) == 'm') pPoint.tc = 'L';
                pPoint.point.x = std::stof(element);
                ss >> pPoint.point.y;
            } else if (tolower(pPoint.tc) == 'h') {
                pPoint.point.x = std::stof(element);
                pPoint.point.y = 0;
            } else if (tolower(pPoint.tc) == 'v') {
                pPoint.point.y = std::stof(element);
                pPoint.point.x = 0;
            } else if (tolower(pPoint.tc) == 'a') {
                pPoint.radius.x = std::stof(element);
                ss >> pPoint.radius.y;
                ss >> pPoint.x_axis_rotation;
                ss >> pPoint.large_arc_flag >> pPoint.sweep_flag;
                ss >> pPoint.point.x >> pPoint.point.y;
            }
        }
        points.push_back(pPoint);
    }

    std::vector< PathPoint > handle_points;

    // Processing and transforming raw path points
    Vector2Df first_point{0, 0}, cur_point{0, 0};
    int n = points.size();
    for (int i = 0; i < n; i++) {
        if (tolower(points[i].tc) == 'm') {
            first_point = points[i].point;
            if (points[i].tc == 'm') {
                first_point.x = cur_point.x + points[i].point.x;
                first_point.y = cur_point.y + points[i].point.y;
            }
            cur_point = first_point;
            handle_points.push_back({first_point, 'm'});

        } else if (tolower(points[i].tc) == 'l' ||
                   tolower(points[i].tc) == 't') {
            Vector2Df end_point{cur_point.x + points[i].point.x,
                                cur_point.y + points[i].point.y};
            if (points[i].tc == 'L' || points[i].tc == 'T')
                end_point = points[i].point;
            cur_point = end_point;
            char TC = tolower(points[i].tc);
            handle_points.push_back({end_point, TC});

        } else if (tolower(points[i].tc) == 'h') {
            Vector2Df end_point{cur_point.x + points[i].point.x, cur_point.y};
            if (points[i].tc == 'H')
                end_point = Vector2Df{points[i].point.x, cur_point.y};
            cur_point = end_point;
            handle_points.push_back({end_point, 'h'});

        } else if (tolower(points[i].tc) == 'v') {
            Vector2Df end_point{cur_point.x, cur_point.y + points[i].point.y};
            if (points[i].tc == 'V')
                end_point = Vector2Df{cur_point.x, points[i].point.y};
            cur_point = end_point;
            handle_points.push_back({end_point, 'v'});

        } else if (tolower(points[i].tc) == 'c') {
            if (i + 2 < n) {
                Vector2Df control_point1 =
                    Vector2Df{cur_point.x + points[i].point.x,
                              cur_point.y + points[i].point.y};
                Vector2Df control_point2 =
                    Vector2Df{cur_point.x + points[i + 1].point.x,
                              cur_point.y + points[i + 1].point.y};
                Vector2Df control_point3 =
                    Vector2Df{cur_point.x + points[i + 2].point.x,
                              cur_point.y + points[i + 2].point.y};
                if (points[i].tc == 'C') {
                    control_point1 = points[i].point;
                    control_point2 = points[i + 1].point;
                    control_point3 = points[i + 2].point;
                }
                i += 2;
                cur_point = control_point3;
                handle_points.push_back({control_point1, 'c'});
                handle_points.push_back({control_point2, 'c'});
                handle_points.push_back({control_point3, 'c'});
            }
        } else if (tolower(points[i].tc) == 'z') {
            cur_point = first_point;
            handle_points.push_back({first_point, 'z'});

        } else if (tolower(points[i].tc) == 's' ||
                   tolower(points[i].tc) == 'q') {
            if (i + 1 < n) {
                Vector2Df control_point1 =
                    Vector2Df{cur_point.x + points[i].point.x,
                              cur_point.y + points[i].point.y};
                Vector2Df control_point2 =
                    Vector2Df{cur_point.x + points[i + 1].point.x,
                              cur_point.y + points[i + 1].point.y};
                if (points[i].tc == 'S' || points[i].tc == 'Q') {
                    control_point1 = points[i].point;
                    control_point2 = points[i + 1].point;
                }
                i += 1;
                cur_point = control_point2;
                char TC = tolower(points[i].tc);
                handle_points.push_back({control_point1, TC});
                handle_points.push_back({control_point2, TC});
            }

        } else if (tolower(points[i].tc) == 'a') {
            Vector2Df end_point{cur_point.x + points[i].point.x,
                                cur_point.y + points[i].point.y};
            if (points[i].tc == 'A') end_point = points[i].point;
            handle_points.push_back(
                {end_point, 'a', points[i].radius, points[i].x_axis_rotation,
                 points[i].large_arc_flag, points[i].sweep_flag});
            cur_point = end_point;
        }
    }
    return handle_points;
}

// Retrieve the order of transformations applied to the SVG element
std::vector< std::string > Parser::getTransformOrder(
    rapidxml::xml_node<> *node) {
    std::string transform_tag;
    if (std::string(node->name()).find("Gradient") != std::string::npos) {
        transform_tag = getAttribute(node, "gradientTransform");
    } else {
        transform_tag = getAttribute(node, "transform");
    }

    std::vector< std::string > order;
    std::stringstream ss(transform_tag);
    std::string type;
    while (ss >> type) {
        if (type.find("translate") != std::string::npos ||
            type.find("scale") != std::string::npos ||
            type.find("rotate") != std::string::npos ||
            type.find("matrix") != std::string::npos) {
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

// Parse and convert SVG elements from the XML document
SVGElement *Parser::parseShape(rapidxml::xml_node<> *node) {
    SVGElement *shape = NULL;
    std::string type = node->name();
    std::string id = "";
    ColorShape stroke_color = parseColor(node, "stroke", id);
    ColorShape fill_color = parseColor(node, "fill", id);
    float stroke_width = getFloatAttribute(node, "stroke-width");
    // Determine the type of SVG element and create the corresponding object
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
        shape = parseText(node, fill_color, stroke_color, stroke_width);
    }

    // Apply transformations and gradient if applicable
    if (shape != NULL) {
        if (type == "text") {
            float dx = getFloatAttribute(node, "dx");
            float dy = getFloatAttribute(node, "dy");
            std::string transform = "translate(" + std::to_string(dx) + " " +
                                    std::to_string(dy) + ")";
            std::vector< std::string > transform_order =
                getTransformOrder(node);
            transform_order.push_back(transform);
            shape->setTransforms(transform_order);
        } else
            shape->setTransforms(getTransformOrder(node));
        if (id != "") {
            shape->setGradient(parseGradient(id));
        }
    }
    return shape;
}

// Parse a line element
Line *Parser::parseLine(rapidxml::xml_node<> *node,
                        const ColorShape &stroke_color, float stroke_width) {
    Line *shape = new Line(
        Vector2Df(getFloatAttribute(node, "x1"), getFloatAttribute(node, "y1")),
        Vector2Df(getFloatAttribute(node, "x2"), getFloatAttribute(node, "y2")),
        stroke_color, stroke_width);
    return shape;
}

// Parse a rectangle element
Rect *Parser::parseRect(rapidxml::xml_node<> *node,
                        const ColorShape &fill_color,
                        const ColorShape &stroke_color, float stroke_width) {
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

// Parse a circle element
Circle *Parser::parseCircle(rapidxml::xml_node<> *node,
                            const ColorShape &fill_color,
                            const ColorShape &stroke_color,
                            float stroke_width) {
    float cx = getFloatAttribute(node, "cx");
    float cy = getFloatAttribute(node, "cy");
    float radius = getFloatAttribute(node, "r");
    Circle *shape = new Circle(radius, Vector2Df(cx, cy), fill_color,
                               stroke_color, stroke_width);
    return shape;
}

// Parse an ellipse element
Ell *Parser::parseEllipse(rapidxml::xml_node<> *node,
                          const ColorShape &fill_color,
                          const ColorShape &stroke_color, float stroke_width) {
    float radius_x = getFloatAttribute(node, "rx");
    float radius_y = getFloatAttribute(node, "ry");
    float cx = getFloatAttribute(node, "cx");
    float cy = getFloatAttribute(node, "cy");
    Ell *shape = new Ell(Vector2Df(radius_x, radius_y), Vector2Df(cx, cy),
                         fill_color, stroke_color, stroke_width);
    return shape;
}

// Parse a polygon element
Plygon *Parser::parsePolygon(rapidxml::xml_node<> *node,
                             const ColorShape &fill_color,
                             const ColorShape &stroke_color,
                             float stroke_width) {
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

// Parse a polyline element
Plyline *Parser::parsePolyline(rapidxml::xml_node<> *node,
                               const ColorShape &fill_color,
                               const ColorShape &stroke_color,
                               float stroke_width) {
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

// Parse a text element
Text *Parser::parseText(rapidxml::xml_node<> *node,
                        const ColorShape &fill_color,
                        const ColorShape &stroke_color, float stroke_width) {
    float x = getFloatAttribute(node, "x");
    float y = getFloatAttribute(node, "y");
    float font_size = getFloatAttribute(node, "font-size");
    std::string text = getAttribute(node, "text");

    Text *shape =
        new Text(Vector2Df(x - (font_size * 6.6 / 40),
                           y - font_size + (font_size * 4.4 / 40)),
                 text, font_size, fill_color, stroke_color, stroke_width);

    std::string anchor = getAttribute(node, "text-anchor");
    anchor.erase(std::remove(anchor.begin(), anchor.end(), ' '), anchor.end());
    shape->setAnchor(anchor);

    std::string style = getAttribute(node, "font-style");
    style.erase(std::remove(style.begin(), style.end(), ' '), style.end());
    shape->setFontStyle(style);

    return shape;
}

// Parse a path element
Path *Parser::parsePath(rapidxml::xml_node<> *node,
                        const ColorShape &fill_color,
                        const ColorShape &stroke_color, float stroke_width) {
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

// Destructor
Parser::~Parser() {
    delete root;
    for (auto gradient : gradients) {
        delete gradient.second;
    }
}

// Print data of parsed SVG elements
void Parser::printShapesData() { root->printData(); }

// Get the viewBox of the SVG document
std::pair< Vector2Df, Vector2Df > Parser::getViewBox() const { return viewbox; }

// Get the viewport of the SVG document
Vector2Df Parser::getViewPort() const { return viewport; }