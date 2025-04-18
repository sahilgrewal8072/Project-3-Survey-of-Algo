#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <chrono>
#include <fstream>
#include <unordered_set>
#include <random>

/*
 * Part 1.1: HashTable Class Implementation
 * - Custom hash table for string storage using linear probing
 * - Supports dynamic resizing with two strategies:
 *   Strategy 1: Doubling table size
 *   Strategy 2: Adding 10000 to table size
 */
class HashTable {
private:
    std::vector<std::string> table;
    size_t size;
    size_t count;
    float maxLoadFactor;
    int resizeStrategy;

    size_t hash(const std::string& str) {
        return std::hash<std::string>{}(str);
    }

    size_t getIndex(const std::string& str) {
        return hash(str) % size;
    }

    void resize() {
        size_t oldSize = size;
        if (resizeStrategy == 1) size *= 2;
        else size += 10000;

        std::vector<std::string> oldTable = table;
        table.clear();
        table.resize(size);
        count = 0;

        for (const auto& str : oldTable) {
            if (!str.empty()) insert(str);
        }
    }

public:
    HashTable(size_t initialSize = 16, float maxLoad = 0.75f, int strategy = 1)
        : size(initialSize), count(0), maxLoadFactor(maxLoad), resizeStrategy(strategy) {
        table.resize(size);
    }

    void insert(const std::string& str) {
        if (find(str)) return;

        if ((float)(count + 1) / size > maxLoadFactor) resize();

        size_t index = getIndex(str);
        while (!table[index].empty()) {
            index = (index + 1) % size;
        }

        table[index] = str;
        count++;
    }

    bool find(const std::string& str) {
        size_t index = getIndex(str);
        size_t start = index;

        while (!table[index].empty()) {
            if (table[index] == str) return true;
            index = (index + 1) % size;
            if (index == start) return false;
        }

        return false;
    }

    size_t getSize() const { return size; }
    size_t getCount() const { return count; }
};

/*
 * Part 1.2: Test Cases
 * - Check correctness for insertion, find, and resize behaviors
 */
void runTests() {
    std::cout << "\n=== Running Part 1.2 Tests ===\n";

    // 1.2.1: Insert 5 elements, check successful and failed searches
    {
        HashTable ht;
        std::vector<std::string> present = {"a", "b", "c", "d", "e"};
        std::vector<std::string> absent = {"x", "y", "z", "w", "v"};

        for (auto& str : present) ht.insert(str);
        bool pass = true;
        for (auto& str : present) pass &= ht.find(str);
        for (auto& str : absent) pass &= !ht.find(str);

        std::cout << "Test 1.2.1 - Basic insert/find: " << (pass ? "PASSED" : "FAILED") << "\n";
    }

    // 1.2.2: Resize once (23 inserts, doubling strategy)
    {
        HashTable ht(16, 0.75f, 1);
        for (int i = 0; i < 23; i++) ht.insert("val" + std::to_string(i));
        std::cout << "Test 1.2.2 - Single resize (Expected Size: 32): "
                  << (ht.getSize() == 32 ? "PASSED" : "FAILED") << "\n";
    }

    // 1.2.3: Resize twice (24 inserts, doubling strategy)
    {
        HashTable ht(16, 0.75f, 1);
        for (int i = 0; i < 24; i++) ht.insert("val" + std::to_string(i));
        std::cout << "Test 1.2.3 - Double resize (Expected Size: 64): "
                  << (ht.getSize() == 64 ? "PASSED" : "FAILED") << "\n";
    }

    // 1.2.4: Resize with addition strategy
    {
        HashTable ht(16, 0.75f, 2);
        for (int i = 0; i < 23; i++) ht.insert("val" + std::to_string(i));
        std::cout << "Test 1.2.4 - Resize with addition (Expected Size: 10016): "
                  << (ht.getSize() == 10016 ? "PASSED" : "FAILED") << "\n";
    }
}

/*
 * Part 2.1: Generate Unique 8-digit Strings
 */
std::vector<std::string> generateUniqueStrings(int count) {
    std::unordered_set<std::string> set;
    std::vector<std::string> result;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10000000, 99999999);

    while ((int)set.size() < count) {
        std::string val = std::to_string(dis(gen));
        if (set.insert(val).second) result.push_back(val);
    }
    return result;
}

/*
 * Part 2.2: Measure Rehash Cost vs q
 */
void timeRehashCost() {
    std::ofstream file("rehash_times.csv");
    file << "q,no_rehash,doubling,addition\n";

    std::vector<std::string> values = generateUniqueStrings(131072);

    for (int q = 4; q <= 17; q++) {
        int num = (1 << q) * 0.75 - 1;

        // No rehash
        auto start = std::chrono::high_resolution_clock::now();
        HashTable noRehash(1 << 17, 0.75f, 1);
        for (int i = 0; i < num; ++i) noRehash.insert(values[i]);
        auto t1 = std::chrono::high_resolution_clock::now();

        // Doubling
        start = std::chrono::high_resolution_clock::now();
        HashTable doubling(1 << q, 0.75f, 1);
        for (int i = 0; i < num; ++i) doubling.insert(values[i]);
        auto t2 = std::chrono::high_resolution_clock::now();

        // Addition
        start = std::chrono::high_resolution_clock::now();
        HashTable addition(1 << q, 0.75f, 2);
        for (int i = 0; i < num; ++i) addition.insert(values[i]);
        auto t3 = std::chrono::high_resolution_clock::now();

        file << q << ","
             << std::chrono::duration_cast<std::chrono::microseconds>(t1 - start).count() / num << ","
             << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / num << ","
             << std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count() / num << "\n";
    }
}

/*
 * Part 2.3: Load Factor Timing for Successful & Failed Searches
 */
void timeLoadFactor() {
    std::vector<std::string> whole = generateUniqueStrings(131072);
    std::vector<std::string> addValues(whole.begin(), whole.begin() + 65536);
    std::vector<std::string> checkValues(whole.begin() + 65536, whole.end());

    std::ofstream file("load_factor_times.csv");
    file << "load_factor,success_time,fail_time\n";

    HashTable ht(65536, 1.0f, 1);

    for (double lf = 0.1; lf < 1.0; lf += 0.03) {
        int targetCount = static_cast<int>(lf * 65536);

        while ((int)ht.getCount() < targetCount) {
            ht.insert(addValues[ht.getCount()]);
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < targetCount; ++i) ht.find(addValues[i]);
        double successTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
                                 std::chrono::high_resolution_clock::now() - start)
                                 .count() /
                             targetCount;

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < targetCount; ++i) ht.find(checkValues[i]);
        double failTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
                              std::chrono::high_resolution_clock::now() - start)
                              .count() /
                          targetCount;

        file << lf << "," << successTime << "," << failTime << "\n";
    }
}

/*
 * Main Method: Executes tests, timings, and exports CSVs for Python plotting
 */
int main() {
    runTests();
    timeRehashCost();
    timeLoadFactor();
    std::cout << "\nAll data files generated successfully. Use Python scripts to plot graphs for Part 2.4.\n";
    return 0;
}
