#include "ZoomableDraggableView.hpp"

ZoomableDraggableView::ZoomableDraggableView(sf::RenderWindow& Window,
                                             sf::View& View)
    : window(Window), view(View) {}

void ZoomableDraggableView::handleEvents(sf::Event event) {
    if (event.type == sf::Event::Closed) window.close();

    // Zoom in by +
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Add) {
        zoom(0.9f);
        window.setView(view);
    }

    // Zoom out by -
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Subtract) {
        zoom(1.1f);
        window.setView(view);
    }

    // Start dragging the left mouse button
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
        startDragging();
    }

    // Finish dragging the left mouse button
    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left) {
        stopDragging();
    }
}

void ZoomableDraggableView::handleDragging() {
    if (isMouseDragging) {
        sf::Vector2i currentMousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f offset =
            sf::Vector2f(currentMousePosition - lastMousePosition);
        view.move(-offset);
        window.setView(view);
        lastMousePosition = currentMousePosition;
    }
}

void ZoomableDraggableView::zoom(float factor) {
    view.zoom(factor);
    window.setView(view);
}

void ZoomableDraggableView::startDragging() {
    isMouseDragging = true;
    lastMousePosition = sf::Mouse::getPosition(window);
}

void ZoomableDraggableView::stopDragging() { isMouseDragging = false; }

void ZoomableDraggableView::moveView(const sf::Vector2f& offset) {
    view.move(-offset);
    window.setView(view);
}
