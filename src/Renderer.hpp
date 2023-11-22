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
    void draw(Shape* shape);

    /**
     * @brief Draws the polyline on the specified render target.
     *
     */
    void draw(Polyline* polyline);

    /**
     * @brief Draws the text on the specified render target.
     *
     */
    void draw(Text* text);

private:
    Renderer(sf::RenderWindow& window);
    static Renderer* instance;
};

#endif