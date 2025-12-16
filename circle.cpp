#include "circle.h"
#include <cmath>

Circle::Circle(int x, int y, int radius, const QColor& color)
    : Shape(x, y, color), radius(radius) {
}

void Circle::draw(QPainter& painter) const {
    painter.setBrush(selected ? QColor(100, 150, 255) : color);
    painter.setPen(QPen(Qt::black, 1));
    painter.drawEllipse(x - radius, y - radius, radius * 2, radius * 2);
}

bool Circle::contains(int mx, int my) const {
    int dx = mx - x;
    int dy = my - y;
    return (dx * dx + dy * dy) <= (radius * radius);
}

QRect Circle::getBounds() const {
    return QRect(x - radius, y - radius, radius * 2, radius * 2);
}

void Circle::resize(float scale) {
    radius = std::max(5, std::min(200, (int)(radius * scale)));
}
