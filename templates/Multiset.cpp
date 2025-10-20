#ifndef __MULTISET_CPP__
#define __MULTISET_CPP__

#include "../headers/Multiset.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <limits>
#include <iostream>
#include <iomanip>
#include <stack>
#include <queue>
#include <utility>

template <typename Data>
Multiset<Data>::Multiset()
    : root_(NULL)
{}

template <typename Data>
template <typename InputIterator>
Multiset<Data>::Multiset(InputIterator first, InputIterator last)
{
    while (first != last) {
        insert(*first);
        ++first;
    }
}

template <typename Data>
Multiset<Data>::Multiset(const Multiset<Data>& rhv)
    : root_(NULL)
{
    copyHepler(rhv.root_);
}

template <typename Data>
void
Multiset<Data>::copyHepler(Node* root)
{
    if (NULL == root) return;
    insert(root->data_);
    copyHepler(root->left_);
    copyHepler(root->right_);
}


template <typename Data>
Multiset<Data>::~Multiset()
{
    clear();
}

template <typename Data>
Multiset<Data>&
Multiset<Data>::operator=(const Multiset& rhv)
{
    if (this != &rhv) {
        clear();
        copyHepler(rhv.root_);
    }
    return *this;
}

template <typename Data>
bool
Multiset<Data>::operator==(const Multiset<Data>& rhv) const
{
    if (this == &rhv) return true;
    if (size() != rhv.size()) return false;
    const_iterator it1 = begin();
    const_iterator it2 = rhv.begin();
    for (; it1 != end(); ++it1, ++it2) {
        if (*it1 != *it2) return false;
    }
    return true;
}

template <typename Data>
bool
Multiset<Data>::operator!=(const Multiset<Data>& rhv) const
{
    return !(*this == rhv);
}

template <typename Data>
bool
Multiset<Data>::operator<(const Multiset<Data>& rhv) const
{
    if (this == &rhv) return false;
    const_iterator it1 = begin();
    const_iterator it2 = rhv.begin();
    for (; it1 != end() || it2 != rhv.end(); ++it1, ++it2) {
        if (*it1 < *it2) return true;
        if (*it1 > *it2) return false;
    }
    return it1 == end() && it2 != rhv.end();
}

template <typename Data>
bool
Multiset<Data>::operator>(const Multiset<Data>& rhv) const
{
    return (rhv < *this);
}

template <typename Data>
bool
Multiset<Data>::operator>=(const Multiset<Data>& rhv) const
{
    return !(*this < rhv);
}

template <typename Data>
bool
Multiset<Data>::operator<=(const Multiset<Data>& rhv) const
{
    return !(*this > rhv);
}

template <typename Data>
typename Multiset<Data>::iterator
Multiset<Data>::insert(const value_type& x)
{
    return insertHelper(iterator(root_), x);
}

template <typename Data>
typename Multiset<Data>::iterator
Multiset<Data>::insert(iterator it, const value_type& x)
{
    return insertHelper(it, x);
}

template <typename Data>
template <typename InputIterator>
void
Multiset<Data>::insert(InputIterator first, InputIterator last)
{
    while (first != last) {
        insert(*first);
        ++first;
    }
}

template <typename Data>
typename Multiset<Data>::iterator
Multiset<Data>::insertHelper(iterator it, const value_type& x)
{
    if (empty()) {
        root_ = new Node(x);
        return begin();
    }
    goUp(it, x);
    goDownAndInsert(it, x);
    balance(it);
    return it;
}

template <typename Data>
typename Multiset<Data>::iterator
Multiset<Data>::erase(iterator position)
{
    if (position == end()) return end();
    iterator next;
    if (position.left() && position.right()) {
        iterator succ = position.right();
        succ = iterator(getLeftMost(succ.getPtr()));
        *position = *succ;
        next = erase(succ);
        return next;
    }

    iterator child = position.left() ? position.left() : position.right();
    iterator parent = position.parent();

    if (child) child.setParent(parent);
    if (!parent) {
        root_ = child.getPtr();
    } else if (position.isLeftChild()) {
        parent.setLeft(child);
    } else {
        parent.setRight(child);
    }
    next = ++position;
    delete position.getPtr();
    if (parent) {
        balance(parent);
    }
    return next;

}

