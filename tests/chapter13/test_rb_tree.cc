#include "gtest/gtest.h"

#include "chapter13/rb_tree.h"
#include "tree_fixture.h"

TEST(Chapter13, RBTREE) {
  Chapter13::RedBlackTree<int> tree{6, 3, 4, 2};
  tree.insert(12);
}

using MyRBTree = ::testing::Types<Chapter13::RedBlackTree<int>>;

INSTANTIATE_TYPED_TEST_SUITE_P(RedBlackTreeTest, TreeTest, MyRBTree);
