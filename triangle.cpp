#include "triangle.h"

Triangle::Triangle(int x, int y, int size, const QColor& color)
    : Shape(x, y, color), size(size) {
}

void Triangle::draw(QPainter& painter) const {
    QPolygon polygon;
    polygon << QPoint(x, y - size/2)      // Верхняя вершина
            << QPoint(x - size/2, y + size/2)  // Левая нижняя
            << QPoint(x + size/2, y + size/2); // Правая нижняя

    painter.setBrush(selected ? QColor(255, 200, 100) : color);
    painter.setPen(QPen(Qt::black, 1));
    painter.drawPolygon(polygon);
}

bool Triangle::contains(int mx, int my) const {
    QPolygon polygon;
    polygon << QPoint(x, y - size/2)
            << QPoint(x - size/2, y + size/2)
            << QPoint(x + size/2, y + size/2);

    return polygon.containsPoint(QPoint(mx, my), Qt::OddEvenFill);
}

QRect Triangle::getBounds() const {
    return QRect(x - size/2, y - size/2, size, size);
}


void Triangle::resize(float scale) {
    size = std::max(10, std::min(200, (int)(size * scale)));
}

#include <fstream>

void Triangle::save(std::ofstream& out) const {
    out << getTypeId() << " " << x << " " << y << " ";
    out << color.red() << " " << color.green() << " " << color.blue() << " ";
    out << size << "\n";
}

