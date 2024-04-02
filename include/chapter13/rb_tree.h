#pragma once

#include "chapter12/tree.h"

#include <functional>
#include <cstddef>
#include <utility>
#include <initializer_list>
#include <iostream>

namespace Chapter13 {

template <typename T>
struct RedBlackNode {
  using value_type = T;
  enum Color {Black, Red};

  value_type data;
  RedBlackNode *parent = nullptr;
  RedBlackNode *left = nullptr;
  RedBlackNode *right = nullptr;
  Color color = Color::Black;

  RedBlackNode(const value_type& val) : data{val} {}
  RedBlackNode(value_type&& val) : data{std::move(val)} {}
  RedBlackNode(const RedBlackNode &other) : data{other.data}, parent{nullptr}, left{nullptr}, right{nullptr} {
    if (other.left) {
      left = new RedBlackNode(*other.left);
      left->parent = this;
    }
    if (other.right) {
      right = new RedBlackNode(*other.right);
      right->parent = this;
    }
  }
  RedBlackNode(RedBlackNode &&other) : data{std::move(other.data)}, parent{other.parent}, left{other.left}, right{other.right} {
    other.parent = nullptr;
    other.left = nullptr;
    other.right = nullptr;
  }
  RedBlackNode &operator=(RedBlackNode other) {
    std::swap(data, other.data);
    std::swap(parent, other.parent);
    std::swap(left, other.left);
    std::swap(right, other.right);
    return *this;
  }
  ~RedBlackNode() {
    delete left;
    delete right;
  }
};


template <typename T, typename Compare = std::less<T>>
class RedBlackTree {
 public:
  using value_type = T;
  using key_compare = Compare;
  using node_type = RedBlackNode<value_type>;
  using size_type = size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

 private:
  size_type items = 0;
  node_type *head = nullptr;
  node_type *sentinel = nullptr;
  key_compare comp;

 public:
  // Constructors and Destructors
  explicit RedBlackTree(const key_compare& comp = key_compare()) : comp{comp} {
    setupSentinel();
  }
  RedBlackTree(const RedBlackTree &other) : items{other.items}, head{nullptr}, sentinel{nullptr}, comp{other.comp} {
    setupSentinel();
  }
  RedBlackTree(RedBlackTree &&other) : items{other.items}, head{other.head}, sentinel{other.sentinel}, comp{other.comp} {
    other.items = 0;
    other.head = nullptr;
  }
  RedBlackTree(std::initializer_list<value_type> il, const key_compare& comp = key_compare()) : comp{comp} {
    setupSentinel();
    for (auto &val : il) insert(val);
  }
  RedBlackTree &operator=(RedBlackTree other) {
    std::swap(items, other.items);
    std::swap(head, other.head);
    std::swap(sentinel, other.sentinel);
    std::swap(comp, other.comp);
    return *this;
  }
  ~RedBlackTree() { delete head; }

  // Capacity
  size_type size() const { return items; }
  bool empty() const { return items == 0; }

  class iterator {
    friend class RedBlackTree<T, Compare>;
    friend class const_iterator;
    node_type *p;
    node_type *prev;
   protected:
    iterator(node_type *p, node_type *prev): p{p}, prev{prev} {}
   public:
    bool operator==(const iterator &other) const { return p == other.p; }
    reference operator*() { return p->data; }
    iterator &operator++() {
      prev = p;
      p = Chapter12::getSuccessor(p);
      return *this;
    }
    iterator &operator--() {
      p = prev;
      prev = Chapter12::getPredecessor(prev);
      return *this;
    }
  };

  class const_iterator {
    friend class RedBlackTree<T, Compare>;
    node_type *p;
    node_type *prev;
   protected:
    const_iterator(node_type *p, node_type *prev): p{p}, prev{prev} {}
   public:
    const_iterator(const iterator& x) : p{x.p}, prev{x.prev} {}
    bool operator==(const const_iterator &other) const { return p == other.p; }
    const_reference operator*() const { return p->data; }
    const_iterator &operator++() {
      prev = p;
      p = Chapter12::getSuccessor(p);
      return *this;
    }
    const_iterator &operator--() {
      p = prev;
      prev = Chapter12::getPredecessor(prev);
      return *this;
    }
  };

  // Iterators
  iterator begin() { 
    node_type *min = Chapter12::getMinimum(head);
    return createIterator(min);
  }
  const_iterator begin() const { 
    node_type *min = Chapter12::getMinimum(head);
    return createIterator(min);
  }
  iterator end() {
    return createIterator(nullptr);
  }
  const_iterator end() const { 
    return createIterator(nullptr);
  }

