#include "objecttree.h"
#include "mystorage.h"
#include "shape.h"
#include "circle.h"      // ← ДОБАВЬ
#include "rectangle.h"   // ← ДОБАВЬ
#include "triangle.h"    // ← ДОБАВЬ
#include "line.h"        // ← ДОБАВЬ
#include "group.h"       // ← ДОБАВЬ
#include <QDockWidget>   // ← ДОБАВЬ для QDockWidget
#include <QHeaderView>
#include <QMenu>
#include <iostream>

ObjectTree::ObjectTree(QWidget* parent)
    : QTreeWidget(parent), storage(nullptr) {

    setColumnCount(2);
    QStringList headers;
    headers << "Объект" << "Тип";
    setHeaderLabels(headers);

    setSelectionMode(QAbstractItemView::SingleSelection);

    connect(this, &QTreeWidget::itemClicked,
            this, &ObjectTree::onItemClicked);
}

ObjectTree::~ObjectTree() {
    if (storage) {
        // Отписываемся от хранилища
    }
}

void ObjectTree::setStorage(MyStorage* newStorage) {
    if (storage) {
        storage->removeObserver(this);
    }

    storage = newStorage;

    if (storage) {
        storage->addObserver(this);
        buildTree();
    }
}

void ObjectTree::buildTree() {
    clear();

    if (!storage) return;

    for (int i = 0; i < storage->getCount(); i++) {
        ::Shape* shape = storage->getObject(i);
        if (!shape) continue;

        QTreeWidgetItem* item = new QTreeWidgetItem(this);

        // Используем getTypeId() вместо dynamic_cast
        QString typeName;
        int typeId = shape->getTypeId();

        switch (typeId) {
        case 1: typeName = "Круг"; break;
        case 2: typeName = "Прямоугольник"; break;
        case 3: typeName = "Треугольник"; break;
        case 4: typeName = "Линия"; break;
        case 99: typeName = "Группа"; break;
        default: typeName = "Фигура"; break;
        }

        item->setText(0, QString("Объект %1").arg(i + 1));
        item->setText(1, typeName);
        item->setData(0, Qt::UserRole, i);

        // Для групп добавляем детей
        if (typeId == 99) {
            Group* group = dynamic_cast<Group*>(shape);
            if (group) {
                for (int j = 0; j < group->getChildCount(); j++) {
                    ::Shape* child = group->getChild(j);
                    if (child) {
                        QTreeWidgetItem* childItem = new QTreeWidgetItem(item);
                        childItem->setText(0, QString("Ребёнок %1").arg(j + 1));

                        int childTypeId = child->getTypeId();
                        QString childTypeName;
                        switch (childTypeId) {
                        case 1: childTypeName = "Круг"; break;
                        case 2: childTypeName = "Прямоугольник"; break;
                        case 3: childTypeName = "Треугольник"; break;
                        case 4: childTypeName = "Линия"; break;
                        case 99: childTypeName = "Группа"; break;
                        default: childTypeName = "Фигура"; break;
                        }

                        childItem->setText(1, childTypeName);
                    }
                }
            }
        }
    }

    expandAll();
}

// Observer методы
void ObjectTree::update(const std::string& message) {
    // Перестраиваем дерево при любом изменении
    buildTree();
}

void ObjectTree::onObjectAdded(int index) {
    buildTree();  // Упрощённо - перестраиваем всё дерево
}

void ObjectTree::onObjectRemoved(int index) {
    buildTree();
}

void ObjectTree::onObjectChanged(int index) {
    buildTree();
}

void ObjectTree::onSelectionChanged() {
    // Выделяем в дереве то же, что выделено на холсте
    clearSelection();

    if (!storage) return;

    for (int i = 0; i < storage->getCount(); i++) {
        if (storage->getObject(i)->isSelected()) {
            // Ищем элемент с соответствующим индексом
            QList<QTreeWidgetItem*> items = findItems("*", Qt::MatchWildcard);
            for (auto item : items) {
                int idx = item->data(0, Qt::UserRole).toInt();
                if (idx == i) {
                    setCurrentItem(item);
                    break;
                }
            }
        }
    }
}

void ObjectTree::onStorageCleared() {
    clear();
}

void ObjectTree::onItemClicked(QTreeWidgetItem* item, int column) {
    if (!storage || !item) return;

    // Получаем индекс из UserRole
    int index = item->data(0, Qt::UserRole).toInt();

    if (index >= 0 && index < storage->getCount()) {
        // Выделяем только этот объект
        for (int i = 0; i < storage->getCount(); i++) {
            storage->getObject(i)->setSelected(i == index);
        }

        // Уведомляем других наблюдателей
        if (storage) {
            // Вызываем метод уведомления о изменении выделения
            // Это зависит от твоей реализации Subject
        }
    }
}

void ObjectTree::clearTree() {
    clear();
}

void ObjectTree::buildTreeNode(QTreeWidgetItem* parent, Shape* shape, int storageIndex) {
    // Пока используем упрощённый buildTree
}

QTreeWidgetItem* ObjectTree::findItemByShape(Shape* shape) {
    // Упрощённая реализация
    return nullptr;
}
