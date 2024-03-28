#pragma once

#include "chapter12/tree.h"

template <typename T>
struct RedblackNode : public Chapter12::Node<T> {
  enum Color {Red, Black};

  Color color;

  Color getColor() { return color; }
};

template <typename T, typename Compare = std::less<T>>
class RedBlackTree : public Tree<RedBlackTree<T, Compare>> {
 public:
  using value_type = T;
  using key_compare = Compare;
  using node_type = RedBlackNode<value_type>;
  using size_type = size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

 private:
  int hello;
};
