#ifndef GROUP_H
#define GROUP_H

#include "shape.h"
#include <vector>
#include <memory>

class Group : public Shape {
private:
    std::vector<std::shared_ptr<Shape>> children;

public:
    Group(const QColor& color = Qt::gray);
    ~Group() override;

    // Виртуальные методы Shape
    void draw(QPainter& painter) const override;
    bool contains(int mx, int my) const override;
    QRect getBounds() const override;
    void resize(float scale) override;
    void move(int dx, int dy) override;

    // Работа с детьми
    void addShape(std::shared_ptr<Shape> shape);
    void removeShape(int index);
    int getChildCount() const { return (int)children.size(); }
    Shape* getChild(int index) const;

    // Для сериализации
    int getTypeId() const override { return 99; }
    void save(std::ofstream& out) const override;
    static std::shared_ptr<Shape> load(std::ifstream& in);
};

#endif
