#ifndef SHAPE_H
#define SHAPE_H

#include <QPainter>
#include <QRect>

class Shape {
protected:
    int x, y;
    QColor color;
    bool selected;

public:
    Shape(int x = 0, int y = 0, const QColor& color = Qt::red);
    virtual ~Shape() {}

    virtual void draw(QPainter& painter) const = 0;  // Чисто виртуальный
    virtual bool contains(int mx, int my) const = 0;
    virtual QRect getBounds() const = 0;
    virtual void resize(float scale) = 0;
    // Общие методы для всех фигур
    void move(int dx, int dy);
    void setColor(const QColor& newColor);
    void setSelected(bool s);

    bool isSelected() const { return selected; }
    QColor getColor() const { return color; }
    int getX() const { return x; }
    int getY() const { return y; }

    // Проверка выхода за границы (общая для всех)
    bool checkBounds(const QRect& area) const;


};

#endif
