#include "shape.h"
#include <fstream>
#include "circle.h"
#include "rectangle.h"
#include "triangle.h"
#include "line.h"
#include "group.h"
#include <iostream>

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
    if (in.eof()) {
        return nullptr;
    }

    int typeId;
    in >> typeId;

    if (in.fail()) {
        std::cerr << "Ошибка чтения typeId" << std::endl;
        return nullptr;
    }

    std::cout << "Загружаем объект typeId=" << typeId << std::endl;

    switch (typeId) {
    case 1: { // Circle
        int x, y, r, g, b, radius;
        in >> x >> y >> r >> g >> b >> radius;
        if (in.fail()) {
            std::cerr << "Ошибка чтения Circle" << std::endl;
            return nullptr;
        }
        return std::make_shared<Circle>(x, y, radius, QColor(r, g, b));
    }
    case 2: { // Rectangle
        int x, y, r, g, b, width, height;
        in >> x >> y >> r >> g >> b >> width >> height;
        if (in.fail()) {
            std::cerr << "Ошибка чтения Rectangle" << std::endl;
            return nullptr;
        }
        return std::make_shared<Rectangle>(x, y, width, height, QColor(r, g, b));
    }
    case 3: { // Triangle
        int x, y, r, g, b, size;
        in >> x >> y >> r >> g >> b >> size;
        if (in.fail()) {
            std::cerr << "Ошибка чтения Triangle" << std::endl;
            return nullptr;
        }
        return std::make_shared<Triangle>(x, y, size, QColor(r, g, b));
    }
    case 4: { // Line
        int x, y, r, g, b, x2, y2;
        in >> x >> y >> r >> g >> b >> x2 >> y2;
        if (in.fail()) {
            std::cerr << "Ошибка чтения Line" << std::endl;
            return nullptr;
        }
        return std::make_shared<Line>(x, y, x2, y2, QColor(r, g, b));
    }
    case 99: { // Group
        return Group::load(in);  // Передаем поток, typeId уже считан
    }
    case 100: { // Arrow
        // Пока заглушка - нужно доработать сохранение/загрузку
        int x, y, r, g, b, dummy1, dummy2;
        in >> x >> y >> r >> g >> b >> dummy1 >> dummy2;
        // Стрелки будут создаваться отдельно
        return nullptr;
    }
    default:
        std::cerr << "Неизвестный typeId: " << typeId << std::endl;
        return nullptr;
    }
}
