#include "shape.h"

Shape::Shape(int x, int y, const QColor& color)
    : x(x), y(y), color(color), selected(false) {
}

void Shape::move(int dx, int dy) {
    x += dx;
    y += dy;
}

void Shape::setColor(const QColor& newColor) {
    color = newColor;
}

void Shape::setSelected(bool s) {
    selected = s;
}

bool Shape::checkBounds(const QRect& area) const {
    return area.contains(getBounds());
}
