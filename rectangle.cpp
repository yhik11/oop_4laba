#include "rectangle.h"

Rectangle::Rectangle(int x, int y, int width, int height, const QColor& color)
    : Shape(x, y, color), width(width), height(height) {
}

void Rectangle::draw(QPainter& painter) const {
    painter.setBrush(selected ? QColor(150, 200, 100) : color);
    painter.setPen(QPen(Qt::black, 1));
    painter.drawRect(x - width/2, y - height/2, width, height);
}

bool Rectangle::contains(int mx, int my) const {
    return QRect(x - width/2, y - height/2, width, height).contains(mx, my);
}

QRect Rectangle::getBounds() const {
    return QRect(x - width/2, y - height/2, width, height);
}
