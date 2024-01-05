#include "Viewer.hpp"
Viewer* Viewer::instance = nullptr;

Viewer* Viewer::getInstance() {
    if (!instance) {
        instance = new Viewer();
    }
    return instance;
}

Viewer::Viewer() {
    is_dragging = false;
    zoom_factor = 1.0f;
    rotate_angle = 0.0f;
    offset_x = 0.0f;
    offset_y = 0.0f;
}

Viewer::~Viewer() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

void Viewer::handleMouseEvent(UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_MOUSEWHEEL:
            handleMouseWheel(wParam);
            break;

        case WM_MOUSEMOVE:
            if (wParam & MK_LBUTTON) {
                handleMouseMove(lParam);
            }

        case WM_LBUTTONDOWN:
            handleLeftButtonDown(lParam);
            break;

        case WM_LBUTTONUP:
            handleLeftButtonUp();
            break;
    }
}

void Viewer::handleKeyEvent(WPARAM wParam) { handleKeyDown(wParam); }

void Viewer::handleMouseWheel(WPARAM wParam) {
    if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
        zoom_factor *= 1.1f;
        needs_repaint = true;
    } else {
        zoom_factor /= 1.1f;
        needs_repaint = true;
    }
}

void Viewer::handleMouseMove(LPARAM lParam) {
    if (is_dragging) {
        int x = static_cast< int >(LOWORD(lParam));
        int y = static_cast< int >(HIWORD(lParam));

        if (x != last_mouse_pos.x || y != last_mouse_pos.y) {
            offset_x += (x - last_mouse_pos.x) * zoom_factor;
            offset_y += (y - last_mouse_pos.y) * zoom_factor;
            last_mouse_pos.x = x;
            last_mouse_pos.y = y;
            needs_repaint = true;
        }
    }
}

void Viewer::handleLeftButtonDown(LPARAM lParam) {
    is_dragging = true;
    last_mouse_pos.x = static_cast< int >(LOWORD(lParam));
    last_mouse_pos.y = static_cast< int >(HIWORD(lParam));
    SetCapture(GetActiveWindow());
}

void Viewer::handleLeftButtonUp() {
    is_dragging = false;
    ReleaseCapture();

    if (needs_repaint) {
        needs_repaint = false;
    }
}

void Viewer::handleKeyDown(WPARAM wParam) {
    char key = static_cast< char >(wParam);
    switch (tolower(key)) {
        case 'q':
            rotate_angle -= 1.0f;
            break;

        case 'e':
            rotate_angle += 1.0f;
            break;
    }
}

void Viewer::getWindowSize(HWND hWnd) const {
    RECT rect;
    GetClientRect(hWnd, &rect);
    instance->window_size.x = static_cast< float >(rect.right - rect.left);
    instance->window_size.y = static_cast< float >(rect.bottom - rect.top);
}