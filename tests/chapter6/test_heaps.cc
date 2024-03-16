#include "gtest/gtest.h"

#include "chapter6/heaps.h"
#include "random_generators.h"
#include "sorting_fixture.h"

GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(StableSortingTest);

TEST(Chapter6, SimpleHeapify) {
  std::vector<int> vec1{2, 1, 3};
  std::vector<int> vec2{5, 4, 3};
  std::vector<int> vec3{2, 6, 5, 3, 4};
  Chapter6::heapify(vec1, 3, 0);
  Chapter6::heapify(vec2, 3, 0);
  Chapter6::heapify(vec3, 5, 0);
  std::vector<int> result1{3, 1, 2};
  std::vector<int> result2{5, 4, 3};
  std::vector<int> result3{6, 4, 5, 3, 2};
  EXPECT_EQ(vec1, result1); 
  EXPECT_EQ(vec2, result2);
  EXPECT_EQ(vec3, result3);
}

TEST(Chapter6, SimpleBuildHeap) {
  std::vector<int> vec{1, 2, 6, 3};
  Chapter6::buildHeap(vec);
  std::vector<int> result{6, 3, 1, 2};
  EXPECT_EQ(vec, result);
  EXPECT_TRUE(std::is_heap(result.begin(), result.end()));
}

TEST(Chapter6, RandomBuildHeap) {
  for (int i = 0; i < 5; ++i) {
    std::vector<int> vec = generateRandomIntVector(20, 100);
    std::vector<int> copy = vec;
    Chapter6::buildHeap(vec);
    std::make_heap(copy.begin(), copy.end());
    EXPECT_EQ(vec, copy);
  }
}

TEST(Chapter6, SimpleBubbleUp) {
  std::vector<int> vec1{6, 5, 4, 7};
  std::vector<int> vec2{9, 8, 7, 3, 4, 12, 6};
  std::vector<int> vec3{12, 3, 6};
  Chapter6::bubbleUp(vec1, 3);
  Chapter6::bubbleUp(vec2, 5);
  Chapter6::bubbleUp(vec3, 0);
  std::vector<int> result1{7, 6, 4, 5};
  std::vector<int> result2{12, 8, 9, 3, 4, 7, 6};
  std::vector<int> result3{12, 3, 6};
  EXPECT_EQ(vec1, result1); 
  EXPECT_EQ(vec2, result2);
  EXPECT_EQ(vec3, result3);
}

TEST(Chapter6, HeapContainer) {
  Chapter6::MaxHeap<int> heap;

  EXPECT_THROW(heap.extractPeak(), std::out_of_range);

  std::vector<int> entries{17, 8, 9, 4, 12, 6};
  for (auto val : entries) {
    heap.insert(val);
  }
  std::vector<int> result;
  for (int i = 0; i < 4; ++i) {
    result.push_back(heap.extractPeak());
  }

  std::vector<int> expected{17, 12, 9, 8};
  EXPECT_EQ(result, expected);
  EXPECT_EQ(heap.size(), 2);
  heap.extractPeak(); heap.extractPeak();
  EXPECT_TRUE(heap.empty());
}

TEST(Chapter6, HeapConstructor) {
  Chapter6::MaxHeap<int> heap{{20, 9, 7, 5, 17, 25, 32}};
  EXPECT_EQ(heap.size(), 7);

  std::vector<int> result;
  for (int i = 0; i < 5; ++i) {
    result.push_back(heap.extractPeak());
  }
  std::vector<int> expected{32, 25, 20, 17, 9};
  EXPECT_EQ(result, expected);
  EXPECT_EQ(heap.size(), 2);
}

INSTANTIATE_TEST_SUITE_P(HeapSort, SortingTest, ::testing::Values(
  Chapter6::heapSort<int>
));