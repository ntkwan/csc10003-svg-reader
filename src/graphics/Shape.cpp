#include "Shape.hpp"

#include <cmath>

namespace {
    Vector2Df computeNormal(const Vector2Df& p1, const Vector2Df& p2) {
        Vector2Df normal(p1.y - p2.y, p2.x - p1.x);
        float length = std::sqrt(normal.x * normal.x + normal.y * normal.y);
        if (length != 0.f) normal /= length;
        return normal;
    }

    float dotProduct(const Vector2Df& p1, const Vector2Df& p2) {
        return p1.x * p2.x + p1.y * p2.y;
    }
}  // namespace

void Shape::setFillColor(const Color& color) {
    fill_color = color;
    updateFillColors();
}

const Color& Shape::getFillColor() const { return fill_color; }

void Shape::setOutlineColor(const Color& color) {
    outline_color = color;
    updateOutlineColors();
}

const Color& Shape::getOutlineColor() const { return outline_color; }

void Shape::setOutlineThickness(float thickness) {
    outline_thickness = thickness;
    update();  // recompute everything because the whole shape must be
               // offset
}

float Shape::getOutlineThickness() const { return outline_thickness; }

Shape::Shape()
    : fill_color(255, 255, 255), outline_color(255, 255, 255),
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
        vertices[i + 1].position = sf::Vector2f(getPoint(i).x, getPoint(i).y);
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

void Shape::draw(Renderer& target) const {
    const float* mat = getTransform().getMatrix();
    sf::Transform transform =
        sf::Transform(mat[0], mat[4], mat[12], mat[1], mat[5], mat[13], mat[3],
                      mat[7], mat[15]);
    // Render the inside
    target.window.draw(vertices, transform);

    // Render the outline
    if (outline_thickness != 0) {
        target.window.draw(outline_vertices, transform);
    }
}

void Shape::updateFillColors() {
    for (std::size_t i = 0; i < vertices.getVertexCount(); ++i)
        vertices[i].color =
            sf::Color(fill_color.r, fill_color.g, fill_color.b, fill_color.a);
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
        Vector2Df p0 = (i == 0) ? Vector2Df(vertices[count].position.x,
                                            vertices[count].position.y)
                                : Vector2Df(vertices[index - 1].position.x,
                                            vertices[index - 1].position.y);
        Vector2Df p1 =
            Vector2Df(vertices[index].position.x, vertices[index].position.y);
        Vector2Df p2 = Vector2Df(vertices[index + 1].position.x,
                                 vertices[index + 1].position.y);

        // Compute their normal
        Vector2Df n1 = computeNormal(p0, p1);
        Vector2Df n2 = computeNormal(p1, p2);

        // Make sure that the normals point towards the outside of the shape
        // (this depends on the order in which the points were defined)
        if (dotProduct(
                n1, Vector2Df(vertices[0].position.x, vertices[0].position.y) -
                        p1) > 0)
            n1 = -n1;
        if (dotProduct(
                n2, Vector2Df(vertices[0].position.x, vertices[0].position.y) -
                        p1) > 0)
            n2 = -n2;

        // Combine them to get the extrusion direction
        float factor = 1.f + (n1.x * n2.x + n1.y * n2.y);
        Vector2Df normal = (n1 + n2) / factor;

        // Update the outline points
        Vector2Df offset = normal * (outline_thickness / 2.f);
        Vector2Df result = p1 - offset;
        outline_vertices[i * 2 + 0].position = sf::Vector2f(result.x, result.y);
        result = p1 + offset;
        outline_vertices[i * 2 + 1].position = sf::Vector2f(result.x, result.y);
    }

    // Duplicate the first point at the end, to close the outline
    outline_vertices[count * 2 + 0].position = outline_vertices[0].position;
    outline_vertices[count * 2 + 1].position = outline_vertices[1].position;

    // Update outline colors
    updateOutlineColors();

    // Update the shape's bounds
    bounds = outline_vertices.getBounds();
}

#include <iostream>
void Shape::translate(float x, float y) {
    position.x += x * std::cos(rotation * acos(-1) / 180.f) -
                  y * std::sin(rotation * acos(-1) / 180.f);
    position.y += x * std::sin(rotation * acos(-1) / 180.f) +
                  y * std::cos(rotation * acos(-1) / 180.f);
    transform_need_update = true;
    inverse_transform_need_update = true;
}

void Shape::rotate(float angle) {
    rotation += angle;
    transform_need_update = true;
    inverse_transform_need_update = true;
}

void Shape::updateOutlineColors() {
    for (std::size_t i = 0; i < outline_vertices.getVertexCount(); ++i)
        outline_vertices[i].color = sf::Color(outline_color.r, outline_color.g,
                                              outline_color.b, outline_color.a);
}

const Transform& Shape::getTransform() const {
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

        transform = Transform(sxc, sys, tx, -sxs, syc, ty, 0.f, 0.f, 1.f);
        transform_need_update = false;
    }

    return transform;
}

const Transform& Shape::getInverseTransform() const {
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

void Shape::setPosition(const Vector2Df& position) {
    setPosition(position.x, position.y);
}
