#include <vector>
#include <string>
#include <functional>

class HashTable {
private:
    std::vector<std::string> table;
    int currentSize;
    int numElements;
    double maxLoadFactor;
    int resizeStrategy;
    int resizeCount;

public:
    HashTable(int initialSize, double maxLoad, int strategy)
        : currentSize(initialSize), maxLoadFactor(maxLoad), resizeStrategy(strategy), numElements(0), resizeCount(0) {
        table.resize(initialSize, "");
    }

    void insert(const std::string& s) {
        if (find(s)) {
            return;
        }

        int index = hashFunction(s);
        int startIndex = index;
        do {
            if (table[index].empty()) {
                table[index] = s;
                numElements++;
                if (static_cast<double>(numElements) / currentSize >= maxLoadFactor) {
                    resize();
                }
                return;
            } else if (table[index] == s) {
                return;
            }
            index = (index + 1) % currentSize;
        } while (index != startIndex);

        resize();
        insert(s);
    }

    bool find(const std::string& s) const {
        int index = hashFunction(s);
        int startIndex = index;
        do {
            if (table[index].empty()) {
                return false;
            } else if (table[index] == s) {
                return true;
            }
            index = (index + 1) % currentSize;
        } while (index != startIndex);
        return false;
    }

    int getResizeCount() const {
        return resizeCount;
    }

private:
    int hashFunction(const std::string& s) const {
        std::hash<std::string> hasher;
        return hasher(s) % currentSize;
    }

    void resize() {
        int oldSize = currentSize;
        std::vector<std::string> oldTable = table;

        if (resizeStrategy == 1) {
            currentSize *= 2;
        } else {
            currentSize += 10000;
        }

        table.clear();
        table.resize(currentSize, "");
        numElements = 0;
        resizeCount++;

        for (int i = 0; i < oldSize; ++i) {
            if (!oldTable[i].empty()) {
                int index = hashFunction(oldTable[i]);
                int startIndex = index;
                do {
                    if (table[index].empty()) {
                        table[index] = oldTable[i];
                        numElements++;
                        break;
                    }
                    index = (index + 1) % currentSize;
                } while (index != startIndex);
            }
        }
    }
};