#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"

class Circle : public Shape {
private:
    int radius;

public:
    Circle(int x = 0, int y = 0, int radius = 20,
           const QColor& color = Qt::red);

    void draw(QPainter& painter) const override;
    bool contains(int mx, int my) const override;
    QRect getBounds() const override;
    void resize(float scale) override;

    int getRadius() const { return radius; }
    void setRadius(int r) { radius = r; }

    int getTypeId() const override { return 1; }
    void save(std::ofstream& out) const override;
};

#endif
