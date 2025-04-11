#include <iostream>
#include <vector>
#include <string>
#include <functional>

/*
 * HashTable Implementation with Linear Probing
 * Features:
 * - Stores strings only
 * - Linear probing collision resolution
 * - Configurable resize strategies:
 *   1: Double the size
 *   2: Add 10000 slots
 * - Automatic resizing when load factor exceeds threshold
 */
class HashTable {
private:
    std::vector<std::string> table;  // Storage for strings
    size_t capacity;                 // Current table size
    size_t itemCount;                // Number of items stored
    float maxLoadFactor;             // Threshold for resizing
    int resizeStrategy;              // 1=double, 2=add 10000

    // Hash function using std::hash (now const)
    size_t hashFunction(const std::string& str) const {
        return std::hash<std::string>{}(str);
    }

    // Get initial index using modulo operation (now const)
    size_t getIndex(const std::string& str) const {
        return hashFunction(str) % capacity;
    }

    // Resize the table and rehash all elements
    void resizeTable() {
        size_t newCapacity = (resizeStrategy == 1) ? capacity * 2 : capacity + 10000;
        std::vector<std::string> oldTable = table;
        
        // Reset table with new capacity
        table.clear();
        table.resize(newCapacity);
        capacity = newCapacity;
        itemCount = 0;  // Will be rebuilt during rehashing
        
        // Rehash all existing elements
        for (const auto& str : oldTable) {
            if (!str.empty()) {
                insert(str);  // Re-insert using new capacity
            }
        }
    }

public:
    // Constructor with configurable parameters
    HashTable(size_t initialSize = 16, float maxLoad = 0.75f, int strategy = 1)
        : capacity(initialSize), itemCount(0), maxLoadFactor(maxLoad), resizeStrategy(strategy) {
        table.resize(capacity);
    }

    // Insert a string into the table
    void insert(const std::string& str) {
        // Check if resize needed before inserting
        if ((float)(itemCount + 1) / capacity > maxLoadFactor) {
            resizeTable();
        }

        size_t index = getIndex(str);
        size_t startIndex = index;
        
        // Linear probing loop
        do {
            if (table[index].empty()) {  // Found empty slot
                table[index] = str;
                itemCount++;
                return;
            }
            if (table[index] == str) {  // Already exists
                return;
            }
            index = (index + 1) % capacity;  // Move to next slot
        } while (index != startIndex);       // Wrap around if needed
    }

    // Check if string exists in table
    bool find(const std::string& str) const {
        size_t index = getIndex(str);
        size_t startIndex = index;
        
        do {
            if (table[index].empty()) {
                return false;  // Found empty slot - not here
            }
            if (table[index] == str) {
                return true;  // Found the string
            }
            index = (index + 1) % capacity;  // Linear probing
        } while (index != startIndex);       // Checked all slots
        
        return false;  // Not found after full cycle
    }

    // Getters for testing
    size_t getCapacity() const { return capacity; }
    size_t getItemCount() const { return itemCount; }
};

/*
 * Test Cases for HashTable Implementation
 */
void runTests() {
    std::cout << "=== HashTable Test Results ===\n";
    
    // Test 1: Basic insert/find operations
    {
        HashTable ht;
        std::vector<std::string> items = {"apple", "banana", "cherry", "date", "elderberry"};
        std::vector<std::string> notInTable = {"fig", "grape", "kiwi", "lemon", "mango"};
        
        for (const auto& item : items) {
            ht.insert(item);
        }
        
        bool allFound = true;
        for (const auto& item : items) {
            if (!ht.find(item)) {
                allFound = false;
                break;
            }
        }
        
        bool noneFound = true;
        for (const auto& item : notInTable) {
            if (ht.find(item)) {
                noneFound = false;
                break;
            }
        }
        
        std::cout << "Test 1 - Basic operations: " 
                  << (allFound && noneFound ? "PASSED" : "FAILED") << "\n";
    }

    // Test 2: Resize with doubling strategy (23 items)
    {
        HashTable ht(16, 0.75f, 1);
        for (int i = 0; i < 23; i++) {
            ht.insert("item" + std::to_string(i));
        }
        
        bool passed = ht.getCapacity() == 32;  // Should have doubled once (16->32)
        std::cout << "Test 2 - Doubling strategy (23 items): "
                  << (passed ? "PASSED" : "FAILED") 
                  << " (Size: " << ht.getCapacity() << ")\n";
    }

    // Test 3: Second resize with doubling (24 items)
    {
        HashTable ht(16, 0.75f, 1);
        for (int i = 0; i < 24; i++) {
            ht.insert("item" + std::to_string(i));
        }
        
        bool passed = ht.getCapacity() == 64;  // Should have doubled twice (16->32->64)
        std::cout << "Test 3 - Second doubling (24 items): "
                  << (passed ? "PASSED" : "FAILED") 
                  << " (Size: " << ht.getCapacity() << ")\n";
    }

    // Test 4: Addition strategy (23 items)
    {
        HashTable ht(16, 0.75f, 2);
        for (int i = 0; i < 23; i++) {
            ht.insert("item" + std::to_string(i));
        }
        
        bool passed = ht.getCapacity() == 10016;  // 16 + 10000
        std::cout << "Test 4 - Addition strategy (23 items): "
                  << (passed ? "PASSED" : "FAILED") 
                  << " (Size: " << ht.getCapacity() << ")\n";
    }
}

int main() {
    runTests();
    return 0;
}
