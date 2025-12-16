#ifndef MYSTORAGE_H
#define MYSTORAGE_H

#include "circle.h"
#include <vector>

class MyStorage {
private:
    std::vector<CCircle*> circles;

public:
    MyStorage(int initialCapacity = 10);
    //MyStorage();
    ~MyStorage();

    void setObject(int index, CCircle* circle);
    CCircle* getObject(int index) const;
    int getCount() const;
    void remove(int index);
    void add(CCircle* circle);
    void removeSelected();
    void selectAt(int x, int y);
    void deselectAll();
};

#endif
