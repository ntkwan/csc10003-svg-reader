#ifndef ZOOMABLE_DRAGGABLE_VIEW_HPP
#define ZOOMABLE_DRAGGABLE_VIEW_HPP

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

#endif  // ZOOMABLE_DRAGGABLE_VIEW_H
