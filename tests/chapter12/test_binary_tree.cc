#include "gtest/gtest.h"

#include "chapter12/binary_tree.h"
#include "tree_fixture.h"

using MyTree = ::testing::Types<Chapter12::BinaryTree<int>>;

INSTANTIATE_TYPED_TEST_SUITE_P(BinaryTreeTest, TreeTest, MyTree);
