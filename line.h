#ifndef LINE_H
#define LINE_H

#include "shape.h"
#include <QPainter>
#include <QRect>
#include <QColor>
#include <cmath>

class Line : public Shape {
private:
    int x2, y2;

public:
    Line(int x1 = 0, int y1 = 0, int x2 = 50, int y2 = 50,
         const QColor& color = Qt::black);

    void draw(QPainter& painter) const override;
    bool contains(int mx, int my) const override;
    QRect getBounds() const override;

    void move(int dx, int dy);
    void resize(float scale) override;
    int getX2() const { return x2; }
    int getY2() const { return y2; }
    void setEndPoint(int newX2, int newY2) { x2 = newX2; y2 = newY2; }
    int getTypeId() const override { return 4; }
    void save(std::ofstream& out) const override;
};

#endif