template <typename Data>
void
Multiset<Data>::erase(iterator first, iterator last)
{
    while (first != last) {
        first = erase(first);
    }
}

template <typename Data>
typename Multiset<Data>::size_type
Multiset<Data>::erase(const key_type& key)
{
    iterator first = lower_bound(key);
    iterator last = upper_bound(key);
    if (!key) return 0;
    size_type count = 0;
    while (first != last) {
        first = erase(first);
        ++count;
    }
    return count;
}

template <typename Data>
typename Multiset<Data>::iterator
Multiset<Data>::find(const key_type& key)
{
    Node* current = root_;
    Node* result = NULL;

    while (NULL != current) {
        if (key < current->data_) {
            current = current->left_;
        } else if (key > current->data_) {
            current = current->right_;
        } else {
            result = current;
            current = current->left_;
        }
    }
    return (NULL != result) ? iterator(result) : end();
}

template <typename Data>
typename Multiset<Data>::size_type
Multiset<Data>::count(const key_type& key) const
{
    const_iterator first = lower_bound(key);
    const_iterator last = upper_bound(key);
    size_type count = 0;
    while (first != last) {
        ++first;
        ++count;
    }
    return count;
}

template <typename Data>
typename Multiset<Data>::const_iterator
Multiset<Data>::lower_bound(const key_type& key) const
{
    return boundHelper(const_iterator(root_), key).first;
}

template <typename Data>
typename Multiset<Data>::const_iterator
Multiset<Data>::upper_bound(const key_type& key) const
{
    std::pair<const_iterator, bool> p = boundHelper(const_iterator(root_), key);
    return p.second ? ++p.first : p.first;
}

template <typename Data>
typename Multiset<Data>::iterator
Multiset<Data>::lower_bound(const key_type& key)
{
    return boundHelper(iterator(root_), key).first;
}

template <typename Data>
typename Multiset<Data>::iterator
Multiset<Data>::upper_bound(const key_type& key)
{
    std::pair<iterator, bool> p = boundHelper(iterator(root_), key);
    return p.second ? ++p.first : p.first;
}

template <typename Data>
std::pair<typename Multiset<Data>::iterator, bool>
Multiset<Data>::boundHelper(iterator root, const key_type& key)
{
    if (!root) return std::make_pair(root, false);
    if (key < *root) {
        if (root.left()) {
            return boundHelper(root.left(), key);
        }
        ++root;
        return std::make_pair(root, false);
    }
    if (*root < key) {
        if (root.right()) {
            return boundHelper(root.right(), key);
        }
        return std::make_pair(++root, false);
    }
    return std::make_pair(root, true);
}

template <typename Data>
std::pair<typename Multiset<Data>::const_iterator, bool>
Multiset<Data>::boundHelper(const_iterator root, const key_type& key) const
{
    if (!root) {
        return std::make_pair(root, false);
    }
    if (key < *root) {
        if (root.left()) {
            return boundHelper(root.left(), key);
        }
        ++root;
        return std::make_pair(root, false);
    }
    if (*root < key) {
        if (root.right()) {
            return boundHelper(root.right(), key);
        }
        return std::make_pair(++root, false);
    }
    return std::make_pair(root, true);
}

template <typename Data>
std::pair<typename Multiset<Data>::iterator, typename Multiset<Data>::iterator>
Multiset<Data>::equal_range(const key_type& key)
{
    return std::make_pair(lower_bound(key), upper_bound(key));
}

template <typename Data>
std::pair<typename Multiset<Data>::const_iterator, typename Multiset<Data>::const_iterator>
Multiset<Data>::equal_range(const key_type& key) const
{
    return std::make_pair(lower_bound(key), upper_bound(key));
}

