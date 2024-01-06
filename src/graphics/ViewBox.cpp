#include "ViewBox.hpp"

ViewBox::ViewBox() : x(0), y(0), w(0), h(0) {}

ViewBox::ViewBox(float X, float Y, float W, float H) : x(X), y(Y), w(W), h(H) {}

float ViewBox::getX() const { return x; }

float ViewBox::getY() const { return y; }

float ViewBox::getWidth() const { return w; }

float ViewBox::getHeight() const { return h; }