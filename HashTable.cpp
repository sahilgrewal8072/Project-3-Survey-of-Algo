#include "HashTable.h"
#include <cmath>

HashTable::HashTable(size_t initialSize, float maxLoadFactor, ResizeStrategy strategy)
    : table(initialSize), maxLoadFactor(maxLoadFactor), 
      resizeStrategy(strategy) {}

bool HashTable::insert(const std::string& value) {
    if (contains(value)) return false;
    
    if (loadFactor() >= maxLoadFactor) {
        rehash();
    }

    size_t pos = probe(value);
    if (!table[pos]) {
        table[pos] = value;
        ++itemCount;
        return true;
    }
    return false;
}

bool HashTable::contains(const std::string& value) const {
    size_t pos = probe(value);
    return table[pos] && table[pos].value() == value;
}

size_t HashTable::probe(const std::string& value) const {
    size_t hash = std::hash<std::string>{}(value);
    size_t pos = hash % table.size();
    
    while (table[pos] && table[pos].value() != value) {
        pos = (pos + 1) % table.size();
    }
    return pos;
}

void HashTable::rehash() {
    size_t newSize = (resizeStrategy == ResizeStrategy::Double) ? 
                    table.size() * 2 : table.size() + 10000;

    auto oldTable = std::move(table);
    table = std::vector<std::optional<std::string>>(newSize);
    itemCount = 0;

    for (const auto& entry : oldTable) {
        if (entry) {
            insert(entry.value());
        }
    }

    ++resizeCount;
}