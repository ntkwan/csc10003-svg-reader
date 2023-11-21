#include "Renderer.hpp"

Renderer* Renderer::instance = nullptr;

Renderer* Renderer::getInstance(sf::RenderWindow& window) {
    if (instance == nullptr) {
        instance = new Renderer(window);
    }
    return instance;
}

Renderer::Renderer(sf::RenderWindow& window) : window(window) {}