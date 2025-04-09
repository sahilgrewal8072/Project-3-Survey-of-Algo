#include "HashTable.h"
#include <cassert>
#include <iostream>

void testBasicOperations() {
    HashTable ht;
    
    // Test insert and find
    assert(ht.insert("apple"));
    assert(ht.find("apple"));
    assert(!ht.find("banana"));
    
    // Test duplicate insert
    assert(!ht.insert("apple"));
    
    // Test multiple inserts
    assert(ht.insert("banana"));
    assert(ht.insert("cherry"));
    assert(ht.insert("date"));
    assert(ht.insert("elderberry"));
    
    assert(ht.find("banana"));
    assert(ht.find("cherry"));
    assert(ht.find("date"));
    assert(ht.find("elderberry"));
    
    // Test non-existent items
    assert(!ht.find("fig"));
    assert(!ht.find("grape"));
    
    std::cout << "Basic operations test passed!\n";
}

void testResizingDouble() {
    HashTable ht(16, 0.75f, HashTable::DOUBLE);
    
    // Add 11 items (16 * 0.75 = 12, so no resize yet)
    for (int i = 0; i < 11; ++i) {
        assert(ht.insert(std::to_string(i)));
    }
    assert(ht.tableSize() == 16);
    assert(ht.getResizeCount() == 0);
    
    // Add 12th item - should trigger resize
    assert(ht.insert("12"));
    assert(ht.tableSize() == 32);
    assert(ht.getResizeCount() == 1);
    
    // Add items up to 23 (32 * 0.75 = 24)
    for (int i = 13; i < 23; ++i) {
        assert(ht.insert(std::to_string(i)));
    }
    assert(ht.tableSize() == 32);
    assert(ht.getResizeCount() == 1);
    
    // Add 24th item - should trigger second resize
    assert(ht.insert("24"));
    assert(ht.tableSize() == 64);
    assert(ht.getResizeCount() == 2);
    
    // Verify all items can be found
    for (int i = 0; i <= 24; ++i) {
        assert(ht.find(std::to_string(i)));
    }
    
    std::cout << "Resizing with doubling strategy test passed!\n";
}

void testResizingAdd10000() {
    HashTable ht(16, 0.75f, HashTable::ADD_10000);
    
    // Add 11 items (16 * 0.75 = 12, so no resize yet)
    for (int i = 0; i < 11; ++i) {
        assert(ht.insert(std::to_string(i)));
    }
    assert(ht.tableSize() == 16);
    assert(ht.getResizeCount() == 0);
    
    // Add 12th item - should trigger resize
    assert(ht.insert("12"));
    assert(ht.tableSize() == 16 + 10000);
    assert(ht.getResizeCount() == 1);
    
    std::cout << "Resizing with add 10000 strategy test passed!\n";
}

void runAllTests() {
    testBasicOperations();
    testResizingDouble();
    testResizingAdd10000();
}