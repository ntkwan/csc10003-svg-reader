#include "Shape.hpp"

#include <cmath>

namespace {
    sf::Vector2f computeNormal(const sf::Vector2f& p1, const sf::Vector2f& p2) {
        sf::Vector2f normal(p1.y - p2.y, p2.x - p1.x);
        float length = std::sqrt(normal.x * normal.x + normal.y * normal.y);
        if (length != 0.f) normal /= length;
        return normal;
    }

    float dotProduct(const sf::Vector2f& p1, const sf::Vector2f& p2) {
        return p1.x * p2.x + p1.y * p2.y;
    }
}  // namespace

void Shape::setTexture(const sf::Texture* texture, bool resetRect) {
    if (texture) {
        // Recompute the texture area if requested, or if there was no
        // texture & rect before
        if (resetRect || (!m_texture && (m_textureRect == sf::IntRect())))
            setTextureRect(
                sf::IntRect(0, 0, static_cast< int >(texture->getSize().x),
                            static_cast< int >(texture->getSize().y)));
    }

    // Assign the new texture
    m_texture = texture;
}

const sf::Texture* Shape::getTexture() const { return m_texture; }

void Shape::setTextureRect(const sf::IntRect& rect) {
    m_textureRect = rect;
    updateTexCoords();
}

const sf::IntRect& Shape::getTextureRect() const { return m_textureRect; }

void Shape::setFillColor(const sf::Color& color) {
    m_fillColor = color;
    updateFillColors();
}

const sf::Color& Shape::getFillColor() const { return m_fillColor; }

void Shape::setOutlineColor(const sf::Color& color) {
    m_outlineColor = color;
    updateOutlineColors();
}

const sf::Color& Shape::getOutlineColor() const { return m_outlineColor; }

void Shape::setOutlineThickness(float thickness) {
    m_outlineThickness = thickness;
    update();  // recompute everything because the whole shape must be
               // offset
}

float Shape::getOutlineThickness() const { return m_outlineThickness; }

sf::FloatRect Shape::getLocalBounds() const { return m_bounds; }

sf::FloatRect Shape::getGlobalBounds() const {
    return getTransform().transformRect(getLocalBounds());
}

Shape::Shape()
    : m_texture(NULL), m_textureRect(), m_fillColor(255, 255, 255),
      m_outlineColor(255, 255, 255), m_outlineThickness(0),
      m_vertices(sf::TriangleFan), m_outlineVertices(sf::TriangleStrip),
      m_insideBounds(), m_bounds(), m_origin(0, 0), m_position(0, 0),
      m_rotation(0), m_scale(1, 1), m_transform(), m_transformNeedUpdate(true),
      m_inverseTransform(), m_inverseTransformNeedUpdate(true) {}

void Shape::update() {
    // Get the total number of points of the shape
    std::size_t count = getPointCount();
    if (count < 3) {
        m_vertices.resize(0);
        m_outlineVertices.resize(0);
        return;
    }

    m_vertices.resize(count + 2);  // + 2 for center and repeated first point

    // Position
    for (std::size_t i = 0; i < count; ++i)
        m_vertices[i + 1].position = getPoint(i);
    m_vertices[count + 1].position = m_vertices[1].position;

    // Update the bounding rectangle
    m_vertices[0] =
        m_vertices[1];  // so that the result of getBounds() is correct
    m_insideBounds = m_vertices.getBounds();

    // Compute the center and make it the first vertex
    m_vertices[0].position.x = m_insideBounds.left + m_insideBounds.width / 2;
    m_vertices[0].position.y = m_insideBounds.top + m_insideBounds.height / 2;

    // Color
    updateFillColors();

    // sf::Texture coordinates
    updateTexCoords();

    // Outline
    updateOutline();
}

void Shape::draw(sf::RenderWindow& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    // Render the inside
    states.texture = m_texture;
    target.draw(m_vertices, states);

    // Render the outline
    if (m_outlineThickness != 0) {
        states.texture = NULL;
        target.draw(m_outlineVertices, states);
    }
}

void Shape::updateFillColors() {
    for (std::size_t i = 0; i < m_vertices.getVertexCount(); ++i)
        m_vertices[i].color = m_fillColor;
}

