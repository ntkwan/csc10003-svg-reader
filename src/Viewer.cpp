#include "Viewer.hpp"

Viewer* Viewer::instance = nullptr;

Viewer* Viewer::getInstance(sf::RenderWindow& Window, sf::View& View) {
    if (instance == nullptr) {
        instance = new Viewer(Window, View);
    }
    return instance;
}

Viewer::Viewer(sf::RenderWindow& Window, sf::View& View)
    : window(Window), view(View) {}

void Viewer::handleEvents(sf::Event event) {
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

    // Rotate clockwise by 'R' key
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::R) {
        rotate(90.0f);
        window.setView(view);
    }

    // Rotate anti-clockwise by 'E' key
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::E) {
        rotate(-90.0f);
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

void Viewer::handleDragging() {
    if (is_mouse_dragging) {
        sf::Vector2i currentMousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f offset =
            sf::Vector2f(currentMousePosition - last_mouse_position);
        view.move(-offset);
        window.setView(view);
        last_mouse_position = currentMousePosition;
    }
}

void Viewer::zoom(float factor) {
    view.zoom(factor);
    window.setView(view);
}

void Viewer::rotate(float angle) {
    view.rotate(angle);
    window.setView(view);
}

void Viewer::startDragging() {
    is_mouse_dragging = true;
    last_mouse_position = sf::Mouse::getPosition(window);
}

void Viewer::stopDragging() { is_mouse_dragging = false; }

void Viewer::moveView(const sf::Vector2f& offset) {
    view.move(-offset);
    window.setView(view);
}
