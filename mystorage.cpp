#include "mystorage.h"
#include <fstream>
#include <iostream>
#include <limits>

MyStorage::MyStorage() {
}

MyStorage::~MyStorage() {
    clear();
}

void MyStorage::clear() {
    shapes.clear();
    notifyStorageCleared();
    notifyUpdate("storage_cleared");
}

void MyStorage::add(Shape* shape) {
    if (shape) {
        shapes.push_back(std::shared_ptr<Shape>(shape));
        notifyObjectAdded(static_cast<int>(shapes.size()) - 1);
        notifyUpdate("object_added");
    }
}

void MyStorage::addShared(std::shared_ptr<Shape> shape) {
    if (shape) {
        shapes.push_back(shape);
        notifyObjectAdded(static_cast<int>(shapes.size()) - 1);
        notifyUpdate("object_added");
    }
}

Shape* MyStorage::getObject(int index) const {
    if (index >= 0 && index < static_cast<int>(shapes.size())) {
        return shapes[index].get();
    }
    return nullptr;
}

std::shared_ptr<Shape> MyStorage::getSharedPtr(int index) const {
    if (index >= 0 && index < static_cast<int>(shapes.size())) {
        return shapes[index];
    }
    return nullptr;
}

void MyStorage::remove(int index) {
    if (index >= 0 && index < static_cast<int>(shapes.size())) {
        shapes.erase(shapes.begin() + index);
        notifyObjectRemoved(index);
        notifyUpdate("object_removed");
    }
}

int MyStorage::getCount() const {
    return static_cast<int>(shapes.size());
}

void MyStorage::selectAt(int x, int y) {
    for (int i = static_cast<int>(shapes.size()) - 1; i >= 0; i--) {
        if (shapes[i]->contains(x, y)) {
            shapes[i]->setSelected(true);
            notifySelectionChanged();
            break;
        }
    }
}

void MyStorage::deselectAll() {
    for (auto& shape : shapes) {
        shape->setSelected(false);
    }
    notifySelectionChanged();
}

void MyStorage::removeSelected() {
    std::vector<int> toRemove;

    for (int i = 0; i < static_cast<int>(shapes.size()); i++) {
        if (shapes[i]->isSelected()) {
            toRemove.push_back(i);
        }
    }

    for (auto it = toRemove.rbegin(); it != toRemove.rend(); ++it) {
        remove(*it);
    }
}

bool MyStorage::isEmpty() const {
    return shapes.empty();
}

int MyStorage::countSelected() const {
    int count = 0;
    for (const auto& shape : shapes) {
        if (shape->isSelected()) {
            count++;
        }
    }
    return count;
}

bool MyStorage::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла для записи: " << filename << std::endl;
        return false;
    }

    file << shapes.size() << std::endl;

    for (const auto& shape : shapes) {
        shape->save(file);
    }

    file.close();
    std::cout << "Сохранено " << shapes.size() << " фигур в файл: " << filename << std::endl;
    return true;
}

bool MyStorage::loadFromFile(const std::string& filename) {
    std::cout << "Попытка загрузки из: " << filename << std::endl;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ОШИБКА: Не удалось открыть файл: " << filename << std::endl;
        return false;
    }

    clear();

    int count;
    file >> count;

    if (file.fail()) {
        std::cerr << "ОШИБКА: Не могу прочитать количество фигур" << std::endl;
        return false;
    }

    std::cout << "Найдено фигур в файле: " << count << std::endl;

    // Пропускаем перевод строки после count
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (int i = 0; i < count; i++) {
        if (file.eof()) {
            std::cerr << "ОШИБКА: Неожиданный конец файла" << std::endl;
            break;
        }

        auto shape = Shape::load(file);
        if (shape) {
            shapes.push_back(shape);
            notifyObjectAdded(static_cast<int>(shapes.size()) - 1);
            std::cout << "Успешно загружена фигура #" << i << std::endl;
        } else {
            std::cerr << "ОШИБКА: Не удалось загрузить фигуру #" << i << std::endl;
            // Пропускаем до конца строки
            file.clear();
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        // Пропускаем возможные пустые строки
        while (file.peek() == '\n' || file.peek() == '\r') {
            file.ignore();
        }
    }

    file.close();
    std::cout << "Итого загружено: " << shapes.size() << " фигур" << std::endl;
    notifyUpdate("storage_loaded");
    return true;
}
