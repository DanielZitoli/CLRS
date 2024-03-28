#pragma once

#include <functional>
#include <cstddef>
#include <utility>
#include <initializer_list>
#include <iostream>

namespace Chapter12 {

template <typename T>
struct Node {
  using value_type = T;

  value_type data;
  Node *parent = nullptr;
  Node *left = nullptr;
  Node *right = nullptr;
  Node(const value_type& val) : data{val} {}
  Node(value_type&& val) : data{std::move(val)} {}
  Node(const Node &other) : data{other.data}, parent{nullptr}, left{nullptr}, right{nullptr} {
    if (other.left) {
      left = new Node(*other.left);
      left->parent = this;
    }
    if (other.right) {
      right = new Node(*other.right);
      right->parent = this;
    }
  }
  Node(Node &&other) : data{std::move(other.data)}, parent{other.parent}, left{other.left}, right{other.right} {
    other.parent = nullptr;
    other.left = nullptr;
    other.right = nullptr;
  }
  Node &operator=(Node other) {
    std::swap(data, other.data);
    std::swap(parent, other.parent);
    std::swap(left, other.left);
    std::swap(right, other.right);
    return *this;
  }
  ~Node() {
    delete left;
    delete right;
  }
};

template <typename Derived>
class Tree {
 public:
  using value_type = typename Derived::value_type;
  using key_compare = typename Derived::Compare;
  using node_type = typename Derived::node_type;
  using iterator = typename Derived::iterator;
  using const_iterator = typename Derived::const_iterator;
  using size_type = typename Derived::size_type;
  using reference = value_type&;
  using const_reference = const value_type&;

 protected:
  size_type items = 0;
  node_type *head = nullptr;
  key_compare comp;

 public:
  // Constructors and Destructors
  // explicit Tree(const key_compare& comp = key_compare()) : comp{comp} {}
  // Tree(const Tree &other) : items{other.items}, head{other.head? new node_type(*other.head): nullptr}, comp{other.comp} {}
  // Tree(Tree &&other) : items{other.items}, head{other.head}, comp{other.comp} {
  //   other.items = 0;
  //   other.head = nullptr;
  // }
  // Tree(std::initializer_list<value_type> il, const key_compare& comp = key_compare()) : comp{comp} {
  //   for (auto &val : il) insert(val);
  // }
  // Tree &operator=(Tree other) {
  //   std::swap(items, other.items);
  //   std::swap(head, other.head);
  //   std::swap(comp, other.comp);
  //   return *this;
  // }
  virtual ~Tree() { delete head; }

  // Capacity
  size_type size() const { return items; }
  bool empty() const { return items == 0; }

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

  virtual std::pair<iterator,bool> insert(const value_type& val) = 0;

  virtual iterator erase(const_iterator position) = 0;

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
 
 protected:
  static node_type *getMinimum(node_type *node);
  static node_type *getMaximum(node_type *node);
  static node_type *getPredecessor(node_type *node);
  static node_type *getSuccessor(node_type *node);

  virtual iterator createIterator(node_type *node) = 0;
  virtual const_iterator createIterator(node_type *node) const = 0;

 private:
  const_iterator findHelper(const value_type& val) const;
  node_type *getLowerBound(node_type *node, const value_type& val) const;
  node_type *getUpperBound(node_type *node, const value_type& val) const;
};

template <typename Derived>
typename Tree<Derived>::const_iterator Tree<Derived>::findHelper(const value_type& val) const {
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

template <typename Derived>
typename Tree<Derived>::node_type *Tree<Derived>::getMinimum(node_type *node) {
  if (node == nullptr) return node;
  while (node->left != nullptr) {
    node = node->left;
  }
  return node;
}

template <typename Derived>
typename Tree<Derived>::node_type *Tree<Derived>::getMaximum(node_type *node) {
  if (node == nullptr) return node;
  while (node->right != nullptr) {
    node = node->right;
  }
  return node;
}

template <typename Derived>
typename Tree<Derived>::node_type *Tree<Derived>::getPredecessor(node_type *node) {
  if (node == nullptr) return node;
  if (node->left != nullptr) return getMaximum(node->left);
  node_type *p = node->parent;
  while (p != nullptr && node == p->left) {
    node = p;
    p = p->parent;
  }
  return p;
}

template <typename Derived>
typename Tree<Derived>::node_type *Tree<Derived>::getSuccessor(node_type *node) {
  if (node == nullptr) return node;
  if (node->right != nullptr) return getMinimum(node->right);
  node_type *p = node->parent;
  while (p != nullptr && node == p->right) {
    node = p;
    p = p->parent;
  }
  return p;
}

template <typename Derived>
typename Tree<Derived>::node_type *Tree<Derived>::getLowerBound(node_type *node, const value_type& val) const {
  if (node == nullptr) return node;
  if (comp(node->data, val)) { // if current node < val
    return getLowerBound(node->right, val);
  } else { // if val <= current node
    node_type *leftLowerBound = getLowerBound(node->left, val);
    return leftLowerBound? leftLowerBound: node;
  }
}

template <typename Derived>
typename Tree<Derived>::node_type *Tree<Derived>::getUpperBound(node_type *node, const value_type& val) const {
  if (node == nullptr) return node;
  if (comp(val, node->data)) { // if val < current node
    node_type *leftUpperBound = getUpperBound(node->left, val);
    return leftUpperBound? leftUpperBound: node;
  } else { // if current node <= val
    return getUpperBound(node->right, val);
  }
}

template <typename Derived>
std::ostream &operator<<(std::ostream &out, const Tree<Derived> &tree) {
  for (auto val : tree) std::cout << val << " ";
  std::cout << std::endl;
  return out;
}

} // end namespace Chapter12
