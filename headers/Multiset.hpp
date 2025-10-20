#ifndef __MULTISET_HPP__
#define __MULTISET_HPP__

#include <cstddef>
#include <ostream>

template <typename Data>
class Multiset
{
    template <typename T> 
    friend std::ostream& operator<<(std::ostream& out, const Multiset<T>& rhv);
struct Node {
        Node(const Data& data,
                   Node* parent = NULL,
                   Node* left = NULL,
                   Node* right = NULL)
            : data_(data)
            , parent_(parent), left_(left), right_(right)
        {}
        Data data_;
        Node* parent_;
        Node* left_;
        Node* right_;
    };

public:
    typedef Data value_type;
    typedef Data key_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::size_t size_type;

///                     ====================CONST_ITERATOR=======
public:
    class const_iterator {
    friend class Multiset<Data>;
    public:
        const_iterator();
        const_iterator(const const_iterator& rhv);
        ~const_iterator();

        const_iterator&   operator=(const const_iterator& rhv);
        const value_type& operator*()  const;
        const value_type* operator->() const;

        const_iterator&   operator++();
        const_iterator    operator++(int);
        const_iterator&   operator--();
        const_iterator    operator--(int);
        bool              operator!()                           const;
        bool              operator==(const const_iterator& rhv) const;
        bool              operator!=(const const_iterator& rhv) const;

    protected:
        void setPtr(Node* ptr);
        Node* getPtr()          const;
        const_iterator right()  const;
        const_iterator left()   const;
        const_iterator parent() const;
        bool isLeftChild()      const;
        bool isRightChild()     const;
        void setParent(const_iterator it);
        void setLeft(const_iterator it);
        void setRight(const_iterator it);
        void createRight(const value_type& x);
        void createLeft(const value_type& x);
        int depth()     const;
        operator bool() const;
        const_iterator& moveParent();
        const_iterator& moveRight();
        const_iterator& moveLeft();

    private:
        explicit const_iterator(Node* ptr);

    private:
        Node* ptr_;
    };
///                     ====================ITERATOR========

public:
    class iterator : public const_iterator {
    friend class Multiset<Data>;
    public:
        iterator();
        iterator(const iterator& rhv);
        ~iterator();
        iterator&   operator=(const iterator& rhv);
        value_type& operator*();
        value_type* operator->();
        bool        operator==(const iterator& rhv) const;
        bool        operator!=(const iterator& rhv) const;
        iterator&   operator++();
        iterator    operator++(int);
        iterator&   operator--();
        iterator    operator--(int);

    private:
        iterator firstLeftParent()  const;
        iterator firstRightParent() const;
        iterator& moveParent();
        iterator& moveRight();
        iterator& moveLeft();
        iterator right();
        iterator left();
        iterator parent();
        int balance() const;

    private:
        explicit iterator(Node* ptr);
    };
///                     ====================CONST_REVERSE_ITERATOR=======
public:
    class const_reverse_iterator {
    friend class Multiset<Data>;
    public:
        const_reverse_iterator();
        const_reverse_iterator(const const_reverse_iterator& rhv);
        ~const_reverse_iterator();

        const_reverse_iterator& operator=(const const_reverse_iterator& rhv);
        const value_type&       operator*()  const;
        const value_type*       operator->() const;
        const_reverse_iterator& operator++(); 
        const_reverse_iterator  operator++(int);
        const_reverse_iterator& operator--();   
        const_reverse_iterator  operator--(int);
        bool                    operator==(const const_reverse_iterator& rhv) const;
        bool                    operator!=(const const_reverse_iterator& rhv) const;
        const_iterator base() const;  

    protected:
        explicit const_reverse_iterator(const_iterator base);

    private:
        const_iterator current_;
    };
///                     ====================REVERSE_ITERATOR=======

public:
    class reverse_iterator : public const_reverse_iterator {
    friend class Multiset<Data>;
    public:
        reverse_iterator();
        reverse_iterator(const reverse_iterator& rhv);
        explicit reverse_iterator(iterator base);
        ~reverse_iterator();

        reverse_iterator& operator=(const reverse_iterator& rhv);
        value_type&       operator*();
        value_type*       operator->();
        reverse_iterator& operator++();
        reverse_iterator  operator++(int);
        reverse_iterator& operator--();
        reverse_iterator  operator--(int);

        iterator base() const;
    };

public:
    Multiset();
    Multiset(const Multiset<Data>& rhv);
    template <typename InputIterator>
    Multiset(InputIterator first, InputIterator last); 
    ~Multiset();

    Multiset& operator= (const Multiset& rhv);
    bool operator==(const Multiset& rhv) const;
    bool operator!=(const Multiset& rhv) const;
    bool operator< (const Multiset& rhv) const;
    bool operator<=(const Multiset& rhv) const;
    bool operator> (const Multiset& rhv) const;
    bool operator>=(const Multiset& rhv) const;

    iterator insert(const value_type& value);
    iterator insert(iterator pos, const value_type& value);
    template <class InputIterator>
    void     insert(InputIterator first, InputIterator last);

    iterator  erase(iterator position);
    size_type erase(const key_type& key);
    void      erase(iterator first, iterator last);

    iterator  find(const key_type& key);
    size_type count(const key_type& key)            const;
    const_iterator lower_bound(const key_type& key) const;
    const_iterator upper_bound(const key_type& key) const;
    iterator       lower_bound(const key_type& key);
    iterator       upper_bound(const key_type& key);
    std::pair<iterator, bool> boundHelper(iterator root, const key_type& key);
    std::pair<const_iterator, bool> boundHelper(const_iterator root, const key_type& key) const;
    std::pair<const_iterator, const_iterator> equal_range(const key_type& key)            const;
    std::pair<iterator, iterator> equal_range(const key_type& key);
    void swap(Multiset& rhv);

    void preOrder(Node* root);
    void preOrderIterative(Node* root);
    void inOrder(Node* root);
    void inOrderIterative(Node* root);
    void postOrder(Node* root);
    void postOrderIterative(Node* root);
    void levelOrder(Node* root);
    
    size_type      size()     const;
    size_type      max_size() const;
    bool           empty()    const;
    void           clear();

    iterator       begin();
    iterator       end();
    const_iterator begin()    const;
    const_iterator end()      const;

private:
    void balance(iterator& it);
    void rotateLeft(iterator& it);
    void rotateRight(iterator& it);
    void goUp(iterator& it, const value_type& x);
    static Node* getRightMost(Node* ptr);
    static Node* getLeftMost(Node* ptr);

    void outputTree(Node* ptr, std::ostream& out, const int totalSpaces = 0) const;
    bool isRoot(const const_iterator& rhv) const;
    void goDownAndInsert(iterator& it, const value_type& x);
    iterator insertHelper(iterator it, const value_type& x);
    void clearNode(Node* ptr);
    void copyHepler(Node* root);
    void visit(Node* ptr) const;
    Node*& nextPreOrder(Node*& ptr);
    static Node*& nextInOrder(Node*& ptr);
    static Node*& prevInOrder(Node*& ptr);
    Node*& nextPostOrder(Node*& ptr);

private:
    Node* root_;
};

#include "../templates/Multiset.cpp"

#endif // __SET_HPP__

