#include "gtest/gtest.h"

#include "chapter10/list.h"
#include "chapter10/xor_list.h"

#include "sequence_fixture.h"

using MyLists = ::testing::Types<Chapter10::List<int>, Chapter10::XORList<int>>;

INSTANTIATE_TYPED_TEST_SUITE_P(DoublyLinkedLists, SequenceTest, MyLists);
