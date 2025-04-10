# Project-3-Survey-of-Algo

# Using C++ 14 - Run code - Vs code

# Hashing

## HashTable Basics

This implementation provides a string-only hash table with:

**Core Components**:
- **Storage**: Dynamic array of strings
- **Collision Handling**: Linear probing
- **Hash Function**: `std::hash<string>` with modulo operation
- **Load Factor Control**: Automatic resizing when threshold exceeded

**Resizing Strategies**:
1. **Double Size** (Strategy 1): Table size × 2
2. **Fixed Increase** (Strategy 2): Table size + 10,000

**Key Operations**:
- `insert(str)`: Adds string if not present, triggers resize when needed
- `find(str)`: Returns `true` if string exists in table

**Technical Details**:
- Empty slots marked with empty strings ("")
- Linear probing walks sequentially through slots
- Counters track total elements and resize operations