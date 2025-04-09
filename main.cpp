#include <iostream>
#include "HashTable.h"
#include "utilities.h"
#include "test_cases.h"

int main() {
    // Run correctness tests
    runAllTests();
    
    // Part 2.1: Generate data set
    std::vector<std::string> wholeList = HashTableUtils::generateUniqueStrings(131072);
    std::vector<std::string> addValues, checkValues;
    HashTableUtils::splitData(wholeList, addValues, checkValues);
    
    // Part 2.2: Timing rehash cost
    HashTableUtils::timeNoRehash(addValues);
    HashTableUtils::timeRehashDoubling(addValues);
    HashTableUtils::timeRehashAdd10000(addValues);
    
    // Part 2.3: Timing vs load factor
    HashTableUtils::timeLoadFactorPerformance(65536, addValues, checkValues);
    
    std::cout << "All operations completed. Check data/ folder for results.\n";
    return 0;
}