#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"

class Triangle : public Shape {
private:
    int size;

public:
    Triangle(int x = 0, int y = 0, int size = 30,
             const QColor& color = Qt::blue);

    void draw(QPainter& painter) const override;
    bool contains(int mx, int my) const override;
    QRect getBounds() const override;
};

#endif
