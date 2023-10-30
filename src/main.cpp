#include <SFML/Graphics.hpp>
#include <iostream>

#include "pugixml.hpp"

int main() {
    constexpr int screen_width = 1600;
    constexpr int screen_height = 900;

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("sample/pic.svg");
    if (!result) return -1;

    auto get_attribute = [&](std::string name) {
        auto node = doc.child("svg").child("circle");
        auto attr = node.attribute(name.c_str());
        return attr.value();
    };

    int pos_x = std::stoi(get_attribute("cx"));
    int pos_y = std::stoi(get_attribute("cy"));
    float radius = std::stof(get_attribute("r"));

    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height),
                            "svg-reader-version-0.1");
    sf::CircleShape shape(radius, 1000);
    shape.setPosition(pos_x, pos_y);
    shape.setFillColor(sf::Color::Red);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
    return 0;
}