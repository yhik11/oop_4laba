#ifndef OBJECTTREE_H
#define OBJECTTREE_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "observer.h"
#include "mystorage.h"

class MyStorage;
class Shape;

class ObjectTree : public QTreeWidget, public Observer {
    Q_OBJECT

private:
    MyStorage* storage;

    // Вспомогательные методы
    void buildTree();
    void buildTreeNode(QTreeWidgetItem* parent, Shape* shape, int storageIndex);
    QTreeWidgetItem* findItemByShape(Shape* shape);
    void clearTree();

public:
    ObjectTree(QWidget* parent = nullptr);
    ~ObjectTree();

    void setStorage(MyStorage* storage);

    // Observer interface
    void update(const std::string& message) override;
    void onObjectAdded(int index) override;
    void onObjectRemoved(int index) override;
    void onObjectChanged(int index) override;
    void onSelectionChanged() override;
    void onStorageCleared() override;

private slots:
    void onItemClicked(QTreeWidgetItem* item, int column);
};

#endif
