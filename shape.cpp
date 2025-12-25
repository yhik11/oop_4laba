#include "shape.h"
#include <fstream>
#include "circle.h"
#include "rectangle.h"
#include "triangle.h"
#include "line.h"
#include "group.h"

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

// РЕАЛИЗАЦИЯ load() только здесь:
std::shared_ptr<Shape> Shape::load(std::ifstream& in) {
    int typeId;
    in >> typeId;

    int x, y, r, g, b;
    in >> x >> y >> r >> g >> b;
    QColor color(r, g, b);

    switch (typeId) {
    case 1: { // Circle
        int radius;
        in >> radius;
        return std::make_shared<Circle>(x, y, radius, color);
    }
    case 2: { // Rectangle
        int width, height;
        in >> width >> height;
        return std::make_shared<Rectangle>(x, y, width, height, color);
    }
    case 3: { // Triangle
        int size;
        in >> size;
        return std::make_shared<Triangle>(x, y, size, color);
    }
    case 4: { // Line
        int x2, y2;
        in >> x2 >> y2;
        return std::make_shared<Line>(x, y, x2, y2, color);
    }

    default:
        return nullptr;
    }
}