template <typename Data>
void
Multiset<Data>::swap(Multiset& rhv)
{
    std::swap(root_, rhv.root_);
}

template <typename Data>
void
Multiset<Data>::preOrder(Node* root)
{
    if (root != NULL) {
        std::cout << root->data_ << ' ';
        preOrder(root->left_);
        preOrder(root->right_);
    }
}

template <typename Data>
void 
Multiset<Data>::inOrder(Node* root)
{
    if (root != NULL) {
        inOrder(root->left_);
        std::cout << root->data_ << ' ';
        inOrder(root->right_);
    }
}

template <typename Data>
void
Multiset<Data>::postOrder(Node* root)
{
    if (root != NULL) {
        postOrder(root->left_);
        postOrder(root->right_);
        std::cout << root->data_ << ' ';
    }
}

template <typename Data>
void
Multiset<Data>::levelOrder(Node* root)
{
    if (root == NULL) return;

    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        std::cout << current->data_ << ' ';

        if (current->left_ != NULL)  q.push(current->left_);
        if (current->right_ != NULL) q.push(current->right_);
    }
}

template <typename Data>
void
Multiset<Data>::preOrderIterative(Node* root)
{
    if (root == NULL) return;

    std::stack<Node*> stack;
    Node* current = root;

    while (current != NULL || !stack.empty()) {
        while (current != NULL) {

            std::cout << current->data_ << ' ';
            if (current->right_ != NULL) {
                stack.push(current->right_);
            }

            current = current->left_;
        }

        if (!stack.empty()) {
            current = stack.top();
            stack.pop();
        }
    }
}

template <typename Data>
void
Multiset<Data>::postOrderIterative(Node* root)
{
    if (NULL == root) return;
    while (root->right_ != NULL) {
        if (root->left_ != NULL) root = getLeftMost(root);
        if (root->right_ != NULL) root = root->right_;
    }
    while (root != NULL) {
        visit(root);
        nextPostOrder(root);
    }
}

template <typename Data>
typename Multiset<Data>::Node*&
Multiset<Data>::nextPreOrder(Node*& ptr)
{
    if (NULL == ptr) return ptr;

    if (ptr->left_ != NULL) {
        ptr = ptr->left_;
        return ptr;
    }
    if (ptr->right_ != NULL) {
        ptr = ptr->right_;
        return ptr;
    }

    while (ptr->parent_ != NULL) {
        if (ptr->parent_->left_ == ptr && ptr->parent_->right_ != NULL) {
            ptr = ptr->parent_->right_;
            return ptr;
        }
        ptr = ptr->parent_;
    }

    ptr = NULL;
    return ptr;
}

template <typename Data>
void
Multiset<Data>::inOrderIterative(Node* root)
{
    if (NULL == root) return;

    root = getLeftMost(root);

    while (root != NULL) {
        visit(root);
        nextInOrder(root);
    }
}

template <typename Data>
typename Multiset<Data>::Node*&
Multiset<Data>::nextInOrder(Node*& ptr)
{
    if (ptr->right_ != NULL) {
        ptr = ptr->right_;
        ptr = getLeftMost(ptr);
        return ptr;
    }

    while (ptr->parent_ != NULL) {
        if (ptr->parent_->left_ == ptr) {
            ptr = ptr->parent_;
            return ptr;
        }
        ptr = ptr->parent_;
    }

    ptr = NULL;
    return ptr;
}

template <typename Data>
typename Multiset<Data>::Node*&
Multiset<Data>::prevInOrder(Node*& ptr)
{
    if (ptr->left_ != NULL) {
        ptr = ptr->left_;
        ptr = getRightMost(ptr);
        return ptr;
    }

    while (ptr->parent_ != NULL) {
        if (ptr->parent_->right_ == ptr) {
            ptr = ptr->parent_;
            return ptr;
        }
        ptr = ptr->parent_;
    }

    ptr = NULL;
    return ptr;
}

