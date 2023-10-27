#include <SFML/Graphics.hpp>

#include "raylib.h"

int main() {
    constexpr int screen_width = 1600;
    constexpr int screen_height = 900;

    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height),
                            "svg-reader-version-0.1");

    constexpr int pos_x = 700;
    constexpr int pos_y = 300;
    constexpr float radius = 100.f;
    sf::CircleShape shape(radius, 1000);
    shape.setPosition(pos_x, pos_y);
    shape.setFillColor(sf::Color::Green);

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