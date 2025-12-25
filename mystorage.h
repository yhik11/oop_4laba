#ifndef MYSTORAGE_H
#define MYSTORAGE_H

#include "shape.h"
#include "observer.h"
#include <vector>
#include <memory>
#include <string>

class MyStorage : public Subject {
private:
    std::vector<std::shared_ptr<Shape>> shapes;

public:
    MyStorage();
    ~MyStorage();

    // Основные методы
    void add(Shape* shape);
    void addShared(std::shared_ptr<Shape> shape);
    Shape* getObject(int index) const;
    std::shared_ptr<Shape> getSharedPtr(int index) const;
    void remove(int index);
    int getCount() const;
    void clear();

    // Методы для выделения
    void selectAt(int x, int y);
    void deselectAll();
    void removeSelected();

    // Вспомогательные методы
    bool isEmpty() const;
    int countSelected() const;

    // Методы для сериализации
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);

    // Для итерации
    std::vector<std::shared_ptr<Shape>>& getAll() { return shapes; }
    const std::vector<std::shared_ptr<Shape>>& getAll() const { return shapes; }
};

#endif