template <typename Data>
typename Multiset<Data>::Node*&
Multiset<Data>::nextPostOrder(Node*& ptr)
{
    if (NULL == ptr) return ptr;

    Node* previous = ptr->parent_;
    if (NULL == previous) {
        ptr = NULL;
        return ptr;
    }

    if (previous->left_ == ptr && previous->right_ != NULL) {
        ptr = previous->right_;
        if (ptr->left_ != NULL) {
            ptr = getLeftMost(ptr);
        } else {
            ptr = getRightMost(ptr);
        }
        return ptr;
    }

    ptr = previous;
    return ptr;
}

template <typename Data>
void
Multiset<Data>::clear()
{
    clearNode(root_);
    root_ = NULL;
}

template <typename Data>
bool
Multiset<Data>::empty() const
{
    return NULL == root_;
}

template <typename Data>
typename Multiset<Data>::size_type
Multiset<Data>::size() const
{
    size_type i = 0;
    const_iterator it = begin();
    while (it != end()) {
        ++i;
        ++it;
    }
    return i;
}

template <typename Data>
typename Multiset<Data>::size_type
Multiset<Data>::max_size() const
{
    return std::numeric_limits<size_t>::max() / sizeof(Node);
}

template <typename Data>
typename Multiset<Data>::const_iterator
Multiset<Data>::begin() const
{
    return const_iterator(getLeftMost(root_));
}

template <typename Data>
typename Multiset<Data>::const_iterator
Multiset<Data>::end() const
{
    return const_iterator(NULL);
}

template <typename Data>
typename Multiset<Data>::iterator
Multiset<Data>::begin()
{
    return iterator(getLeftMost(root_));
}

template <typename Data>
typename Multiset<Data>::iterator
Multiset<Data>::end()
{
    return iterator(NULL);
}   

template <typename Data>
void
Multiset<Data>::goUp(iterator& it, const value_type& x)
{
    if (!it.parent() || x == *it) return;
    const const_iterator temp = (x < *it) 
                              ? it.firstLeftParent()
                              : it.firstRightParent();
    if (isRoot(temp)) return;
    const bool isRightPlace = (x < *it) 
                            ? (*temp < x) 
                            : (*temp > x);
    if (isRightPlace) return;
    it.ptr_ = temp.ptr_;
    goUp(it, x);
}

template <typename Data>
void
Multiset<Data>::goDownAndInsert(iterator& it, const value_type& x)
{
    if (!it) return;

    if (x <= *it) {
        if (!it.left()) {
            it.createLeft(x);
            return;
        }
        return goDownAndInsert(it.moveLeft(), x);
    } else {
        if (!it.right()) {
            it.createRight(x);
            return;
        }
        return goDownAndInsert(it.moveRight(), x);
    }
}

template <typename Data>
void
Multiset<Data>::rotateRight(iterator& it)
{
    iterator itParent = it.parent(), itRight = it.right();
    const bool isRightParent = it.isLeftChild();
    it.setRight(itRight.left());
    if (itRight.left()) itRight.left().setParent(it);
    itRight.setLeft(it);
    it.setParent(itRight);
    itRight.setParent(itParent);
    if (itParent) {
        isRightParent ? itParent.setLeft(itRight)
                      : itParent.setRight(itRight);
    } else { root_ = itRight.getPtr(); }
    it = itRight;
}

template <typename Data>
void
Multiset<Data>::rotateLeft(iterator& it)
{
    iterator itParent = it.parent(), itLeft = it.left();
    const bool isLeftParent = it.isRightChild();
    it.setLeft(itLeft.right());
    if (itLeft.right()) { itLeft.right().setParent(it); }
    itLeft.setRight(it);
    it.setParent(itLeft);
    itLeft.setParent(itParent);
    if (itParent) {
        isLeftParent ? itParent.setRight(itLeft)
                     : itParent.setLeft(itLeft);
    } else { root_ = itLeft.getPtr(); }
    it = itLeft;
}

