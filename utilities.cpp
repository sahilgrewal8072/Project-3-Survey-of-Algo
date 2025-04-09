#include "utilities.h"
#include "HashTable.h"
#include <random>
#include <algorithm>
#include <fstream>
#include <iostream>

namespace HashTableUtils {

TestData generateTestData(size_t totalSize, size_t strLength) {
    std::cout << "Generating test data...\n";
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 9);
    
    std::unordered_set<std::string> uniqueStrings;
    while (uniqueStrings.size() < totalSize) {
        std::string num;
        for (size_t i = 0; i < strLength; ++i) {
            num += std::to_string(dist(gen));
        }
        uniqueStrings.insert(num);
    }

    TestData data;
    data.addValues.assign(uniqueStrings.begin(), uniqueStrings.end());
    std::shuffle(data.addValues.begin(), data.addValues.end(), gen);
    
    size_t half = data.addValues.size() / 2;
    data.checkValues.assign(data.addValues.begin() + half, data.addValues.end());
    data.addValues.resize(half);
    
    std::cout << "Generated " << data.addValues.size() 
              << " add values and " << data.checkValues.size()
              << " check values\n";
    return data;
}

void measurePerformance(const TestData& data) {
    std::cout << "\nMeasuring insertion performance...\n";
    std::ofstream out("data/timing_results.csv");
    out << "q,NoRehash,Doubling,Add10000\n";
    
    for (int q = 4; q <= 17; ++q) {
        std::cout << "Processing q=" << q << "... ";
        size_t numToInsert = static_cast<size_t>((3.0/4) * std::pow(2, q) - 1);
        
        // No rehash
        auto start = std::chrono::high_resolution_clock::now();
        HashTable ht(1 << 17, 1.0f);
        for (size_t i = 0; i < numToInsert; ++i) {
            ht.insert(data.addValues[i]);
        }
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - start).count();
        double noRehash = static_cast<double>(duration) / numToInsert;
        
        // Rehash with doubling
        start = std::chrono::high_resolution_clock::now();
        HashTable ht2(1 << q, 0.75f, HashTable::ResizeStrategy::Double);
        for (size_t i = 0; i < numToInsert; ++i) {
            ht2.insert(data.addValues[i]);
        }
        duration = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - start).count();
        double doubling = static_cast<double>(duration) / numToInsert;
        
        // Rehash with add 10000
        start = std::chrono::high_resolution_clock::now();
        HashTable ht3(1 << q, 0.75f, HashTable::ResizeStrategy::Add10000);
        for (size_t i = 0; i < numToInsert; ++i) {
            ht3.insert(data.addValues[i]);
        }
        duration = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - start).count();
        double add10000 = static_cast<double>(duration) / numToInsert;
        
        out << q << "," << noRehash << "," << doubling << "," << add10000 << "\n";
        std::cout << "done\n";
    }
    std::cout << "Timing data saved to data/timing_results.csv\n";
}

void analyzeLoadFactorPerformance(size_t initialSize, const TestData& data) {
    std::cout << "\nAnalyzing load factor performance...\n";
    std::ofstream out("data/load_factor_results.csv");
    out << "LoadFactor,SuccessfulSearch,UnsuccessfulSearch\n";
    
    HashTable ht(initialSize, 1.0f);
    const size_t sampleSize = 1000;
    
    for (float targetLF = 0.05f; targetLF < 1.0f; targetLF += 0.05f) {
        std::cout << "Testing load factor " << targetLF << "... ";
        
        // Fill to target load factor
        while (ht.loadFactor() < targetLF && ht.size() < data.addValues.size()) {
            ht.insert(data.addValues[ht.size()]);
        }
        
        // Time successful searches
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < sampleSize && i < ht.size(); ++i) {
            ht.contains(data.addValues[i]);
        }
        auto succTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now() - start).count() / sampleSize;
        
        // Time unsuccessful searches
        start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < sampleSize && i < data.checkValues.size(); ++i) {
            ht.contains(data.checkValues[i]);
        }
        auto failTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now() - start).count() / sampleSize;
        
        out << ht.loadFactor() << "," << succTime << "," << failTime << "\n";
        std::cout << "done\n";
    }
    std::cout << "Load factor data saved to data/load_factor_results.csv\n";
}
}