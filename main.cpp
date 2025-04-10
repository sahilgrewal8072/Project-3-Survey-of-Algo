#include <iostream>
#include <string>
#include "HashTable.hpp"

using namespace std;

void test1() {
    HashTable ht(10, 0.75, 1);
    vector<string> strings = {"apple", "banana", "cherry", "date", "elderberry"};
    for (auto& s : strings) {
        ht.insert(s);
    }
    for (auto& s : strings) {
        if (!ht.find(s)) {
            cout << "Test 1 failed: " << s << " not found." << endl;
            return;
        }
    }
    vector<string> notPresent = {"fig", "grape", "honeydew", "kiwi", "lemon"};
    for (auto& s : notPresent) {
        if (ht.find(s)) {
            cout << "Test 1 failed: " << s << " incorrectly found." << endl;
            return;
        }
    }
    cout << "Test 1 passed." << endl;
}

void test2() {
    HashTable ht(16, 0.75, 1);
    for (int i = 0; i < 23; ++i) {
        ht.insert(to_string(i));
    }
    if (ht.getResizeCount() != 1) {
        cout << "Test 2 failed: resize count " << ht.getResizeCount() << ", expected 1." << endl;
        return;
    }
    for (int i = 0; i < 23; ++i) {
        if (!ht.find(to_string(i))) {
            cout << "Test 2 failed: " << i << " not found." << endl;
            return;
        }
    }
    cout << "Test 2 passed." << endl;
}

void test3() {
    HashTable ht(16, 0.75, 1);
    for (int i = 0; i < 24; ++i) {
        ht.insert(to_string(i));
    }
    if (ht.getResizeCount() != 2) {
        cout << "Test 3 failed: resize count " << ht.getResizeCount() << ", expected 2." << endl;
        return;
    }
    for (int i = 0; i < 24; ++i) {
        if (!ht.find(to_string(i))) {
            cout << "Test 3 failed: " << i << " not found." << endl;
            return;
        }
    }
    cout << "Test 3 passed." << endl;
}

void test4() {
    HashTable ht(16, 0.75, 2);
    for (int i = 0; i < 24; ++i) {
        ht.insert(to_string(i));
    }
    if (ht.getResizeCount() != 1) {
        cout << "Test 4 failed: resize count " << ht.getResizeCount() << ", expected 1." << endl;
        return;
    }
    for (int i = 0; i < 24; ++i) {
        if (!ht.find(to_string(i))) {
            cout << "Test 4 failed: " << i << " not found." << endl;
            return;
        }
    }
    cout << "Test 4 passed." << endl;
}

int main() {
    test1();
    test2();
    test3();
    test4();
    return 0;
}