template <typename Data>
void
Multiset<Data>::balance(iterator& it)
{
    if (!it) return;
    const int factor = it.balance();
    if (factor > 1) {
        iterator itLeft = it.left();
        if (itLeft.balance() < 0) rotateRight(itLeft);
        rotateLeft(it);
        return;
    } 
    if (factor < -1) {
        iterator itRight = it.right();
        if (itRight.balance() > 0) rotateLeft(itRight);
        rotateRight(it);
        return;
      }
    balance(it.moveParent());
}

template <typename Data>
bool
Multiset<Data>::isRoot(const const_iterator& rhv) const
{
    return rhv == const_iterator(root_);
}

template <typename Data>
typename Multiset<Data>::Node*
Multiset<Data>::getRightMost(Node* ptr)
{
    if (NULL == ptr) return NULL;
    while (ptr->right_ != NULL) ptr = ptr->right_;
    return ptr;
}

template <typename Data>
typename Multiset<Data>::Node*
Multiset<Data>::getLeftMost(Node* ptr)
{
    if (NULL == ptr) return NULL;
    while (ptr->left_ != NULL) ptr = ptr->left_;
    return ptr;
}

template <typename Data>
void
Multiset<Data>::clearNode(Node* ptr)
{
    if (NULL == ptr) return;
    clearNode(ptr->left_);
    clearNode(ptr->right_);
    delete ptr;
    ptr = NULL;
}

template <typename Data>
void
Multiset<Data>::visit(Node* ptr) const
{
    std::cout << ptr->data_ << ' ';
}

///========================================CONST_ITERATOR===========

template <typename Data>
Multiset<Data>::const_iterator::const_iterator()
    : ptr_(NULL)
{}

template <typename Data>
Multiset<Data>::const_iterator::const_iterator(const const_iterator& rhv)
    : ptr_(rhv.getPtr())
{}

template <typename Data>
Multiset<Data>::const_iterator::const_iterator(Node* ptr)
    : ptr_(ptr)
{}

template <typename Data>
Multiset<Data>::const_iterator::~const_iterator()
{
    ptr_ = NULL;
}

template <typename Data>
typename Multiset<Data>::const_iterator&
Multiset<Data>::const_iterator::operator=(const const_iterator& rhv)
{
    if (this == &rhv) return *this;
    ptr_ = rhv.getPtr();
    return *this;
}

template <typename Data>
const typename Multiset<Data>::value_type&
Multiset<Data>::const_iterator::operator*() const
{
    return ptr_->data_;
}

template <typename Data>
const typename Multiset<Data>::value_type*
Multiset<Data>::const_iterator::operator->() const
{
    return &(ptr_->data_);
}

template <typename Data>
bool
Multiset<Data>::const_iterator::isRightChild() const
{
    return ptr_->parent_ != NULL && ptr_->parent_->right_ == ptr_;
}

template <typename Data>
bool
Multiset<Data>::const_iterator::isLeftChild() const
{
    return ptr_->parent_ != NULL && ptr_->parent_->left_ == ptr_;
}

template <typename Data>
typename Multiset<Data>::const_iterator&
Multiset<Data>::const_iterator::operator++()
{
    ptr_ = nextInOrder(ptr_);
    return *this;
}

template <typename Data>
typename Multiset<Data>::const_iterator&
Multiset<Data>::const_iterator::operator--()
{
    ptr_ = prevInOrder(ptr_);
    return *this;
}

template <typename Data>
typename Multiset<Data>::const_iterator
Multiset<Data>::const_iterator::operator++(int)
{
    assert(ptr_ != NULL);
    const_iterator temp = *this;
    ++(*this);
    return temp;
}

template <typename Data>
typename Multiset<Data>::const_iterator
Multiset<Data>::const_iterator::operator--(int)
{
    assert(ptr_ != NULL);
    const_iterator temp = *this;
    --(*this);
    return temp;
}

template <typename Data>
bool
Multiset<Data>::const_iterator::operator==(const const_iterator& rhv) const
{
    return rhv.getPtr() == getPtr();
}

template <typename Data>
bool
Multiset<Data>::const_iterator::operator!=(const const_iterator& rhv) const
{
    return !(*this == rhv);
}

