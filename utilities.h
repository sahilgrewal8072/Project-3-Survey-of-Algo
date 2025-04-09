#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <string>
#include "HashTable.h"

namespace HashTableUtils {
    std::vector<std::string> generateUniqueStrings(size_t count, size_t length = 8);
    void splitData(const std::vector<std::string>& wholeList, 
                   std::vector<std::string>& addValues, 
                   std::vector<std::string>& checkValues);
    
    // Timing functions for Part 2.2
    void timeNoRehash(const std::vector<std::string>& addValues);
    void timeRehashDoubling(const std::vector<std::string>& addValues);
    void timeRehashAdd10000(const std::vector<std::string>& addValues);
    
    // Timing functions for Part 2.3
    void timeLoadFactorPerformance(size_t initialSize, 
                                  const std::vector<std::string>& addValues,
                                  const std::vector<std::string>& checkValues);
}

#endif // UTILITIES_H