#ifndef VIEWER_HPP_
#define VIEWER_HPP_

#include <SFML/Graphics.hpp>

class ZoomableDraggableView {
public:
    ZoomableDraggableView(sf::RenderWindow& Window, sf::View& View);
    void handleEvents(sf::Event event);
    void handleDragging();

private:
    sf::RenderWindow& window;
    sf::View& view;
    sf::Vector2i lastMousePosition;
    bool isMouseDragging = false;

    void zoom(float factor);

    void startDragging();

    void stopDragging();

    void moveView(const sf::Vector2f& offset);
};

#endif  // VIEWER_HPP_
