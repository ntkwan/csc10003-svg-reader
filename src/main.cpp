#include "Parser.hpp"

int main() {
    constexpr int screen_width = 1600;
    constexpr int screen_height = 900;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;
    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height),
                            "svg-reader-version-0.1", sf::Style::Default,
                            settings);

    Parser parser("sample/sample.svg");
    parser.parseSVG();

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear(sf::Color::White);
        parser.renderSVG(window);
        window.display();
    }

    return 0;
}