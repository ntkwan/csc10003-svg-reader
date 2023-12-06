#include "Group.hpp"

Group::Group() {}

Group::Group(Attributes attributes) : attributes(attributes) {}

Group::~Group() {
    for (auto& shape : shapes) {
        delete shape;
    }
}

std::string Group::getClass() const { return "Group"; }

void Group::render(Gdiplus::Graphics& graphics,
                   const Renderer& renderer) const {
    for (auto shape : shapes) {
        renderer.draw(graphics, shape);
    }
}

Attributes Group::getAttributes() const { return attributes; }

void Group::addElement(SVGElement* shape) {
    shapes.push_back(shape);
    shape->setParent(this);
}

std::vector< SVGElement* > Group::getElements() const { return shapes; }

void Group::printData() const {
    std::cout << "Group: " << std::endl;
    for (auto shape : shapes) {
        std::cout << "    ";
        shape->printData();
        std::cout << std::endl;
    }
}