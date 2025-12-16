#include "mystorage.h"

MyStorage::MyStorage() {
    // Конструктор пустой
}

MyStorage::~MyStorage() {
    clear();
}

void MyStorage::clear() {
    for (auto* shape : shapes) {
        delete shape;
    }
    shapes.clear();
}

void MyStorage::add(Shape* shape) {
    shapes.push_back(shape);
}

void MyStorage::remove(int index) {
    if (index >= 0 && index < (int)shapes.size()) {
        delete shapes[index];
        shapes.erase(shapes.begin() + index);
    }
}

Shape* MyStorage::getObject(int index) const {
    if (index >= 0 && index < (int)shapes.size()) {
        return shapes[index];
    }
    return nullptr;
}

int MyStorage::getCount() const {
    return (int)shapes.size();
}

void MyStorage::selectAt(int x, int y) {
    // Ищем с конца (верхние фигуры в приоритете)
    for (int i = (int)shapes.size() - 1; i >= 0; i--) {
        if (shapes[i]->contains(x, y)) {
            shapes[i]->setSelected(true);
            break;
        }
    }
}

void MyStorage::deselectAll() {
    for (auto* shape : shapes) {
        shape->setSelected(false);
    }
}

void MyStorage::removeSelected() {
    for (size_t i = 0; i < shapes.size(); ) {
        if (shapes[i]->isSelected()) {
            delete shapes[i];
            shapes.erase(shapes.begin() + i);
        } else {
            i++;
        }
    }
}

bool MyStorage::isEmpty() const {
    return shapes.empty();
}

int MyStorage::countSelected() const {
    int count = 0;
    for (auto* shape : shapes) {
        if (shape->isSelected()) {
            count++;
        }
    }
    return count;
}
