#pragma once

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

template <typename TreeNode>
TreeNode *getMinimum(TreeNode *node) {
  if (node == nullptr) return node;
  while (node->left != nullptr) {
    node = node->left;
  }
  return node;
}

template <typename TreeNode>
TreeNode *getMaximum(TreeNode *node) {
  if (node == nullptr) return node;
  while (node->right != nullptr) {
    node = node->right;
  }
  return node;
}

template <typename TreeNode>
TreeNode *getPredecessor(TreeNode *node) {
  if (node == nullptr) return node;
  if (node->left != nullptr) return getMaximum(node->left);
  TreeNode *p = node->parent;
  while (p != nullptr && node == p->left) {
    node = p;
    p = p->parent;
  }
  return p;
}

template <typename TreeNode>
TreeNode *getSuccessor(TreeNode *node) {
  if (node == nullptr) return node;
  if (node->right != nullptr) return getMinimum(node->right);
  TreeNode *p = node->parent;
  while (p != nullptr && node == p->right) {
    node = p;
    p = p->parent;
  }
  return p;
}

} // end namespace Chapter12
