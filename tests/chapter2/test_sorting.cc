#include "gtest/gtest.h"
#include "chapter2/sorting.h"
#include "sorting_fixture.h"

TEST(Chapter2, Merge1) {
  std::vector<int> arr = {1, 2, 6, 7, 3, 4, 5, 8};
  std::vector<int> result = {1, 2, 3, 4, 5, 6, 7, 8};  
  Chapter2::merge(arr, 0, 4, 8);
  ASSERT_EQ(arr, result);
}

TEST(Chapter2, Merge2) {
  std::vector<int> arr = {10, 10, 1, 4, 7, 8, 2, 3, 5, 6, 10, 10};
  std::vector<int> result = {10, 10, 1, 2, 3, 4, 5, 6, 7, 8, 10, 10};
  Chapter2::merge(arr, 2, 6, 10);
  ASSERT_EQ(arr, result);
}

TEST(Chapter2, InsertionSort) {
  std::vector<int> arr = {5, 2, 4, 6, 1, 3};
  Chapter2::insertionSort(arr);
  ASSERT_EQ(arr, std::vector<int>({1, 2, 3, 4, 5, 6}));
}

TEST(Chapter2, MergeSort) {
  std::vector<int> arr = {5, 2, 4, 6, 1, 3, 9, 10, 8, 7};
  Chapter2::mergeSort(arr);
  ASSERT_EQ(arr, std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
}


TEST_P(SortingTest, SortTest) {
  auto sortingFunction = GetParam();
  for (int i = 0; i < 10; ++i) {
    std::vector<int> vec = generateRandomArray(100); // Generate a random array
    std::vector<int> copyVec = vec;
    std::sort(copyVec.begin(), copyVec.end());
    sortingFunction(vec);
    ASSERT_EQ(vec, copyVec);
    EXPECT_TRUE(isSorted(vec)); // Check if sorted
  }
}

// Provide sorting functions to be tested
INSTANTIATE_TEST_CASE_P(SortFunctions, SortingTest, ::testing::Values(
  Chapter2::insertionSort,
  Chapter2::selectionSort,
  Chapter2::mergeSort
  // Add more sorting functions here as needed
)); 