template <typename Data>
int
Multiset<Data>::const_iterator::depth() const
{
    if (NULL == ptr_) { return 0; }
    const int leftDepth = left().depth();
    const int rightDepth = right().depth();
    return std::max(leftDepth, rightDepth) + 1;
}

template <typename Data>
typename Multiset<Data>::Node*
Multiset<Data>::const_iterator::getPtr() const
{
    return ptr_;
}

template <typename Data>
typename Multiset<Data>::const_iterator
Multiset<Data>::const_iterator::parent() const
{
    return const_iterator(ptr_->parent_);
}

template <typename Data>
typename Multiset<Data>::const_iterator
Multiset<Data>::const_iterator::left() const
{
    return const_iterator(ptr_->left_);
}

template <typename Data>
typename Multiset<Data>::const_iterator
Multiset<Data>::const_iterator::right() const
{
    return const_iterator(ptr_->right_);
}

template <typename Data>
void
Multiset<Data>::const_iterator::setPtr(Node* ptr)
{
    assert(ptr != NULL);
    ptr_ = ptr;
}

template <typename Data>
void
Multiset<Data>::const_iterator::setParent(const_iterator it)
{
    ptr_->parent_ = it.getPtr();
}

template <typename Data>
void
Multiset<Data>::const_iterator::setLeft(const_iterator it)
{
    ptr_->left_ = it.getPtr();
}

template <typename Data>
void
Multiset<Data>::const_iterator::setRight(const_iterator it)
{
    ptr_->right_ = it.getPtr();
}

template <typename Data>
void
Multiset<Data>::const_iterator::createLeft(const value_type& x)
{
    ptr_->left_ = new Node(x, ptr_);
}

template <typename Data>
void
Multiset<Data>::const_iterator::createRight(const value_type& x)
{
    ptr_->right_ = new Node(x, ptr_);
}

template <typename Data>
bool
Multiset<Data>::const_iterator::operator!() const
{
    return NULL == ptr_;
}

template <typename Data>
Multiset<Data>::const_iterator::operator bool() const
{
    return ptr_ != NULL;
}

template <typename Data>
typename Multiset<Data>::iterator
Multiset<Data>::iterator::firstLeftParent() const
{
    Node* current = this->getPtr();
    while (current && current->parent_ && current->parent_->left_ == current) {
        current = current->parent_;
    }
    return iterator(current ? current->parent_ : NULL);
}

template <typename Data>
typename Multiset<Data>::iterator
Multiset<Data>::iterator::firstRightParent() const
{
    Node* current = this->getPtr();
    while (current && current->parent_ && current->parent_->right_ == current) {
        current = current->parent_;
    }
    return iterator(current ? current->parent_ : NULL);
}

template <typename Data>
typename Multiset<Data>::const_iterator&
Multiset<Data>::const_iterator::moveRight()
{
    setPtr(ptr_->right_);
    return *this;
}

template <typename Data>
typename Multiset<Data>::const_iterator&
Multiset<Data>::const_iterator::moveLeft()
{
    ptr_ = ptr_->left_;
    return *this;
}
template <typename Data>
typename Multiset<Data>::const_iterator&
Multiset<Data>::const_iterator::moveParent()
{
    ptr_ = ptr_->parent_;
    return *this;
}

///=================================ITERATOR=================================

template <typename Data>
Multiset<Data>::iterator::iterator()
    : const_iterator()
{}

template <typename Data>
Multiset<Data>::iterator::iterator(const iterator& rhv)
    : const_iterator(rhv.getPtr())
{}

template <typename Data>
Multiset<Data>::iterator::iterator(Node* ptr)
    : const_iterator(ptr)
{}

template <typename Data>
Multiset<Data>::iterator::~iterator()
{}

template <typename Data>
typename Multiset<Data>::value_type&
Multiset<Data>::iterator::operator*()
{
    return this->getPtr()->data_;
}

