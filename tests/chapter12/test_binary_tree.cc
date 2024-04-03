#include "gtest/gtest.h"

#include "chapter12/binary_tree.h"
#include "fixtures/tree_fixture.h"

TEST(Chapter12, tree) {
  Chapter12::BinaryTree<int> tree{4, 3, 2, 1, 7};
  std::cout << tree;
  tree.erase(2);
  std::cout << tree;
  tree.erase(1);
  std::cout << tree;
  tree.erase(4);
  std::cout << tree;
}

using MyTree = ::testing::Types<Chapter12::BinaryTree<int>>;

INSTANTIATE_TYPED_TEST_SUITE_P(BinaryTreeTest, TreeTest, MyTree);
