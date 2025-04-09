#include "../include/HashTable.h"
#include <iostream>

void testBasicOperations() {
    std::cout << "Running basic operations test... ";
    HashTable ht;
    bool allPassed = true;
    
    // Test insert and contains
    allPassed &= ht.insert("apple");
    allPassed &= ht.contains("apple");
    allPassed &= !ht.contains("banana");
    allPassed &= !ht.insert("apple"); // Duplicate
    
    const std::vector<std::string> fruits = {"banana", "cherry", "date"};
    for (const auto& fruit : fruits) {
        allPassed &= ht.insert(fruit);
    }
    for (const auto& fruit : fruits) {
        allPassed &= ht.contains(fruit);
    }
    
    std::cout << (allPassed ? "PASSED" : "FAILED") << "\n";
}

void testResizing() {
    std::cout << "Running resizing test... ";
    bool allPassed = true;
    
    HashTable htDouble(16, 0.75f, HashTable::ResizeStrategy::Double);
    for (int i = 0; i < 12; ++i) {
        allPassed &= htDouble.insert(std::to_string(i));
    }
    allPassed &= (htDouble.capacity() == 32);
    
    HashTable htAdd(16, 0.75f, HashTable::ResizeStrategy::Add10000);
    for (int i = 0; i < 12; ++i) {
        allPassed &= htAdd.insert(std::to_string(i));
    }
    allPassed &= (htAdd.capacity() == 10016);
    
    std::cout << (allPassed ? "PASSED" : "FAILED") << "\n";
}

void testLargeDataset() {
    std::cout << "Running large dataset test... ";
    bool allPassed = true;
    
    HashTable ht;
    for (int i = 0; i < 1000; ++i) {
        allPassed &= ht.insert("item_" + std::to_string(i));
    }
    for (int i = 0; i < 1000; ++i) {
        allPassed &= ht.contains("item_" + std::to_string(i));
    }
    for (int i = 1000; i < 1100; ++i) {
        allPassed &= !ht.contains("item_" + std::to_string(i));
    }
    
    std::cout << (allPassed ? "PASSED" : "FAILED") << "\n";
}

void runAllTests() {
    std::cout << "\n=== Running Tests ===\n";
    testBasicOperations();
    testResizing();
    testLargeDataset();
    std::cout << "=== Tests Complete ===\n\n";
}