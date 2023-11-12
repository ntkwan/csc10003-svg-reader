#ifndef VIEWER_HPP_
#define VIEWER_HPP_

#include <SFML/Graphics.hpp>

class Viewer {
public:
    void handleEvents(sf::Event event);
    void handleDragging();
    void operator=(const Viewer&) = delete;
    static Viewer* getInstance(sf::RenderWindow& Window, sf::View& View);
    Viewer(const Viewer&) = delete;

private:
    Viewer(sf::RenderWindow& Window, sf::View& View);
    static Viewer* instance;
    sf::RenderWindow& window;
    sf::View& view;
    sf::Vector2i last_mouse_position;
    bool is_mouse_dragging = false;

    void zoom(float factor);

    void rotate(float angle);

    void startDragging();

    void stopDragging();

    void moveView(const sf::Vector2f& offset);
};

#endif  // VIEWER_HPP_
