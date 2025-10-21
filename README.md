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
The container maintains a pointer to the root (root_), and balancing is handled recursively during insertion and deletion.

🧩 Class Template
cpp
Copy code
template <typename Data>
class Multiset {
public:
    typedef Data value_type;
    typedef size_t size_type;

    class iterator;
    class const_iterator;
    class reverse_iterator;
    class const_reverse_iterator;

    Multiset();
    template <typename InputIterator>
    Multiset(InputIterator first, InputIterator last);
    Multiset(const Multiset& rhv);
    ~Multiset();

    Multiset& operator=(const Multiset& rhv);
    bool operator==(const Multiset& rhv) const;
    bool operator!=(const Multiset& rhv) const;
    bool operator<(const Multiset& rhv) const;
    bool operator>(const Multiset& rhv) const;
    bool operator<=(const Multiset& rhv) const;
    bool operator>=(const Multiset& rhv) const;

    bool empty() const;
    size_type size() const;
    size_type max_size() const;

    iterator insert(const value_type& x);
    void insert(iterator pos, const value_type& x);
    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last);

    iterator erase(iterator position);
    void erase(iterator first, iterator last);
    size_type erase(const value_type& key);
    void clear();
    void swap(Multiset& rhv);

    iterator find(const value_type& key);
    size_type count(const value_type& key) const;
    iterator lower_bound(const value_type& key);
    iterator upper_bound(const value_type& key);
    std::pair<iterator, iterator> equal_range(const value_type& key);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    void preOrder(Node* root);
    void inOrder(Node* root);
    void postOrder(Node* root);
    void levelOrder(Node* root);
};
🧪 Example Usage
cpp
Copy code
#include <iostream>
#include "Multiset.hpp"

int main() {
    Multiset<int> ms;
    ms.insert(5);
    ms.insert(3);
    ms.insert(7);
    ms.insert(3);
    ms.insert(5);

    std::cout << "In-order traversal: ";
    for (Multiset<int>::iterator it = ms.begin(); it != ms.end(); ++it)
        std::cout << *it << ' ';
    std::cout << std::endl;

    std::cout << "Count of 3: " << ms.count(3) << std::endl;
    std::cout << "Count of 5: " << ms.count(5) << std::endl;

    ms.erase(3);

    std::cout << "After erasing 3s: ";
    for (Multiset<int>::iterator it = ms.begin(); it != ms.end(); ++it)
        std::cout << *it << ' ';
    std::cout << std::endl;

    return 0;
}
Output:

yaml
Copy code
In-order traversal: 3 3 5 5 7 
Count of 3: 2
Count of 5: 2
After erasing 3s: 5 5 7 
🧷 File Structure
css
Copy code
project/
├── headers/
│   └── Multiset.hpp
├── sources/
│   └── Multiset.cpp
├── tests/
│   └── Multiset_tests.cpp
└── main.cpp
🧰 Compilation Example
bash
Copy code
g++ -Wall -Wextra -Werror -std=c++03 -Iheaders main.cpp sources/Multiset.cpp -o multiset_demo
./multiset_demo
🧪 Google Test Example
cpp
Copy code
#include "gtest/gtest.h"
#include "Multiset.hpp"

TEST(MultisetTest, InsertAndCount) {
    Multiset<int> ms;
    ms.insert(5);
    ms.insert(3);
    ms.insert(5);
    EXPECT_EQ(ms.count(5), 2);
    EXPECT_EQ(ms.count(3), 1);
}

TEST(MultisetTest, EraseElement) {
    Multiset<int> ms;
    ms.insert(2);
    ms.insert(2);
    ms.erase(2);
    EXPECT_EQ(ms.count(2), 0);
}

TEST(MultisetTest, FindAndBounds) {
    Multiset<int> ms;
    ms.insert(1);
    ms.insert(3);
    ms.insert(5);
    EXPECT_NE(ms.find(3), ms.end());
    EXPECT_EQ(ms.find(7), ms.end());
}
Compile:

bash
Copy code
g++ -Wall -Wextra -std=c++03 -Iheaders -I/usr/include/gtest -lgtest -lpthread tests/Multiset_tests.cpp sources/Multiset.cpp -o utest_multiset
./utest_multiset
🧩 Notes
Written entirely for educational purposes (data structures course or project).

Designed for clarity and completeness, not raw performance.

Compatible with C++03 and newer standards.

Includes iterative and recursive tree traversal for better understanding of BST behavior.

🧑‍💻 Author
Arsen Dilanchyan
C++ Data Structures Project
📅 2025
