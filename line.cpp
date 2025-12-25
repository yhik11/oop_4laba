#include "line.h"

Line::Line(int x1, int y1, int x2, int y2, const QColor& color)
    : Shape(x1, y1, color), x2(x2), y2(y2) {
}

void Line::draw(QPainter& painter) const {
    QPen pen(selected ? QColor(255, 200, 100) : color, 2);
    painter.setPen(pen);
    painter.drawLine(x, y, x2, y2);
}

bool Line::contains(int mx, int my) const {
    // Упрощённая проверка - расстояние до линии
    const int tolerance = 5;

    // Вектор линии
    int vx = x2 - x;
    int vy = y2 - y;

    // Вектор от начала линии до точки
    int wx = mx - x;
    int wy = my - y;

    // Проекция
    double dot = wx * vx + wy * vy;
    double lenSq = vx * vx + vy * vy;

    if (lenSq == 0) return false;

    double proj = dot / lenSq;
    if (proj < 0 || proj > 1) return false;

    // Ближайшая точка на линии
    int closestX = x + static_cast<int>(proj * vx);
    int closestY = y + static_cast<int>(proj * vy);

    // Расстояние до линии
    int dx = mx - closestX;
    int dy = my - closestY;

    return (dx * dx + dy * dy) <= (tolerance * tolerance);
}

QRect Line::getBounds() const {
    int left = std::min(x, x2);
    int top = std::min(y, y2);
    int right = std::max(x, x2);
    int bottom = std::max(y, y2);

    return QRect(left, top, right - left, bottom - top);
}

void Line::move(int dx, int dy) {
    // Двигаем обе точки линии
    x += dx;
    y += dy;
    x2 += dx;
    y2 += dy;
}

void Line::resize(float scale) {
    // Для линии можно масштабировать относительно центра
    int centerX = (x + x2) / 2;
    int centerY = (y + y2) / 2;

    // Новые координаты относительно центра
    int newX1 = centerX + static_cast<int>((x - centerX) * scale);
    int newY1 = centerY + static_cast<int>((y - centerY) * scale);
    int newX2 = centerX + static_cast<int>((x2 - centerX) * scale);
    int newY2 = centerY + static_cast<int>((y2 - centerY) * scale);

    x = newX1;
    y = newY1;
    x2 = newX2;
    y2 = newY2;
}

#include <fstream>

void Line::save(std::ofstream& out) const {
    out << getTypeId() << " " << x << " " << y << " ";
    out << color.red() << " " << color.green() << " " << color.blue() << " ";
    out << x2 << " " << y2 << "\n";
}
