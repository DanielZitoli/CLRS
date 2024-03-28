#pragma once

#include <functional>
#include <cstddef>
#include <utility>
#include <initializer_list>

namespace Chapter12 {

template <typename T, typename Compare = std::less<T>>
class BinaryTree {
  using value_type = T;
  using key_compare = Compare;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = size_t;

//  public:
  struct Node {
    value_type data;
    Node *parent = nullptr;
    Node *left = nullptr;
    Node *right = nullptr;
    Node() {}
    Node(const value_type& val) : data{val} {}
    Node(value_type&& val) : data{std::move(val)} {}
    Node(const Node &other) : data{other.data}, parent{nullptr}, left{nullptr}, right{nullptr} {
      if (other.left) {
        left = new Node(*other.left);
        // left->parent = this;
      }
      if (other.right) {
        right = new Node(*other.right);
        // right->parent = this;
      }
      connectChildren();
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
    void connectChildren() {
      if (left != nullptr) {
        left->parent = this;
        left->connectChildren();
      }
      if (right != nullptr) {
        right->parent = this;
        right->connectChildren();
      }
    }
  };


  size_type items = 0;
  Node *head = nullptr;
  key_compare comp;

 public:
  // Constructors and Destructors
  explicit BinaryTree(const key_compare& comp = key_compare()) : comp{comp} {}
  BinaryTree(const BinaryTree &other) : items{other.items}, head{other.head? new Node(*other.head): nullptr}, comp{other.comp} {}
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
    Node *p;
    Node *prev;
   protected:
    iterator(Node *p, Node *prev): p{p}, prev{prev} {}
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
    Node *p;
    Node *prev;
   protected:
    const_iterator(Node *p, Node *prev): p{p}, prev{prev} {}
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
    Node *min = getMinimum(head);
    return createIterator(min);
    // return iterator(min, nullptr); 
  }
  const_iterator begin() const { 
    Node *min = getMinimum(head);
    return createIterator(min);
    // return const_iterator(min, nullptr);
  }
  iterator end() {
    return createIterator(nullptr);
    // return iterator(nullptr, getMaximum(head)); 
  }
  const_iterator end() const { 
    return createIterator(nullptr);
    // return const_iterator(nullptr, getMaximum(head)); 
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
    Node *node = getLowerBound(head, val);
    return iterator(node, getPredecessor(node)); 
  }
  const_iterator lower_bound(const value_type& val) const { 
    Node *node = getLowerBound(head, val);
    return const_iterator(node, getPredecessor(node)); 
  }
  iterator upper_bound(const value_type& val) { 
    Node *node = getUpperBound(head, val);
    return iterator(node, getPredecessor(node)); 
  }
  const_iterator upper_bound(const value_type& val) const { 
    Node *node = getUpperBound(head, val);
    return const_iterator(node, getPredecessor(node)); 
  }
 
 private:
  static Node *getMinimum(Node *node) {
    if (node == nullptr) return node;
    while (node->left != nullptr) {
      node = node->left;
    }
    return node;
  }
  static Node *getMaximum(Node *node) {
    if (node == nullptr) return node;
    while (node->right != nullptr) {
      node = node->right;
    }
    return node;
  }
  static Node *getPredecessor(Node *node) {
    if (node == nullptr) return node;
    if (node->left != nullptr) return getMaximum(node->left);
    Node *p = node->parent;
    while (p != nullptr && node == p->left) {
      node = p;
      p = p->parent;
    }
    return p;
  }
  static Node *getSuccessor(Node *node) {
    if (node == nullptr) return node;
    if (node->right != nullptr) return getMinimum(node->right);
    Node *p = node->parent;
    while (p != nullptr && node == p->right) {
      node = p;
      p = p->parent;
    }
    return p;
  }
  const_iterator findHelper(const value_type& val) const;
  iterator createIterator(Node *node);
  const_iterator createIterator(Node *node) const;
  Node *getLowerBound(Node *node, const value_type& val) const;
  Node *getUpperBound(Node *node, const value_type& val) const;
};

template <typename T, typename Compare>
std::pair<typename BinaryTree<T, Compare>::iterator, bool> BinaryTree<T, Compare>::insert(const value_type& val) {
  Node *p = nullptr;
  Node *cur = head;
  while (cur != nullptr) {
    p = cur;
    if (comp(val, cur->data)) {
      cur = cur->left;
    } else if (comp(cur->data, val)) {
      cur = cur->right;
    } else {
      // found element already in set
      return std::pair<iterator, bool>(end(), false);
    }
  }
  Node *newNode = new Node(val);
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
  return std::pair<iterator, bool>(iterator(newNode, getPredecessor(newNode)), true);
}

template <typename T, typename Compare>
typename BinaryTree<T, Compare>::iterator BinaryTree<T, Compare>::erase(const_iterator position) {
  Node *erasedNode = position.p;
  Node *erasedNodeNext = getSuccessor(erasedNode);
  Node *splicedNode;
  if (erasedNode->left == nullptr || erasedNode->right == nullptr) {
    splicedNode = erasedNode;
  } else {
    splicedNode = getSuccessor(erasedNode);
  }
  Node *splicedChild = splicedNode->left? splicedNode->left: splicedNode->right;

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
  }

  --items;
  return erasedNodeNext? iterator(erasedNodeNext, getPredecessor(erasedNodeNext)): end(); // maybe abstract this into a private helper "createIterator"
}

template <typename T, typename Compare>
typename BinaryTree<T, Compare>::const_iterator BinaryTree<T, Compare>::findHelper(const value_type& val) const {
  Node *cur = head;
  while (head != nullptr) {
    if (comp(val, cur->data)) {
      cur = cur->left;
    } else if (comp(cur->data, val)) {
      cur = cur->right;
    } else {
      return iterator(cur, getPredecessor(cur));
    }
  }
  return end();
}

template <typename T, typename Compare>
typename BinaryTree<T, Compare>::iterator BinaryTree<T, Compare>::createIterator(Node *node) {
  return node? iterator(node, getPredecessor(node)): iterator(nullptr, getMaximum(head)); 
}

template <typename T, typename Compare>
typename BinaryTree<T, Compare>::const_iterator BinaryTree<T, Compare>::createIterator(Node *node) const {
  return node? const_iterator(node, getPredecessor(node)): const_iterator(nullptr, getMaximum(head)); 
}


template <typename T, typename Compare>
typename BinaryTree<T, Compare>::Node *BinaryTree<T, Compare>::getLowerBound(Node *node, const value_type& val) const {
  return head;
}

template <typename T, typename Compare>
typename BinaryTree<T, Compare>::Node *BinaryTree<T, Compare>::getUpperBound(Node *node, const value_type& val) const {
  return head;
}

template <typename T, typename Compare>
std::ostream &operator<<(std::ostream &out, const BinaryTree<T, Compare> &tree) {
  for (auto val : tree) std::cout << val << " ";
  std::cout << std::endl;
  return out;
}

} // end namespace Chapter12
