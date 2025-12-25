#ifndef ARROW_H
#define ARROW_H

#include "shape.h"
#include "observer.h"
#include <memory>

class Arrow : public ::Shape, public Observer {
private:
    std::weak_ptr<::Shape> source;
    std::weak_ptr<::Shape> target;
    bool active;

public:
    Arrow(std::shared_ptr<::Shape> src, std::shared_ptr<::Shape> tgt);
    ~Arrow();

    // Shape interface
    void draw(QPainter& painter) const override;
    bool contains(int mx, int my) const override;
    QRect getBounds() const override;
    void resize(float scale) override;
    void move(int dx, int dy) override;
    int getTypeId() const override { return 100; }
    void save(std::ofstream& out) const override;

    // Observer interface (реагирует на перемещение источника)
    void update(const std::string& message) override;
    void onObjectAdded(int index) override {}
    void onObjectRemoved(int index) override {}
    void onObjectChanged(int index) override {}
    void onSelectionChanged() override {}
    void onStorageCleared() override {}

    // Геттеры
    std::shared_ptr<::Shape> getSource() const;
    std::shared_ptr<::Shape> getTarget() const;
    bool isActive() const { return active; }
    void setActive(bool a) { active = a; }

    // Обновление позиции при изменении источника/цели
    void updatePosition();

private:
    QPoint getSourcePoint() const;
    QPoint getTargetPoint() const;
};

#endif
