#include "mystorage.h"

//MyStorage::MyStorage() {}

MyStorage::~MyStorage() {
    for (auto* circle : circles) {
        delete circle;
    }
}

MyStorage::MyStorage(int initialCapacity) {
    circles.reserve(initialCapacity);
}

void MyStorage::setObject(int index, CCircle* circle) {
    if (index >= 0 && index < circles.size()) {
        delete circles[index];
        circles[index] = circle;
    }
}

void MyStorage::add(CCircle* circle) {
    circles.push_back(circle);
}

void MyStorage::remove(int index) {
    if (index >= 0 && index < circles.size()) {
        delete circles[index];
        circles.erase(circles.begin() + index);
    }
}

CCircle* MyStorage::getObject(int index) const {
    if (index >= 0 && index < circles.size()) {
        return circles[index];
    }
    return nullptr;
}

int MyStorage::getCount() const {
    return circles.size();
}

void MyStorage::selectAt(int x, int y) {
    for (int i = circles.size() - 1; i >= 0; i--) {
        if (circles[i]->contains(x, y)) {
            circles[i]->setSelected(true);
            break;
        }
    }
}

void MyStorage::deselectAll() {
    for (auto* circle : circles) {
        circle->setSelected(false);
    }
}

void MyStorage::removeSelected() {
    for (size_t i = 0; i < circles.size(); ) {
        if (circles[i]->isSelected()) {
            delete circles[i];
            circles.erase(circles.begin() + i);
        } else {
            i++;
        }
    }
}
