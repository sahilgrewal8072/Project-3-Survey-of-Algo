#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>

class HashTable {
public:
    enum ResizeStrategy { DOUBLE, ADD_10000 };

    HashTable(size_t initialSize = 16, float maxLoadFactor = 0.75f, 
              ResizeStrategy strategy = DOUBLE);
    
    bool insert(const std::string& value);
    bool find(const std::string& value) const;
    size_t size() const;
    size_t tableSize() const;
    float loadFactor() const;
    size_t getResizeCount() const;

private:
    std::vector<std::string> table;
    size_t itemCount;
    float maxLoadFactor;
    ResizeStrategy resizeStrategy;
    size_t resizeCount;

    size_t hashFunction(const std::string& value) const;
    void rehash();
    size_t findPosition(const std::string& value) const;
};

#endif // HASHTABLE_H