#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <vector>
#include <algorithm>
#include <stdexcept>

template<typename T>
class Repository {
private:
    std::vector<T> items;

public:
    // Add an item to the repository
    void add(const T& item) {
        items.push_back(item);
    }

    // Remove an item by ID
    bool remove(int id) {
        auto it = std::find_if(items.begin(), items.end(),
            [id](const T& item) { return item.getId() == id; });
        if (it != items.end()) {
            items.erase(it);
            return true;
        }
        return false; // Item not found
    }

    // Get an item by ID (returns pointer, nullptr if not found)
    T* getById(int id) {
        auto it = std::find_if(items.begin(), items.end(),
            [id](const T& item) { return item.getId() == id; });
        if (it != items.end()) {
            return &(*it);
        }
        return nullptr; // Item not found
    }

    // Get all items
    std::vector<T> getAll() const {
        return items;
    }

    // Get the size of the repository
    int size() const {
        return items.size();
    }
};

#endif