template <typename Data>
typename Multiset<Data>::value_type*
Multiset<Data>::iterator::operator->()
{
    return &(this->getPtr()->data_);
}

template <typename Data>
bool
Multiset<Data>::iterator::operator==(const iterator& rhv) const
{
    return rhv.getPtr() == this->getPtr();
}

template <typename Data>
bool
Multiset<Data>::iterator::operator!=(const iterator& rhv) const
{
    return !(*this == rhv);
}

template <typename Data>
typename Multiset<Data>::iterator& 
Multiset<Data>::iterator::operator=(const iterator& rhv)
{
    this->setPtr(rhv.getPtr());
    return *this;
}

template <typename Data>
int
Multiset<Data>::iterator::balance() const
{
    const_iterator self(this->getPtr());
    return self.left().depth() - self.right().depth();
}

template <typename Data>
typename Multiset<Data>::iterator&
Multiset<Data>::iterator::operator++()  
{
    this->ptr_ = nextInOrder(this->ptr_);
    return *this;
}

template <typename Data>
typename Multiset<Data>::iterator
Multiset<Data>::iterator::operator++(int)
{
    iterator temp = *this;
    ++(*this);
    return temp;
}

template <typename Data>
typename Multiset<Data>::iterator&
Multiset<Data>::iterator::operator--()
{
    this->ptr_ = prevInOrder(this->ptr_);
    return *this;
}

template <typename Data>
typename Multiset<Data>::iterator
Multiset<Data>::iterator::operator--(int)
{
    iterator temp = *this;
    --(*this);
    return temp;
}

template <typename Data>
typename Multiset<Data>::iterator&
Multiset<Data>::iterator::moveRight()
{
    this->setPtr(this->ptr_->right_);
    return *this;
}

template <typename Data>
typename Multiset<Data>::iterator&
Multiset<Data>::iterator::moveLeft()
{
    this->ptr_ = this->getPtr()->left_;
    return *this;
}

template <typename Data>
typename Multiset<Data>::iterator&
Multiset<Data>::iterator::moveParent()
{
    this->ptr_ = this->getPtr()->parent_;
    return *this;
}

template <typename Data>
typename Multiset<Data>::iterator
Multiset<Data>::iterator::parent()
{
    Node* temp = this->getPtr();
    return NULL == temp ? iterator(temp) : iterator(temp->parent_);
}

template <typename Data>
typename Multiset<Data>::iterator
Multiset<Data>::iterator::left()
{
    Node* temp = this->getPtr();
    return NULL == temp ? iterator(temp) : iterator(temp->left_);
}

template <typename Data>
typename Multiset<Data>::iterator
Multiset<Data>::iterator::right()
{
    Node* temp = this->getPtr();
    return NULL == temp ? iterator(temp) : iterator(temp->right_);
}

template <typename Data>
void 
Multiset<Data>::outputTree(Node* ptr, std::ostream& out, const int totalSpaces) const
{
    if (NULL == ptr) { return; }
    outputTree(ptr->right_, out, totalSpaces + 5);
    out << std::setw(totalSpaces) << ptr->data_ << std::endl;
    outputTree(ptr->left_, out, totalSpaces + 5);
}

template <typename Data>
std::ostream&
operator<<(std::ostream& out, const Multiset<Data>& rhv) 
{
    rhv.outputTree(rhv.root_, out);
    return out;
}

/// ====================== CONST REVERSE ITERATOR ======================

template <typename Data>
Multiset<Data>::const_reverse_iterator::const_reverse_iterator()
    : current_(const_iterator())
{}

template <typename Data>
Multiset<Data>::const_reverse_iterator::const_reverse_iterator(const const_reverse_iterator& rhv)
    : current_(rhv.current_)
{}

template <typename Data>
Multiset<Data>::const_reverse_iterator::const_reverse_iterator(const_iterator base)
    : current_(base)
{}

template <typename Data>
Multiset<Data>::const_reverse_iterator::~const_reverse_iterator()
{}

