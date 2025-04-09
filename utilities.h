#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <string>
#include <chrono>

namespace HashTableUtils {
    struct TestData {
        std::vector<std::string> addValues;
        std::vector<std::string> checkValues;
    };

    TestData generateTestData(size_t totalSize = 131072, size_t strLength = 8);
    void measurePerformance(const TestData& data);
    void analyzeLoadFactorPerformance(size_t initialSize, const TestData& data);
}

#endif