#include "utilities.h"
#include <random>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <cmath>

namespace HashTableUtils {

std::vector<std::string> generateUniqueStrings(size_t count, size_t length) {
    std::vector<std::string> result;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 9);

    while (result.size() < count) {
        std::string num;
        for (size_t i = 0; i < length; ++i) {
            num += std::to_string(dist(gen));
        }
        
        if (std::find(result.begin(), result.end(), num) == result.end()) {
            result.push_back(num);
        }
    }

    return result;
}

void splitData(const std::vector<std::string>& wholeList, 
               std::vector<std::string>& addValues, 
               std::vector<std::string>& checkValues) {
    size_t half = wholeList.size() / 2;
    addValues = std::vector<std::string>(wholeList.begin(), wholeList.begin() + half);
    checkValues = std::vector<std::string>(wholeList.begin() + half, wholeList.end());
}

void timeNoRehash(const std::vector<std::string>& addValues) {
    std::ofstream outFile("data/timing_results.txt");
    outFile << "q,NoRehashTime,RehashDoubleTime,RehashAdd10000Time\n";
    
    for (int q = 4; q <= 17; ++q) {
        size_t numToInsert = static_cast<size_t>((3.0/4) * pow(2, q) - 1);
        size_t tableSize = pow(2, 17);
        
        auto start = std::chrono::high_resolution_clock::now();
        HashTable ht(tableSize, 0.75f, HashTable::DOUBLE);
        
        for (size_t i = 0; i < numToInsert; ++i) {
            ht.insert(addValues[i]);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        double normalizedTime = static_cast<double>(duration) / numToInsert;
        
        outFile << q << "," << normalizedTime << "\n";
    }
}

void timeRehashDoubling(const std::vector<std::string>& addValues) {
    std::ofstream outFile("data/timing_results.txt", std::ios::app);
    
    for (int q = 4; q <= 17; ++q) {
        size_t numToInsert = static_cast<size_t>((3.0/4) * pow(2, q) - 1);
        size_t tableSize = pow(2, q);
        
        auto start = std::chrono::high_resolution_clock::now();
        HashTable ht(tableSize, 0.75f, HashTable::DOUBLE);
        
        for (size_t i = 0; i < numToInsert; ++i) {
            ht.insert(addValues[i]);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        double normalizedTime = static_cast<double>(duration) / numToInsert;
        
        outFile << q << ",," << normalizedTime << "\n";
    }
}

void timeRehashAdd10000(const std::vector<std::string>& addValues) {
    std::ofstream outFile("data/timing_results.txt", std::ios::app);
    
    for (int q = 4; q <= 17; ++q) {
        size_t numToInsert = static_cast<size_t>((3.0/4) * pow(2, q) - 1);
        size_t tableSize = pow(2, q);
        
        auto start = std::chrono::high_resolution_clock::now();
        HashTable ht(tableSize, 0.75f, HashTable::ADD_10000);
        
        for (size_t i = 0; i < numToInsert; ++i) {
            ht.insert(addValues[i]);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        double normalizedTime = static_cast<double>(duration) / numToInsert;
        
        outFile << q << ",,," << normalizedTime << "\n";
    }
}

void timeLoadFactorPerformance(size_t initialSize, 
                              const std::vector<std::string>& addValues,
                              const std::vector<std::string>& checkValues) {
    std::ofstream outFile("data/load_factor_results.txt");
    outFile << "LoadFactor,SuccessfulSearchTime,UnsuccessfulSearchTime\n";
    
    HashTable ht(initialSize, 1.0f, HashTable::DOUBLE);
    size_t totalValues = addValues.size();
    
    for (float targetLF = 0.05f; targetLF < 1.0f; targetLF += 0.05f) {
        size_t targetItems = static_cast<size_t>(targetLF * initialSize);
        
        // Fill table to target load factor
        while (ht.size() < targetItems && ht.size() < totalValues) {
            ht.insert(addValues[ht.size()]);
        }
        
        float actualLF = ht.loadFactor();
        
        // Time successful searches
        size_t K = std::min(static_cast<size_t>(1000), ht.size());
        auto start = std::chrono::high_resolution_clock::now();
        
        for (size_t i = 0; i < K; ++i) {
            ht.find(addValues[i]);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto succDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        double avgSuccTime = static_cast<double>(succDuration) / K;
        
        // Time unsuccessful searches
        start = std::chrono::high_resolution_clock::now();
        
        for (size_t i = 0; i < K; ++i) {
            ht.find(checkValues[i]);
        }
        
        end = std::chrono::high_resolution_clock::now();
        auto failDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        double avgFailTime = static_cast<double>(failDuration) / K;
        
        outFile << actualLF << "," << avgSuccTime << "," << avgFailTime << "\n";
    }
}
}