  // Operations
  iterator find(const value_type& val) { 
    return findHelper(val); 
  }
  const_iterator find(const value_type& val) const { 
    return findHelper(val); 
  }
  size_type count(const value_type& val) const {
    return (findHelper(val) == end()) ? 0 : 1;
  }	

  std::pair<iterator,bool> insert(const value_type& val);

  iterator erase(const_iterator position);

  size_type erase(const value_type& val) {
    const_iterator position = findHelper(val);
    if (position == end()) { 
      return 0;
    } else {
      erase(position);
      return 1;
    }
  }

  iterator lower_bound(const value_type& val) { 
    node_type *node = getLowerBound(head, val);
    return createIterator(node); 
  }
  const_iterator lower_bound(const value_type& val) const { 
    node_type *node = getLowerBound(head, val);
    return createIterator(node); 
  }
  iterator upper_bound(const value_type& val) { 
    node_type *node = getUpperBound(head, val);
    return createIterator(node); 
  }
  const_iterator upper_bound(const value_type& val) const { 
    node_type *node = getUpperBound(head, val);
    return createIterator(node); 
  }

 private:
  void setupSentinel();
  iterator createIterator(node_type *node);
  const_iterator createIterator(node_type *node) const;
  const_iterator findHelper(const value_type& val) const;
  node_type *getLowerBound(node_type *node, const value_type& val) const;
  node_type *getUpperBound(node_type *node, const value_type& val) const;
  void leftRotate(node_type *node);
  void rightRotate(node_type *node);
};

template <typename T, typename Compare>
typename RedBlackTree<T, Compare>::const_iterator RedBlackTree<T, Compare>::findHelper(const value_type& val) const {
  node_type *cur = head;
  while (cur != nullptr) {
    if (comp(val, cur->data)) {
      cur = cur->left;
    } else if (comp(cur->data, val)) {
      cur = cur->right;
    } else {
      return createIterator(cur);
    }
  }
  return end();
}

template <typename T, typename Compare>
std::pair<typename RedBlackTree<T, Compare>::iterator, bool> RedBlackTree<T, Compare>::insert(const value_type& val) {
  // TODO
  return std::pair<iterator, bool>(createIterator(nullptr), false);
}

template <typename T, typename Compare>
typename RedBlackTree<T, Compare>::iterator RedBlackTree<T, Compare>::erase(const_iterator position) {
  // TODO
  return createIterator(nullptr);
}

template <typename T, typename Compare>
void RedBlackTree<T, Compare>::setupSentinel() {
  // TODO
}

template <typename T, typename Compare>
typename RedBlackTree<T, Compare>::iterator RedBlackTree<T, Compare>::createIterator(node_type *node) {
  return node? iterator(node, Chapter12::getPredecessor(node)): iterator(nullptr, Chapter12::getMaximum(head)); 
}

template <typename T, typename Compare>
typename RedBlackTree<T, Compare>::const_iterator RedBlackTree<T, Compare>::createIterator(node_type *node) const {
  return node? const_iterator(node, Chapter12::getPredecessor(node)): const_iterator(nullptr, Chapter12::getMaximum(head)); 
}

template <typename T, typename Compare>
typename RedBlackTree<T, Compare>::node_type *RedBlackTree<T, Compare>::getLowerBound(node_type *node, const value_type& val) const {
  if (node == nullptr) return node;
  if (comp(node->data, val)) { // if current node < val
    return getLowerBound(node->right, val);
  } else { // if val <= current node
    node_type *leftLowerBound = getLowerBound(node->left, val);
    return leftLowerBound? leftLowerBound: node;
  }
}

template <typename T, typename Compare>
void RedBlackTree<T, Compare>::leftRotate(node_type *x) {
  if (x == nullptr || x->right == nullptr) throw std::logic_error("x must have a valid right child");
  node_type *y = x->right;
}

template <typename T, typename Compare>
void RedBlackTree<T, Compare>::rightRotate(node_type *y) {
  if (y == nullptr || y->left == nullptr) throw std::logic_error("y must have a valid left child");
}

template <typename T, typename Compare>
typename RedBlackTree<T, Compare>::node_type *RedBlackTree<T, Compare>::getUpperBound(node_type *node, const value_type& val) const {
  if (node == nullptr) return node;
  if (comp(val, node->data)) { // if val < current node
    node_type *leftUpperBound = getUpperBound(node->left, val);
    return leftUpperBound? leftUpperBound: node;
  } else { // if current node <= val
    return getUpperBound(node->right, val);
  }
}

template <typename T, typename Compare>
std::ostream &operator<<(std::ostream &out, const RedBlackTree<T, Compare> &tree) {
  for (auto val : tree) std::cout << val << " ";
  std::cout << std::endl;
  return out;
}

} // end namespace Chapter13