template <typename Data>
typename Multiset<Data>::const_reverse_iterator&
Multiset<Data>::const_reverse_iterator::operator=(const const_reverse_iterator& rhv)
{
    if (this != &rhv) {
        current_ = rhv.current_;
    }
    return *this;
}

template <typename Data>
const typename Multiset<Data>::value_type&
Multiset<Data>::const_reverse_iterator::operator*() const
{
    const_iterator temp = current_;
    --temp; 
    return *temp;
}

template <typename Data>
const typename Multiset<Data>::value_type*
Multiset<Data>::const_reverse_iterator::operator->() const
{
    return &(**this);
}

template <typename Data>
typename Multiset<Data>::const_reverse_iterator&
Multiset<Data>::const_reverse_iterator::operator++()
{
    --current_;
    return *this;
}

template <typename Data>
typename Multiset<Data>::const_reverse_iterator
Multiset<Data>::const_reverse_iterator::operator++(int)
{
    const_reverse_iterator temp = *this;
    --current_;
    return temp;
}

template <typename Data>
typename Multiset<Data>::const_reverse_iterator&
Multiset<Data>::const_reverse_iterator::operator--()
{
    ++current_; 
    return *this;
}

template <typename Data>
typename Multiset<Data>::const_reverse_iterator
Multiset<Data>::const_reverse_iterator::operator--(int)
{
    const_reverse_iterator temp = *this;
    ++current_;
    return temp;
}

template <typename Data>
bool
Multiset<Data>::const_reverse_iterator::operator==(const const_reverse_iterator& rhv) const
{
    return current_ == rhv.current_;
}

template <typename Data>
bool
Multiset<Data>::const_reverse_iterator::operator!=(const const_reverse_iterator& rhv) const
{
    return !(*this == rhv);
}

template <typename Data>
typename Multiset<Data>::const_iterator
Multiset<Data>::const_reverse_iterator::base() const
{
    return current_;
}

/// ======================REVERSE_ITERATOR======================

template <typename Data>
Multiset<Data>::reverse_iterator::reverse_iterator()
    : const_reverse_iterator()
{}

template <typename Data>
Multiset<Data>::reverse_iterator::reverse_iterator(const reverse_iterator& rhv)
    : const_reverse_iterator(rhv.current_)
{}

template <typename Data>
Multiset<Data>::reverse_iterator::reverse_iterator(iterator base)
    : const_reverse_iterator(base)
{}

template <typename Data>
Multiset<Data>::reverse_iterator::~reverse_iterator()
{}

template <typename Data>
typename Multiset<Data>::reverse_iterator&
Multiset<Data>::reverse_iterator::operator=(const reverse_iterator& rhv)
{
    this->current_ = rhv.current_;
    return *this;
}

template <typename Data>
typename Multiset<Data>::value_type&
Multiset<Data>::reverse_iterator::operator*()
{
    iterator temp = this->current_;
    --temp;
    return *temp;
}

template <typename Data>
typename Multiset<Data>::value_type*
Multiset<Data>::reverse_iterator::operator->()
{
    return &(**this);
}

template <typename Data>
typename Multiset<Data>::reverse_iterator&
Multiset<Data>::reverse_iterator::operator++()
{
    --this->current_;
    return *this;
}

template <typename Data>
typename Multiset<Data>::reverse_iterator
Multiset<Data>::reverse_iterator::operator++(int)
{
    reverse_iterator temp = *this;
    --this->current_;
    return temp;
}

template <typename Data>
typename Multiset<Data>::reverse_iterator&
Multiset<Data>::reverse_iterator::operator--()
{
    ++this->current_;
    return *this;
}

template <typename Data>
typename Multiset<Data>::reverse_iterator
Multiset<Data>::reverse_iterator::operator--(int)
{
    reverse_iterator temp = *this;
    ++this->current_;
    return temp;
}

template <typename Data>
typename Multiset<Data>::iterator
Multiset<Data>::reverse_iterator::base() const
{
    return iterator(this->current_.getPtr());
}

#endif /// __MULTISET_CPP__

