#include "Parser.hpp"

#include <iostream>
#include <sstream>
#include <vector>

namespace {
    std::vector< std::string > getTransformOrder(pugi::xml_node node) {
        std::vector< std::string > order;
        std::string transform_tag = node.attribute("transform").value();
        std::stringstream ss(transform_tag);
        std::string type = "";
        while (ss >> type) {
            order.push_back(type);
        }

        return order;
    }
}  // namespace

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
    if (!result) exit(EXIT_FAILURE);
    svg = doc.child("svg");
}

std::string Parser::getAttribute(pugi::xml_node node, std::string name) {
    pugi::xml_attribute attr = node.attribute(name.c_str());
    if (!attr) {
        if (name == "fill")
            return "black";
        else if (name == "stroke" || name == "transform" || name == "rotate")
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

Color Parser::parseColor(pugi::xml_node node, std::string name) {
    auto getRgbColor = [](std::string color) -> Color {
        int r, g, b;
        float a = 1;
        sscanf(color.c_str(), "rgb(%d,%d,%d,%f)", &r, &g, &b, &a);
        return Color(r, g, b, 255 * a);
    };

    auto getHexColor = [](std::string color) -> Color {
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
            return Color(r, g, b, a);
        }
        return Color(r, g, b, 255);
    };

    std::string color = getAttribute(node, name);
    for (auto& c : color) c = tolower(c);
    if (color == "none")
        return Color::Transparent;
    else {
        Color result;
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

std::vector< Vector2Df > Parser::parsePoints(pugi::xml_node node) {
    std::vector< Vector2Df > points;
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
                points.push_back(Vector2Df(x, y));
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
        points.push_back(Vector2Df(x, y));
    }
    return points;
}

std::pair< float, float > Parser::getTranslate(pugi::xml_node node,
                                               std::string name) {
    std::string transform_tag = getAttribute(node, name);
    int translate_beg = transform_tag.find("translate(");
    int translate_end = transform_tag.find(')');

    float trans_x = 0, trans_y = 0;
    if (transform_tag != "none") {
        std::string translate_tag =
            (translate_beg != std::string::npos)
                ? transform_tag.substr(translate_beg + 10,
                                       translate_end - translate_beg - 10)
                : "none";
        if (translate_tag != "none") {
            if (translate_tag.find(',') != std::string::npos) {
                int mid = translate_tag.find(',');
                int end = translate_tag.find(')');
                trans_x = std::stof(translate_tag.substr(0, mid));
                trans_y = std::stof(translate_tag.substr(mid + 1, end - mid));
            } else {
                int end = translate_tag.find(')');
                trans_x = std::stof(translate_tag.substr(0, end));
                trans_y = 0;
            }
        }
    }
    return std::pair< float, float >(trans_x, trans_y);
}

float Parser::getRotate(pugi::xml_node node, std::string name) {
    std::string transform_tag = getAttribute(node, name);
    int rotate_beg = transform_tag.find("rotate(");
    int rotate_end = transform_tag.find(')');

    float degree = 0;
    if (transform_tag != "none") {
        std::string rotate_tag =
            (rotate_beg != std::string::npos)
                ? transform_tag.substr(rotate_beg + 7,
                                       rotate_end - rotate_beg - 7)
                : "none";
        if (rotate_tag != "none") {
            degree = std::stof(rotate_tag);
        }
    }

    return degree;
}

void Parser::parseSVG() {
    auto doTransformTag =
        [&](const pugi::xml_node& tool, Shape* shape,
            const std::vector< std::string >& transform_order) {
            float trans_x = getTranslate(tool).first,
                  trans_y = getTranslate(tool).second;
            float degree = getRotate(tool);
            for (auto type : transform_order) {
                if (type.find("translate") != std::string::npos) {
                    shape->translate(trans_x, trans_y);
                } else if (type.find("rotate") != std::string::npos) {
                    shape->rotate(degree);
                }
            }
        };

    for (pugi::xml_node tool = svg.first_child(); tool;
         tool = tool.next_sibling()) {
        Color stroke_color = parseColor(tool, "stroke");
        Color fill_color = parseColor(tool, "fill");
        float stroke_width = std::stof(getAttribute(tool, "stroke-width"));

        std::vector< std::string > transform_order = getTransformOrder(tool);

        if (tool.name() == std::string("rect")) {
            float x = std::stof(getAttribute(tool, "x"));
            float y = std::stof(getAttribute(tool, "y"));
            Rect* shape = new Rect(std::stof(getAttribute(tool, "width")),
                                   std::stof(getAttribute(tool, "height")), x,
                                   y, fill_color, stroke_color, stroke_width);

            doTransformTag(tool, shape, transform_order);

            shapes.push_back(shape);
        } else if (tool.name() == std::string("line")) {
            Line* shape =
                new Line(Vector2Df(std::stof(getAttribute(tool, "x1")),
                                   std::stof(getAttribute(tool, "y1"))),
                         Vector2Df(std::stof(getAttribute(tool, "x2")),
                                   std::stof(getAttribute(tool, "y2"))),
                         stroke_color, stroke_width);

            doTransformTag(tool, shape, transform_order);

            shapes.push_back(shape);
        } else if (tool.name() == std::string("text")) {
            float x = std::stof(getAttribute(tool, "x"));
            float y = std::stof(getAttribute(tool, "y"));
            float font_size = std::stof(getAttribute(tool, "font-size"));
            std::string text = tool.text().get();
            Text* shape = new Text(Vector2Df(x, y - font_size), text,
                                   fill_color, font_size);

            doTransformTag(tool, shape, transform_order);

            shapes.push_back(shape);
        } else if (tool.name() == std::string("circle")) {
            float cx = std::stof(getAttribute(tool, "cx"));
            float cy = std::stof(getAttribute(tool, "cy"));
            float radius = std::stof(getAttribute(tool, "r"));
            Circle* shape =
                new Circle(radius, Vector2Df(cx - radius, cy - radius),
                           fill_color, stroke_color, stroke_width);

            doTransformTag(tool, shape, transform_order);

            shapes.push_back(shape);
        } else if (tool.name() == std::string("ellipse")) {
            float radius_x = std::stof(getAttribute(tool, "rx"));
            float radius_y = std::stof(getAttribute(tool, "ry"));
            float cx = std::stof(getAttribute(tool, "cx"));
            float cy = std::stof(getAttribute(tool, "cy"));
            Ellipse* shape =
                new Ellipse(Vector2Df(radius_x, radius_y),
                            Vector2Df(cx - radius_x, cy - radius_y), fill_color,
                            stroke_color, stroke_width);

            doTransformTag(tool, shape, transform_order);

            shapes.push_back(shape);
        } else if (tool.name() == std::string("polygon")) {
            Polygon* shape =
                new Polygon(fill_color, stroke_color, stroke_width);
            std::vector< Vector2Df > points = parsePoints(tool);
            for (auto point : points) {
                /* translate
                point.x += trans_x;
                point.y += trans_y;
                */
                shape->addPoint(point);
            }
            // TBD: rotate
            shape->updateShape();
            shapes.push_back(shape);
        } else if (tool.name() == std::string("polyline")) {
            Polyline* shape =
                new Polyline(stroke_width, stroke_color, fill_color);
            std::vector< Vector2Df > points = parsePoints(tool);
            for (auto point : points) {
                /* translate
                point.x += trans_x;
                point.y += trans_y;
                */
                shape->addPoint(point);
            }
            // TBD: rotate
            shapes.push_back(shape);
        } else if (tool.name() == std::string("path")) {
            /*

            TBD: PATH

            */
        }
    }
}

void Parser::renderSVG(Renderer& renderer) {
    for (auto shape : shapes) {
        shape->draw(renderer);
    }
}

Parser::~Parser() {
    for (auto shape : shapes) {
        delete shape;
    }
    shapes.clear();
}