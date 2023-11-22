#include "Vertex.hpp"

Vertex::Vertex() : position(0, 0), color(255, 255, 255) {}

Vertex::Vertex(const Vector2Df& thePosition)
    : position(thePosition), color(255, 255, 255) {}

Vertex::Vertex(const Vector2Df& thePosition, const Color& theColor)
    : position(thePosition), color(theColor) {}

Vertex::Vertex(const Vector2Df& thePosition, const Vector2Df& theTexCoords)
    : position(thePosition), color(255, 255, 255) {}

Vertex::Vertex(const Vector2Df& thePosition, const Color& theColor,
               const Vector2Df& theTexCoords)
    : position(thePosition), color(theColor) {}
