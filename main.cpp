#include <iostream>
#include "HashTable.h"
#include "utilities.h"

void runAllTests();

int main() {
    try {
        runAllTests();
        
        std::cout << "Starting performance measurements...\n";
        auto data = HashTableUtils::generateTestData();
        HashTableUtils::measurePerformance(data);
        HashTableUtils::analyzeLoadFactorPerformance(65536, data);
        
        std::cout << "\nAll operations completed successfully!\n";
        std::cout << "1. View timing data in data/timing_results.csv\n";
        std::cout << "2. View load factor data in data/load_factor_results.csv\n";
        std::cout << "3. Run 'python scripts/graph_generator.py' to generate plots\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}