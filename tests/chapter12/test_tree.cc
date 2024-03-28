#include "gtest/gtest.h"

#include "chapter12/tree.h"

#include "tree_fixture.h"

INSTANTIATE_TYPED_TEST_SUITE_P(BinaryTreeTest, TreeTest, Chapter12::BinaryTree<int>);
