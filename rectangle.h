#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

class Rectangle : public Shape {
private:
    int width, height;

public:
    Rectangle(int x = 0, int y = 0, int width = 40, int height = 30,
              const QColor& color = Qt::green);

    void draw(QPainter& painter) const override;
    bool contains(int mx, int my) const override;
    QRect getBounds() const override;
    void resize(float scale) override;
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    void setSize(int w, int h) { width = w; height = h; }
    int getTypeId() const override { return 2; }
    void save(std::ofstream& out) const override;
};

#endif
