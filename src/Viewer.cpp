#include "Viewer.hpp"
Viewer* Viewer::instance = nullptr;

Viewer* Viewer::getInstance() {
    if (!instance) {
        instance = new Viewer();
    }
    return instance;
}

Viewer::Viewer() {
    isDragging = false;
    zoomFactor = 1.0f;
    rotateAngle = 0.0f;
    offsetX = 0.0f;
    offsetY = 0.0f;
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
                InvalidateRect(GetActiveWindow(), NULL, TRUE);
            }

        case WM_LBUTTONDOWN:
            handleLeftButtonDown(lParam);
            break;

        case WM_LBUTTONUP:
            handleLeftButtonUp();
            break;
    }

    InvalidateRect(GetActiveWindow(), NULL, TRUE);
}

void Viewer::handleKeyEvent(WPARAM wParam) {
    handleKeyDown(wParam);
    InvalidateRect(GetActiveWindow(), NULL, TRUE);
}

void Viewer::handleMouseWheel(WPARAM wParam) {
    if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
        zoomFactor *= 1.1f;
    } else {
        zoomFactor /= 1.1f;
    }
}

void Viewer::handleMouseMove(LPARAM lParam) {
    if (isDragging) {
        int x = static_cast< int >(LOWORD(lParam));
        int y = static_cast< int >(HIWORD(lParam));
        offsetX += (x - lastMousePos.x) * zoomFactor;
        offsetY += (y - lastMousePos.y) * zoomFactor;
        lastMousePos.x = x;
        lastMousePos.y = y;
    }
}

void Viewer::handleLeftButtonDown(LPARAM lParam) {
    isDragging = true;
    lastMousePos.x = static_cast< int >(LOWORD(lParam));
    lastMousePos.y = static_cast< int >(HIWORD(lParam));
    SetCapture(GetActiveWindow());
}

void Viewer::handleLeftButtonUp() {
    isDragging = false;
    ReleaseCapture();
}

void Viewer::handleKeyDown(WPARAM wParam) {
    char key = static_cast< char >(wParam);
    switch (tolower(key)) {
        case 'q':
            rotateAngle -= 1.0f;
            break;

        case 'e':
            rotateAngle += 1.0f;
            break;
    }
}