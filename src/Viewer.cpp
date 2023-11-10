#include "Viewer.hpp"

ZoomableDraggableView::ZoomableDraggableView(sf::RenderWindow& Window,
                                             sf::View& View)
    : window(Window), view(View) {}

void ZoomableDraggableView::handleEvents(sf::Event event) {
    if (event.type == sf::Event::Closed) {
        window.close();
    }

    // Zoom in by + (including '=' key)
    if ((event.type == sf::Event::KeyPressed &&
         event.key.code == sf::Keyboard::Add) ||
        (event.type == sf::Event::KeyPressed &&
         event.key.code == sf::Keyboard::Equal)) {
        zoom(0.9f);
        window.setView(view);
    }

    // Zoom out by - (including '-' key)
    if ((event.type == sf::Event::KeyPressed &&
         event.key.code == sf::Keyboard::Subtract) ||
        (event.type == sf::Event::KeyPressed &&
         event.key.code == sf::Keyboard::Hyphen)) {
        zoom(1.1f);
        window.setView(view);
    }

    // Zoom in/out with mouse scroll
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            if (event.mouseWheelScroll.delta > 0) {
                zoom(0.9f);
            } else {
                zoom(1.1f);
            }
            window.setView(view);
        }
    }

    // Zoom in/out with touchpad (control pad)
    if (event.type == sf::Event::TouchMoved) {
        // Assuming that touchpad input scales the zoom based on movement
        float delta = event.touch.y;
        if (delta > 0) {
            zoom(0.9f);
        } else {
            zoom(1.1f);
        }
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
