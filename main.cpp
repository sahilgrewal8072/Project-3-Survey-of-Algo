#include <iostream>
#include "HashTable.h"
#include "utilities.h"

void runAllTests();

int main() {
    runAllTests();
    
    auto data = HashTableUtils::generateTestData();
    HashTableUtils::measurePerformance(data);
    HashTableUtils::analyzeLoadFactorPerformance(65536, data);
    
    std::cout << "Performance data generated in data/ directory\n";
    return 0;
}