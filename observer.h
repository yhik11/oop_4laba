#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>
#include <string>
#include <algorithm>

class Observer {
public:
    virtual ~Observer() {}
    virtual void update(const std::string& message) = 0;
    virtual void onObjectAdded(int index) = 0;
    virtual void onObjectRemoved(int index) = 0;
    virtual void onObjectChanged(int index) = 0;
    virtual void onSelectionChanged() = 0;
    virtual void onStorageCleared() = 0;
};

class Subject {
private:
    std::vector<Observer*> observers;

public:
    virtual ~Subject() {}

    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }

    void removeObserver(Observer* observer) {
        auto it = std::find(observers.begin(), observers.end(), observer);
        if (it != observers.end()) {
            observers.erase(it);
        }
    }

    void notifyObjectChanged(int index) {
        for (auto obs : observers) {
            obs->onObjectChanged(index);
        }
    }

    void notifySelectionChanged() {
        for (auto obs : observers) {
            obs->onSelectionChanged();
        }
    }


    void notifyUpdate(const std::string& message = "") {
        for (auto obs : observers) {
            obs->update(message);
        }
    }

    void notifyObjectAdded(int index) {
        for (auto obs : observers) {
            obs->onObjectAdded(index);
        }
    }

    void notifyObjectRemoved(int index) {
        for (auto obs : observers) {
            obs->onObjectRemoved(index);
        }
    }





    void notifyStorageCleared() {
        for (auto obs : observers) {
            obs->onStorageCleared();
        }
    }
};

#endif
