#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "Color.hpp"
#include "Vector2D.hpp"

/**
 * @brief Defines a point with color and texture coordinates.
 * A vertex is an improved point. It has a position and other
 * extra attributes that will be used for drawing: in SFML,
 * vertices also have a color and a pair of texture coordinates.
 *
 * The vertex is the building block of drawing. Everything which
 * is visible on screen is made of vertices. They are grouped
 * as 2D primitives (triangles, quads, ...), and these primitives
 * are grouped to create even more complex 2D entities such as
 * sprites, texts, etc.
 */
class Vertex {
public:
    /**
     * @brief Default constructor.
     */
    Vertex();

    /**
     * @brief Constructs a vertex from its position.
     *
     * @param thePosition The position of the vertex.
     *
     * @note The vertex color is white and texture coordinates are (0, 0).
     */
    Vertex(const Vector2Df& thePosition);

    /**
     * @brief Constructs a vertex from its position and color.
     *
     * @param thePosition The position of the vertex.
     * @param theColor The color of the vertex.
     *
     * @note The texture coordinates are (0, 0).
     */
    Vertex(const Vector2Df& thePosition, const Color& theColor);

    /**
     * @brief Constructs a vertex from its position and texture coordinates.
     * @param thePosition The position of the vertex.
     * @param theTexCoords The texture coordinates of the vertex.
     * @note The vertex color is white.
     */
    Vertex(const Vector2Df& thePosition, const Vector2Df& theTexCoords);

    /**
     * @brief Constructs a vertex from its position, color and texture
     * coordinates.
     *
     * @param thePosition The position of the vertex.
     * @param theColor The color of the vertex.
     * @param theTexCoords The texture coordinates of the vertex.
     */
    Vertex(const Vector2Df& thePosition, const Color& theColor,
           const Vector2Df& theTexCoords);

    Vector2Df position;   ///< 2D position of the vertex
    Color color;          ///< Color of the vertex
    Vector2Df texCoords;  ///< Coordinates of the texture's pixel to map to
                          ///< the vertex
};

#endif  // VERTEX_HPP
