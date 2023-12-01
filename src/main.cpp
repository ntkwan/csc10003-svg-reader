#include "Parser.hpp"
#include "Viewer.hpp"

int main() {
    constexpr int screen_width = 1600;
    constexpr int screen_height = 900;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;
    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height),
                            "svg-reader-version-0.1", sf::Style::Default,
                            settings);
    sf::View view(sf::FloatRect(0.f, 0.f, static_cast< float >(screen_width),
                                static_cast< float >(screen_height)));
    Viewer *viewer = Viewer::getInstance(window, view);
    Parser *parser = Parser::getInstance("external/samples/mixed/sample8.svg");
    Renderer *renderer = Renderer::getInstance(window);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            viewer->handleEvents(event);
        }

        window.clear(sf::Color::White);
        parser->renderSVG(*renderer);
        window.display();
        viewer->handleDragging();
    }

    delete viewer;
    delete parser;
    delete renderer;
    return 0;
}
