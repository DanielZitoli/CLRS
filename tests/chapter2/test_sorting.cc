#include "gtest/gtest.h"
#include "chapter2/sorting.h"
#include "sorting_fixture.h"
#include <functional>
#include <utility>

TEST(Chapter2, Merge1) {
  std::vector<int> arr = {1, 2, 6, 7, 3, 4, 5, 8};
  std::vector<int> result = {1, 2, 3, 4, 5, 6, 7, 8};  
  Chapter2::merge(arr, 0, 4, 8);
  EXPECT_EQ(arr, result);
}

TEST(Chapter2, Merge2) {
  std::vector<int> arr = {10, 10, 1, 4, 7, 8, 2, 3, 5, 6, 10, 10};
  std::vector<int> result = {10, 10, 1, 2, 3, 4, 5, 6, 7, 8, 10, 10};
  Chapter2::merge(arr, 2, 6, 10);
  EXPECT_EQ(arr, result);
}

TEST(Chapter2, InsertionSort) {
  std::vector<int> arr = {5, 2, 4, 6, 1, 3};
  Chapter2::insertionSort(arr);
  EXPECT_EQ(arr, std::vector<int>({1, 2, 3, 4, 5, 6}));
}

TEST(Chapter2, RangeInsertionSort) {
  std::vector<int> arr = {5, 2, 8, 4, 1, 0, 9, 3};
  Chapter2::rangeInsertionSort(arr, 2, 6);
  EXPECT_EQ(arr, std::vector<int>({5, 2, 0, 1, 4, 8, 9, 3}));
}

TEST(Chapter2, SelectionSort) {
  std::vector<int> arr = {5, 2, 4, 6, 1, 3};
  Chapter2::selectionSort(arr);
  EXPECT_EQ(arr, std::vector<int>({1, 2, 3, 4, 5, 6}));
}

TEST(Chapter2, MergeSort) {
  std::vector<int> arr = {5, 2, 4, 6, 1, 3, 9, 10, 8, 7};
  Chapter2::mergeSort(arr);
  EXPECT_EQ(arr, std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
}

TEST(Chapter2, EvenAndOdds) {
  std::vector<int> arr = {3, 1, 3, 7, 2, 6, 2, 4};
  std::function<bool(int, int)> evensBeforeOdds = [](int a, int b) { return (a%2 == b%2)?  a < b: a%2 < b%2; };
  Chapter2::mergeSort(arr, evensBeforeOdds);
  EXPECT_EQ(arr, std::vector<int>({2, 2, 4, 6, 1, 3, 3, 7}));
}

TEST(Chapter2, SortByProductAndSum) {
  std::vector<std::pair<int, int>> arr = {{3, 2}, {5, 5}, {12, 2}, {2, 7}, {1, 2}, {9, 4}};
  std::function<bool(std::pair<int, int>, std::pair<int, int>)> 
    incProducts = [](std::pair<int, int> a, std::pair<int, int> b) { return (a.first * a.second) < (b.first * b.second); };
  std::function<bool(std::pair<int, int>, std::pair<int, int>)> 
    incSums = [](std::pair<int, int> a, std::pair<int, int> b) { return (a.first + a.second) < (b.first + b.second); };

  std::vector<std::pair<int, int>> productArr = {{1, 2}, {3, 2}, {2, 7}, {12, 2}, {5, 5}, {9, 4}};
  std::vector<std::pair<int, int>> sumArr = {{1, 2}, {3, 2}, {2, 7}, {5, 5}, {9, 4}, {12, 2}};
  Chapter2::mergeSort(arr, incProducts);
  EXPECT_EQ(arr, productArr);
  Chapter2::mergeSort(arr, incSums);
  EXPECT_EQ(arr, sumArr);
}

// Provide sorting functions to be tested
INSTANTIATE_TEST_SUITE_P(SortFunctions, SortingTest, ::testing::Values(
  Chapter2::insertionSort<int>,
  Chapter2::selectionSort<int>,
  Chapter2::mergeSort<int>
)); 

INSTANTIATE_TEST_SUITE_P(StableSortFunctions, StableSortingTest, ::testing::Values(
  Chapter2::insertionSort<std::pair<int, int>, PairComparator>,
  Chapter2::selectionSort<std::pair<int, int>, PairComparator>,
  Chapter2::mergeSort<std::pair<int, int>, PairComparator>
)); 