#pragma once

#include "chapter12/tree.h"

#include <functional>
#include <cstddef>
#include <utility>
#include <initializer_list>
#include <iostream>

namespace Chapter12 {

template <typename T, typename Compare = std::less<T>>
class BinaryTree {
 public:
  using value_type = T;
  using key_compare = Compare;
  using node_type = Node<value_type>;
  using size_type = size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

 private:
  size_type items = 0;
  node_type *head = nullptr;
  key_compare comp;

 public:
  // Constructors and Destructors
  explicit BinaryTree(const key_compare& comp = key_compare()) : comp{comp} {}
  BinaryTree(const BinaryTree &other) : items{other.items}, head{other.head? new node_type(*other.head): nullptr}, comp{other.comp} {}
  BinaryTree(BinaryTree &&other) : items{other.items}, head{other.head}, comp{other.comp} {
    other.items = 0;
    other.head = nullptr;
  }
  BinaryTree(std::initializer_list<value_type> il, const key_compare& comp = key_compare()) : comp{comp} {
    for (auto &val : il) insert(val);
  }
  BinaryTree &operator=(BinaryTree other) {
    std::swap(items, other.items);
    std::swap(head, other.head);
    std::swap(comp, other.comp);
    return *this;
  }
  ~BinaryTree() { delete head; }

  // Capacity
  size_type size() const { return items; }
  bool empty() const { return items == 0; }

  class iterator {
    friend class BinaryTree<T, Compare>;
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
      p = getSuccessor(p);
      return *this;
    }
    iterator &operator--() {
      p = prev;
      prev = getPredecessor(prev);
      return *this;
    }
  };

  class const_iterator {
    friend class BinaryTree<T, Compare>;
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
      p = getSuccessor(p);
      return *this;
    }
    const_iterator &operator--() {
      p = prev;
      prev = getPredecessor(prev);
      return *this;
    }
  };

  // Iterators
  iterator begin() { 
    node_type *min = getMinimum(head);
    return createIterator(min);
  }
  const_iterator begin() const { 
    node_type *min = getMinimum(head);
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
  iterator createIterator(node_type *node);
  const_iterator createIterator(node_type *node) const;
  const_iterator findHelper(const value_type& val) const;
  node_type *getLowerBound(node_type *node, const value_type& val) const;
  node_type *getUpperBound(node_type *node, const value_type& val) const;
};

template <typename T, typename Compare>
typename BinaryTree<T, Compare>::const_iterator BinaryTree<T, Compare>::findHelper(const value_type& val) const {
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
std::pair<typename BinaryTree<T, Compare>::iterator, bool> BinaryTree<T, Compare>::insert(const value_type& val) {
  node_type *p = nullptr;
  node_type *cur = head;
  while (cur != nullptr) {
    p = cur;
    if (comp(val, cur->data)) {
      cur = cur->left;
    } else if (comp(cur->data, val)) {
      cur = cur->right;
    } else {
      // found element already in set
      return std::pair<iterator, bool>(createIterator(cur), false);
    }
  }
  node_type *newNode = new node_type(val);
  newNode->parent = p;
  if (empty()) {
    head = newNode;
  } else {
    if (comp(val, p->data)) {
      p->left = newNode;
    } else {
      p->right = newNode;
    }
  }
  ++items;
  return std::pair<iterator, bool>(createIterator(newNode), true);
}

template <typename T, typename Compare>
typename BinaryTree<T, Compare>::iterator BinaryTree<T, Compare>::erase(const_iterator position) {
  node_type *erasedNode = position.p;
  node_type *erasedNodeNext = getSuccessor(erasedNode);
  node_type *splicedNode;
  if (erasedNode->left == nullptr || erasedNode->right == nullptr) {
    splicedNode = erasedNode;
  } else {
    splicedNode = getSuccessor(erasedNode);
  }
  node_type *splicedChild = splicedNode->left? splicedNode->left: splicedNode->right;

  if (splicedChild != nullptr) {
    splicedChild->parent = splicedNode->parent;
  }

  if (splicedNode->parent == nullptr) {
    head = splicedChild;
  } else {
    if (splicedNode == splicedNode->parent->left) {
      splicedNode->parent->left = splicedChild;
    } else {
      splicedNode->parent->right = splicedChild;
    }
  }

  if (splicedNode != erasedNode) {
    // copy splicedNode data into erased Node
    erasedNode->data = std::move(splicedNode->data);
    splicedNode->left = splicedNode->right = nullptr;
    delete splicedNode;
  } else {
    erasedNode->left = erasedNode->right = nullptr;
    delete erasedNode;
  }

  // if (splicedNode != erasedNode) {
  //   // insert splicedNode into erasedNode's position
  //   splicedNode->parent = erasedNode->parent;
  //   splicedNode->left = erasedNode->left;
  //   splicedNode->right = erasedNode->right;
  //   if (erasedNode->parent == nullptr) {
  //     head = splicedNode;
  //   } else {
  //     if (erasedNode == erasedNode->parent->left) {
  //       erasedNode->parent->left = splicedChild;
  //     } else {
  //       erasedNode->parent->right = splicedChild;
  //     }
  //   }
  //   if (splicedNode->left) {
  //     splicedNode->left->parent = splicedNode;
  //   }
  //   if (splicedNode->right) {
  //     splicedNode->right->parent = splicedNode;
  //   }
  // }
  // erasedNode->left = erasedNode->right = nullptr;
  // delete erasedNode;


  --items;
  return createIterator(erasedNodeNext);
}

template <typename T, typename Compare>
typename BinaryTree<T, Compare>::iterator BinaryTree<T, Compare>::createIterator(node_type *node) {
  return node? iterator(node, getPredecessor(node)): iterator(nullptr, getMaximum(head)); 
}

template <typename T, typename Compare>
typename BinaryTree<T, Compare>::const_iterator BinaryTree<T, Compare>::createIterator(node_type *node) const {
  return node? const_iterator(node, getPredecessor(node)): const_iterator(nullptr, getMaximum(head)); 
}

template <typename T, typename Compare>
typename BinaryTree<T, Compare>::node_type *BinaryTree<T, Compare>::getLowerBound(node_type *node, const value_type& val) const {
  if (node == nullptr) return node;
  if (comp(node->data, val)) { // if current node < val
    return getLowerBound(node->right, val);
  } else { // if val <= current node
    node_type *leftLowerBound = getLowerBound(node->left, val);
    return leftLowerBound? leftLowerBound: node;
  }
}

template <typename T, typename Compare>
typename BinaryTree<T, Compare>::node_type *BinaryTree<T, Compare>::getUpperBound(node_type *node, const value_type& val) const {
  if (node == nullptr) return node;
  if (comp(val, node->data)) { // if val < current node
    node_type *leftUpperBound = getUpperBound(node->left, val);
    return leftUpperBound? leftUpperBound: node;
  } else { // if current node <= val
    return getUpperBound(node->right, val);
  }
}

template <typename T, typename Compare>
std::ostream &operator<<(std::ostream &out, const BinaryTree<T, Compare> &tree) {
  for (auto val : tree) out << val << " ";
  out << std::endl;
  return out;
}

} // end namespace Chapter12
