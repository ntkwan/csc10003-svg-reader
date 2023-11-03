#include <SFML/Graphics.hpp>

#include "Circle.hpp"
#include "Ellipse.hpp"
#include "Parser.hpp"
#include "pugixml.hpp"

int main() {
    constexpr int screen_width = 1600;
    constexpr int screen_height = 900;

    std::vector< sf::Shape* > shapes = parser::parseSVG("../sample/sample.svg");
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;
    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height),
                            "svg-reader-version-0.1", sf::Style::Default,
                            settings);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear(sf::Color::White);
        for (auto shape : shapes) {
            window.draw(*shape);
        }
        window.display();
    }

    parser::deleteShapes(shapes);
    return 0;
}