void Shape::updateTexCoords() {
    sf::FloatRect convertedTextureRect = sf::FloatRect(m_textureRect);

    for (std::size_t i = 0; i < m_vertices.getVertexCount(); ++i) {
        float xratio = m_insideBounds.width > 0
                           ? (m_vertices[i].position.x - m_insideBounds.left) /
                                 m_insideBounds.width
                           : 0;
        float yratio = m_insideBounds.height > 0
                           ? (m_vertices[i].position.y - m_insideBounds.top) /
                                 m_insideBounds.height
                           : 0;
        m_vertices[i].texCoords.x =
            convertedTextureRect.left + convertedTextureRect.width * xratio;
        m_vertices[i].texCoords.y =
            convertedTextureRect.top + convertedTextureRect.height * yratio;
    }
}

void Shape::updateOutline() {
    // Return if there is no outline
    if (m_outlineThickness == 0.f) {
        m_outlineVertices.clear();
        m_bounds = m_insideBounds;
        return;
    }

    std::size_t count = m_vertices.getVertexCount() - 2;
    m_outlineVertices.resize((count + 1) * 2);

    for (std::size_t i = 0; i < count; ++i) {
        std::size_t index = i + 1;

        // Get the two segments shared by the current point
        sf::Vector2f p0 = (i == 0) ? m_vertices[count].position
                                   : m_vertices[index - 1].position;
        sf::Vector2f p1 = m_vertices[index].position;
        sf::Vector2f p2 = m_vertices[index + 1].position;

        // Compute their normal
        sf::Vector2f n1 = computeNormal(p0, p1);
        sf::Vector2f n2 = computeNormal(p1, p2);

        // Make sure that the normals point towards the outside of the shape
        // (this depends on the order in which the points were defined)
        if (dotProduct(n1, m_vertices[0].position - p1) > 0) n1 = -n1;
        if (dotProduct(n2, m_vertices[0].position - p1) > 0) n2 = -n2;

        // Combine them to get the extrusion direction
        float factor = 1.f + (n1.x * n2.x + n1.y * n2.y);
        sf::Vector2f normal = (n1 + n2) / factor;

        // Update the outline points
        sf::Vector2f offset = normal * (m_outlineThickness / 2.f);
        m_outlineVertices[i * 2 + 0].position = p1 - offset;
        m_outlineVertices[i * 2 + 1].position = p1 + offset;
    }

    // Duplicate the first point at the end, to close the outline
    m_outlineVertices[count * 2 + 0].position = m_outlineVertices[0].position;
    m_outlineVertices[count * 2 + 1].position = m_outlineVertices[1].position;

    // Update outline colors
    updateOutlineColors();

    // Update the shape's bounds
    m_bounds = m_outlineVertices.getBounds();
}

void Shape::updateOutlineColors() {
    for (std::size_t i = 0; i < m_outlineVertices.getVertexCount(); ++i)
        m_outlineVertices[i].color = m_outlineColor;
}

const sf::Transform& Shape::getTransform() const {
    // Recompute the combined transform if needed
    if (m_transformNeedUpdate) {
        float angle = -m_rotation * 3.141592654f / 180.f;
        float cosine = std::cos(angle);
        float sine = std::sin(angle);
        float sxc = m_scale.x * cosine;
        float syc = m_scale.y * cosine;
        float sxs = m_scale.x * sine;
        float sys = m_scale.y * sine;
        float tx = -m_origin.x * sxc - m_origin.y * sys + m_position.x;
        float ty = m_origin.x * sxs - m_origin.y * syc + m_position.y;

        m_transform = sf::Transform(sxc, sys, tx, -sxs, syc, ty, 0.f, 0.f, 1.f);
        m_transformNeedUpdate = false;
    }

    return m_transform;
}

const sf::Transform& Shape::getInverseTransform() const {
    // Recompute the inverse transform if needed
    if (m_inverseTransformNeedUpdate) {
        m_inverseTransform = getTransform().getInverse();
        m_inverseTransformNeedUpdate = false;
    }

    return m_inverseTransform;
}

////////////////////////////////////////////////////////////
void Shape::setPosition(float x, float y) {
    m_position.x = x;
    m_position.y = y;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}

////////////////////////////////////////////////////////////
void Shape::setPosition(const sf::Vector2f& position) {
    setPosition(position.x, position.y);
}
