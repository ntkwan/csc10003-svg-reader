#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include <SFML/Graphics.hpp>

class Renderer {
public:
    static Renderer* getInstance(sf::RenderWindow& window);
    Renderer(const Renderer&) = delete;
    void operator=(const Renderer&) = delete;

    sf::RenderWindow& window;

private:
    Renderer(sf::RenderWindow& window);
    static Renderer* instance;
};

#endif