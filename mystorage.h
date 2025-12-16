#ifndef MYSTORAGE_H
#define MYSTORAGE_H

#include "shape.h"
#include <vector>

class MyStorage {
private:
    std::vector<Shape*> shapes;

public:
    MyStorage();
    ~MyStorage();

    // Основные методы
    void add(Shape* shape);
    void remove(int index);
    Shape* getObject(int index) const;
    int getCount() const;

    // Методы для выделения
    void selectAt(int x, int y);
    void deselectAll();
    void removeSelected();

    // Вспомогательные методы
    void clear();
    bool isEmpty() const;
    int countSelected() const;

    // Для итерации (опционально)
    std::vector<Shape*>& getAll() { return shapes; }
};

#endif // MYSTORAGE_H
