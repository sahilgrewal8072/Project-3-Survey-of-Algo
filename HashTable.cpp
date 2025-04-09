#include "HashTable.h"
#include <functional>

HashTable::HashTable(size_t initialSize, float maxLoadFactor, ResizeStrategy strategy)
    : table(initialSize, ""), itemCount(0), maxLoadFactor(maxLoadFactor), 
      resizeStrategy(strategy), resizeCount(0) {}

bool HashTable::insert(const std::string& value) {
    if (find(value)) {
        return true; // Already exists
    }

    if (loadFactor() >= maxLoadFactor) {
        rehash();
    }

    size_t pos = hashFunction(value) % table.size();
    while (!table[pos].empty() && table[pos] != value) {
        pos = (pos + 1) % table.size();
    }

    if (table[pos].empty()) {
        table[pos] = value;
        itemCount++;
        return true;
    }

    return false;
}

bool HashTable::find(const std::string& value) const {
    size_t pos = findPosition(value);
    return !table[pos].empty() && table[pos] == value;
}

size_t HashTable::findPosition(const std::string& value) const {
    size_t pos = hashFunction(value) % table.size();
    while (!table[pos].empty() && table[pos] != value) {
        pos = (pos + 1) % table.size();
    }
    return pos;
}

size_t HashTable::size() const {
    return itemCount;
}

size_t HashTable::tableSize() const {
    return table.size();
}

float HashTable::loadFactor() const {
    return static_cast<float>(itemCount) / table.size();
}

size_t HashTable::getResizeCount() const {
    return resizeCount;
}

size_t HashTable::hashFunction(const std::string& value) const {
    return std::hash<std::string>{}(value);
}

void HashTable::rehash() {
    size_t newSize;
    if (resizeStrategy == DOUBLE) {
        newSize = table.size() * 2;
    } else { // ADD_10000
        newSize = table.size() + 10000;
    }

    std::vector<std::string> oldTable = std::move(table);
    table = std::vector<std::string>(newSize, "");
    itemCount = 0;

    for (const auto& value : oldTable) {
        if (!value.empty()) {
            insert(value);
        }
    }

    resizeCount++;
}