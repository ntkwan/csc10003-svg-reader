#ifndef VIEWER_HPP_
#define VIEWER_HPP_

#include "Renderer.hpp"

class Viewer {
public:
    float offsetX;
    float offsetY;
    float zoomFactor;
    float rotateAngle;
    bool needsRepaint;
    static Viewer* getInstance();
    ~Viewer();

    void handleMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);
    void handleKeyEvent(WPARAM wParam);

private:
    static Viewer* instance;

    Viewer();
    Viewer(const Viewer&) = delete;
    void operator=(const Viewer&) = delete;

    bool isDragging;
    POINT lastMousePos;

    void handleMouseWheel(WPARAM wParam);
    void handleMouseMove(LPARAM lParam);
    void handleLeftButtonDown(LPARAM lParam);
    void handleLeftButtonUp();
    void handleKeyDown(WPARAM wParam);
};

#endif  // VIEWER_HPP_