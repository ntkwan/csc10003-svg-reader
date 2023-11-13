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

void Shape::setFillColor(const sf::Color& color) {
    fill_color = color;
    updateFillColors();
}

const sf::Color& Shape::getFillColor() const { return fill_color; }

void Shape::setOutlineColor(const sf::Color& color) {
    outline_color = color;
    updateOutlineColors();
}

const sf::Color& Shape::getOutlineColor() const { return outline_color; }

void Shape::setOutlineThickness(float thickness) {
    outline_thickness = thickness;
    update();  // recompute everything because the whole shape must be
               // offset
}

float Shape::getOutlineThickness() const { return outline_thickness; }

Shape::Shape()
    : texture(nullptr), fill_color(255, 255, 255), outline_color(255, 255, 255),
      outline_thickness(0), vertices(sf::TriangleFan),
      outline_vertices(sf::TriangleStrip), inside_bounds(), bounds(),
      origin(0, 0), position(0, 0), rotation(0), scale(1, 1), transform(),
      transform_need_update(true), inverse_transform(),
      inverse_transform_need_update(true) {}

void Shape::update() {
    // Get the total number of points of the shape
    std::size_t count = getPointCount();
    if (count < 3) {
        vertices.resize(0);
        outline_vertices.resize(0);
        return;
    }

    vertices.resize(count + 2);  // + 2 for center and repeated first point

    // Position
    for (std::size_t i = 0; i < count; ++i)
        vertices[i + 1].position = getPoint(i);
    vertices[count + 1].position = vertices[1].position;

    // Update the bounding rectangle
    vertices[0] = vertices[1];  // so that the result of getBounds() is correct
    inside_bounds = vertices.getBounds();

    // Compute the center and make it the first vertex
    vertices[0].position.x = inside_bounds.left + inside_bounds.width / 2;
    vertices[0].position.y = inside_bounds.top + inside_bounds.height / 2;

    // Color
    updateFillColors();

    // Outline
    updateOutline();
}

void Shape::draw(sf::RenderWindow& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    // Render the inside
    states.texture = texture;
    target.draw(vertices, states);

    // Render the outline
    if (outline_thickness != 0) {
        states.texture = nullptr;
        target.draw(outline_vertices, states);
    }
}

void Shape::updateFillColors() {
    for (std::size_t i = 0; i < vertices.getVertexCount(); ++i)
        vertices[i].color = fill_color;
}

void Shape::updateOutline() {
    // Return if there is no outline
    if (outline_thickness == 0.f) {
        outline_vertices.clear();
        bounds = inside_bounds;
        return;
    }

    std::size_t count = vertices.getVertexCount() - 2;
    outline_vertices.resize((count + 1) * 2);

    for (std::size_t i = 0; i < count; ++i) {
        std::size_t index = i + 1;

        // Get the two segments shared by the current point
        sf::Vector2f p0 =
            (i == 0) ? vertices[count].position : vertices[index - 1].position;
        sf::Vector2f p1 = vertices[index].position;
        sf::Vector2f p2 = vertices[index + 1].position;

        // Compute their normal
        sf::Vector2f n1 = computeNormal(p0, p1);
        sf::Vector2f n2 = computeNormal(p1, p2);

        // Make sure that the normals point towards the outside of the shape
        // (this depends on the order in which the points were defined)
        if (dotProduct(n1, vertices[0].position - p1) > 0) n1 = -n1;
        if (dotProduct(n2, vertices[0].position - p1) > 0) n2 = -n2;

        // Combine them to get the extrusion direction
        float factor = 1.f + (n1.x * n2.x + n1.y * n2.y);
        sf::Vector2f normal = (n1 + n2) / factor;

        // Update the outline points
        sf::Vector2f offset = normal * (outline_thickness / 2.f);
        outline_vertices[i * 2 + 0].position = p1 - offset;
        outline_vertices[i * 2 + 1].position = p1 + offset;
    }

    // Duplicate the first point at the end, to close the outline
    outline_vertices[count * 2 + 0].position = outline_vertices[0].position;
    outline_vertices[count * 2 + 1].position = outline_vertices[1].position;

    // Update outline colors
    updateOutlineColors();

    // Update the shape's bounds
    bounds = outline_vertices.getBounds();
}

void Shape::updateOutlineColors() {
    for (std::size_t i = 0; i < outline_vertices.getVertexCount(); ++i)
        outline_vertices[i].color = outline_color;
}

const sf::Transform& Shape::getTransform() const {
    // Recompute the combined transform if needed
    if (transform_need_update) {
        float angle = -rotation * acos(-1) / 180.f;
        float cosine = std::cos(angle);
        float sine = std::sin(angle);
        float sxc = scale.x * cosine;
        float syc = scale.y * cosine;
        float sxs = scale.x * sine;
        float sys = scale.y * sine;
        float tx = -origin.x * sxc - origin.y * sys + position.x;
        float ty = origin.x * sxs - origin.y * syc + position.y;

        transform = sf::Transform(sxc, sys, tx, -sxs, syc, ty, 0.f, 0.f, 1.f);
        transform_need_update = false;
    }

    return transform;
}

const sf::Transform& Shape::getInverseTransform() const {
    // Recompute the inverse transform if needed
    if (inverse_transform_need_update) {
        inverse_transform = getTransform().getInverse();
        inverse_transform_need_update = false;
    }

    return inverse_transform;
}

void Shape::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
    transform_need_update = true;
    inverse_transform_need_update = true;
}

void Shape::setPosition(const sf::Vector2f& position) {
    setPosition(position.x, position.y);
}
