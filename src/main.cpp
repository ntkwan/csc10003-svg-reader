#include "Parser.hpp"
#include "Viewer.hpp"

int main() {
    constexpr int screen_width = 1600;
    constexpr int screen_height = 900;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height),
                            "svg-reader-version-0.1", sf::Style::Default,
                            settings);
    sf::View view(sf::FloatRect(0.f, 0.f, static_cast< float >(screen_width),
                                static_cast< float >(screen_height)));
    ZoomableDraggableView zoomableView(window, view);
    Parser parser("sample/sample.svg");
    parser.parseSVG();
    if (!Text::font.loadFromFile("external/font/Arial.ttf")) {
        return EXIT_FAILURE;
    }

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            zoomableView.handleEvents(event);
        }

        window.clear(sf::Color::White);
        parser.renderSVG(window);
        window.display();
        zoomableView.handleDragging();
    }
    return 0;
}