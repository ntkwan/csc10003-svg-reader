#ifndef VIEWER_HPP_
#define VIEWER_HPP_

#include <SFML/Graphics.hpp>

/**
 * @brief Represents a viewer for handling events and interactions with an SFML
 * window.
 *
 * The Viewer class is responsible for handling events, such as zooming,
 * rotating, and dragging, to interact with an SFML window and view.
 */
class Viewer {
public:
    /**
     * @brief Handles SFML events.
     *
     * @param event The SFML event to handle.
     */
    void handleEvents(sf::Event event);

    /**
     * @brief Handles dragging interaction.
     */
    void handleDragging();

    /**
     * @brief Deleted assignment operator to prevent copying of Viewer
     * instances.
     */
    void operator=(const Viewer&) = delete;

    /**
     * @brief Gets the singleton instance of Viewer.
     *
     * @param Window The SFML window to associate with the viewer.
     * @param View The SFML view to associate with the viewer.
     * @return The singleton instance of Viewer.
     */
    static Viewer* getInstance(sf::RenderWindow& Window, sf::View& View);

    /**
     * @brief Deleted copy constructor to prevent copying of Viewer instances.
     */
    Viewer(const Viewer&) = delete;

private:
    /**
     * @brief Constructs a Viewer object.
     *
     * @param Window The SFML window to associate with the viewer.
     * @param View The SFML view to associate with the viewer.
     */
    Viewer(sf::RenderWindow& Window, sf::View& View);

    static Viewer* instance;           ///< Singleton instance of Viewer
    sf::RenderWindow& window;          ///< Reference to the SFML window
    sf::View& view;                    ///< Reference to the SFML view
    sf::Vector2i last_mouse_position;  ///< Last recorded mouse position
    bool is_mouse_dragging =
        false;  ///< Flag indicating whether mouse dragging is active

    /**
     * @brief Zooms the view by the specified factor.
     *
     * @param factor The zoom factor.
     */
    void zoom(float factor);

    /**
     * @brief Rotates the view by the specified angle.
     *
     * @param angle The rotation angle.
     */
    void rotate(float angle);

    /**
     * @brief Starts dragging the view.
     */
    void startDragging();

    /**
     * @brief Stops dragging the view.
     */
    void stopDragging();

    /**
     * @brief Moves the view by the specified offset.
     *
     * @param offset The offset by which to move the view.
     */
    void moveView(const sf::Vector2f& offset);
};

#endif  // VIEWER_HPP_
