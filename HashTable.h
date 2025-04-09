#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
#include <optional>
#include <functional>

class HashTable {
public:
    enum class ResizeStrategy { Double, Add10000 };

    HashTable(size_t initialSize = 16, float maxLoadFactor = 0.75f, 
              ResizeStrategy strategy = ResizeStrategy::Double);
    
    bool insert(const std::string& value);
    bool contains(const std::string& value) const;
    size_t size() const noexcept { return itemCount; }
    size_t capacity() const noexcept { return table.size(); }
    float loadFactor() const noexcept { return static_cast<float>(itemCount) / capacity(); }
    size_t getResizeCount() const noexcept { return resizeCount; }

private:
    std::vector<std::optional<std::string>> table;
    size_t itemCount = 0;
    float maxLoadFactor;
    ResizeStrategy resizeStrategy;
    size_t resizeCount = 0;

    size_t probe(const std::string& value) const;
    void rehash();
};

#endif