#ifndef CCIRCLE_H
#define CCIRCLE_H

#include <QPainter>

class CCircle {
private:
    int x, y;
    const int radius;
    bool selected;

public:
    CCircle(int x = 0, int y = 0, int radius = 20);

    void draw(QPainter &painter) const;
    bool contains(int mx, int my) const;

    bool isSelected() const { return selected; }
    void setSelected(bool s) { selected = s; }

    int getX() const { return x; }
    int getY() const { return y; }
    int getRadius() const { return radius; }
};

#endif
