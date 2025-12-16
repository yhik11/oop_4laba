// circle.cpp
#include "circle.h"
#include <cmath>

CCircle::CCircle(int x, int y, int radius)
    : x(x), y(y), radius(radius) {
}

void CCircle::draw(QPainter &painter) const {
    if (selected) {
        painter.setBrush(Qt::blue);
    } else {
        painter.setBrush(Qt::red);
    }
    painter.drawEllipse(x - radius, y - radius, radius * 2, radius * 2);
}

bool CCircle::contains(int mx, int my) const {
    int dx = mx - x;
    int dy = my - y;
    return (dx * dx + dy * dy) <= (radius * radius);
}
