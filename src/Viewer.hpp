#ifndef VIEWER_HPP_
#define VIEWER_HPP_

#include "Renderer.hpp"

/**
 * @brief Represents a viewer for rendering and interacting with a scene.
 *
 *
 * The viewer supports the following interactions:
 * - Rotation: Press 'Q' to rotate the view counterclockwise and 'E' to rotate
 * clockwise.
 * - Zooming: Use the scroll wheel to zoom in and out of the scene.
 * - Translation: Click and drag the left mouse button to translate the view.
 */
class Viewer {
public:
    float offset_x;         ///< X-coordinate offset of the viewer
    float offset_y;         ///< Y-coordinate offset of the viewer
    float zoom_factor;      ///< Zoom factor for scaling the view
    float rotate_angle;     ///< Rotation angle of the view
    bool needs_repaint;     ///< Flag indicating whether the view needs to be
                            ///< repainted
    Vector2Df window_size;  ///< Size of the window

    /**
     * @brief Gets the singleton instance of the Viewer class.
     *
     * @return The singleton instance of the Viewer class.
     */
    static Viewer* getInstance();

    /**
     * @brief Destructor for the Viewer class.
     */
    ~Viewer();

    /**
     * @brief Handles mouse events, such as wheel, move, left button down, and
     * left button up.
     *
     * @param message The Windows message identifier.
     * @param wParam The WPARAM parameter of the message.
     * @param lParam The LPARAM parameter of the message.
     */
    void handleMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);

    /**
     * @brief Handles keyboard events.
     *
     * @param wParam The WPARAM parameter of the message.
     */
    void handleKeyEvent(WPARAM wParam);

    /**
     * @brief Get the current window size.
     *
     * @param hWnd The handle to the window.
     */
    void getWindowSize(HWND hWnd) const;

private:
    static Viewer* instance;  ///< Singleton instance of the Viewer class

    /**
     * @brief Private constructor for the Viewer class.
     */
    Viewer();

    /**
     * @brief Copy constructor for the Viewer class (deleted to enforce
     * singleton pattern).
     */
    Viewer(const Viewer&) = delete;

    /**
     * @brief Copy assignment operator for the Viewer class (deleted to enforce
     * singleton pattern).
     */
    void operator=(const Viewer&) = delete;

    bool is_dragging;  ///< Flag indicating whether the mouse is being dragged
    POINT last_mouse_pos;  ///< Last recorded mouse position

    /**
     * @brief Handles the mouse wheel event for zooming.
     *
     * @param wParam The WPARAM parameter of the message.
     */
    void handleMouseWheel(WPARAM wParam);

    /**
     * @brief Handles the mouse move event for panning.
     *
     * @param lParam The LPARAM parameter of the message.
     */
    void handleMouseMove(LPARAM lParam);

    /**
     * @brief Handles the left button down event for initiating dragging.
     *
     * @param lParam The LPARAM parameter of the message.
     */
    void handleLeftButtonDown(LPARAM lParam);

    /**
     * @brief Handles the left button up event for ending dragging.
     */
    void handleLeftButtonUp();

    /**
     * @brief Handles the key down event for rotating.
     *
     * @param wParam The WPARAM parameter of the message.
     */
    void handleKeyDown(WPARAM wParam);
};

#endif  // VIEWER_HPP_
