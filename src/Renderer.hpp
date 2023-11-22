#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include <SFML/Graphics.hpp>

#include "graphics/Polyline.hpp"
#include "graphics/Text.hpp"

class Renderer {
public:
    static Renderer* getInstance(sf::RenderWindow& window);
    Renderer(const Renderer&) = delete;
    void operator=(const Renderer&) = delete;

    sf::RenderWindow& window;
    /**
     * @brief Draw the shape on the specified render target
     *
     */
    void draw(Shape* shape) const;

private:
    Renderer(sf::RenderWindow& window);
    static Renderer* instance;
};

#endif