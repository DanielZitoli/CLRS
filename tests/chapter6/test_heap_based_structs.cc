#include "gtest/gtest.h"

#include "chapter6/heap_based_structs.h"
#include "sorting_fixture.h"
#include "random_generators.h"

GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(StableSortTest);

TEST(Chapter6, SimpleStack) {
  Chapter6::HeapStack<int> s;
  Chapter6::HeapQueue<int> q;

  for (auto x : std::vector<int>{3, 8, 5, 9, 12}) {
    s.push(x);
    q.push(x);
  }
  for (auto x : std::vector<int>{12, 9, 5}) {
    EXPECT_EQ(s.top(), x);
    s.pop();
  }
  for (auto x : std::vector<int>{3, 8, 5, 9}) {
    EXPECT_EQ(q.front(), x);
    q.pop();
  }

  EXPECT_EQ(s.size(), 2);
  EXPECT_EQ(q.size(), 1);
}

TEST(Chapter6, YoungArraySimple) {
  Chapter6::YoungArray youngArray(10, 10);
  std::vector<int> input{30, 22, 17, 8, 25, 6, 12};
  for (auto x : input) {
    youngArray.insert(x);
  }
  std::vector<int> result;
  for (int i = 0; i < 4; ++i) {
    result.push_back(youngArray.extractMin());
  }
  std::vector<int> expected{6, 8, 12, 17};
  EXPECT_EQ(result, expected);
}

TEST(Chapter6, YoungArrayConstructor) {
  Chapter6::YoungArray youngArray{{20, 12, 8, 27, 31, 18, 9, 7, 1, 40}};

  std::vector<int> result;
  for (int i = 0; i < 7; ++i) {
    result.push_back(youngArray.extractMin());
  }
  std::vector<int> expected{1, 7, 8, 9, 12, 18, 20};
  EXPECT_EQ(result, expected);
}


TEST(Chapter6, MergeVectors) {
  std::vector<std::vector<int>> input{{1, 4, 7}, {2, 5, 8}, {3, 6, 9}};
  std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8, 9};

  EXPECT_EQ(Chapter6::mergeVectors(input), expected);
}


INSTANTIATE_TEST_SUITE_P(YoungSort, SortingTest, ::testing::Values(
  Chapter6::youngSort<int>
));