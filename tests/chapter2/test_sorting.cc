#include "gtest/gtest.h"
#include "chapter2/sorting.h"
#include "tests/common/sorting_fixture.h"

TEST(Chapter2, Merge) {
  std::vector<int> arr = {1, 2, 6, 7, 3, 4, 5, 8};
  std::vector<int> result = {1, 2, 3, 4, 5, 6, 7, 8};  
  Chapter2::merge(arr, 0, 4, 8);
  ASSERT_EQ(arr, result);
}

TEST(Chapter2, InsertionSort) {
  std::vector<int> arr = {5, 2, 4, 6, 1, 3};
  Chapter2::insertionSort(arr);
  ASSERT_EQ(arr, std::vector<int>({1, 2, 3, 4, 5, 6}));
}

TEST_P(SortingTest, SortTest) {
    std::vector<int> vec = generateRandomArray(100); // Generate a random array
    auto sortingFunction = GetParam();
    sortingFunction(vec);
    EXPECT_TRUE(isSorted(vec)); // Check if sorted
}

// Provide sorting functions to be tested
INSTANTIATE_TEST_CASE_P(SortFunctions, SortingTest, ::testing::Values(
    Chapter2::mergeSort
    // Add more sorting functions here as needed
)); 