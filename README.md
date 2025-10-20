# 🧩 Multiset (C++03 Implementation)

## 📖 Overview

This project provides a **custom implementation of a `Multiset` container** similar to the C++ Standard Library’s `std::multiset`, implemented **entirely from scratch** using templates and AVL-style balancing.

It supports **duplicate elements**, **ordered traversal**, **iterators (including reverse and const)**, and the **basic associative container interface**.

This implementation is compatible with **C++03** standard.

---

## ⚙️ Features

✅ Supports duplicate elements  
✅ Template-based generic type  
✅ Balanced binary search tree (AVL-like)  
✅ Full iterator support (`iterator`, `const_iterator`, `reverse_iterator`, etc.)  
✅ All standard modifiers and lookup operations:  
- `insert()`, `erase()`, `clear()`, `swap()`  
- `find()`, `count()`, `lower_bound()`, `upper_bound()`, `equal_range()`  
✅ Traversal algorithms:  
- Recursive: `preOrder`, `inOrder`, `postOrder`, `levelOrder`  
- Iterative versions for each order  
✅ Relational operators (`==`, `!=`, `<`, `>`, `<=`, `>=`)  
✅ Output operator (`operator<<`) to visualize the tree  

---

## 🧠 Implementation Details

### Data Structure

Each element is stored in a **Node** structure:

```cpp
struct Node {
    Data data_;
    Node* left_;
    Node* right_;
    Node* parent_;

    Node(const Data& data, Node* parent = NULL)
        : data_(data), left_(NULL), right_(NULL), parent_(parent) {}
};
