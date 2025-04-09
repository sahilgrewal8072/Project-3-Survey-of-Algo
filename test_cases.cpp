#include "../include/HashTable.h"
#include <cassert>
#include <iostream>
#include <unordered_set>

void testBasicOperations() {
    HashTable ht;
    assert(ht.insert("apple"));
    assert(ht.contains("apple"));
    assert(!ht.contains("banana"));
    assert(!ht.insert("apple")); // Duplicate
    
    const std::vector<std::string> fruits = {"banana", "cherry", "date"};
    for (const auto& fruit : fruits) {
        assert(ht.insert(fruit));
    }
    for (const auto& fruit : fruits) {
        assert(ht.contains(fruit));
    }
    std::cout << "✓ Basic operations passed\n";
}

void testResizing() {
    HashTable htDouble(16, 0.75f, HashTable::ResizeStrategy::Double);
    for (int i = 0; i < 12; ++i) { // 16 * 0.75 = 12
        assert(htDouble.insert(std::to_string(i)));
    }
    assert(htDouble.capacity() == 32);
    
    HashTable htAdd(16, 0.75f, HashTable::ResizeStrategy::Add10000);
    for (int i = 0; i < 12; ++i) {
        assert(htAdd.insert(std::to_string(i)));
    }
    assert(htAdd.capacity() == 10016);
    std::cout << "✓ Resizing tests passed\n";
}

void testLargeDataset() {
    HashTable ht;
    std::unordered_set<std::string> reference;
    for (int i = 0; i < 1000; ++i) {
        std::string val = "item_" + std::to_string(i);
        assert(ht.insert(val));
        reference.insert(val);
    }
    for (const auto& val : reference) {
        assert(ht.contains(val));
    }
    std::cout << "✓ Large dataset test passed\n";
}

void runAllTests() {
    testBasicOperations();
    testResizing();
    testLargeDataset();
}