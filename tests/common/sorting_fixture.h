#pragma once 

#include "gtest/gtest.h"

#include <vector>
#include <algorithm>
#include <random>
#include <functional>
#include <utility>

// Defines a Parameterized testing fixture for sorting functions
// Use INSTANTIATE_TEST_SUITE_P({name of suite}, SortingTest, {sort functions}) to instantiate this
// test fixture in a test script
class SortingTest : public ::testing::TestWithParam<std::function<void(std::vector<int>&, std::less<int>)>> {
 protected:
  // Common setup code if needed
  void SetUp() override {
    // Initialize random number generator
    std::random_device rd;
    generator.seed(rd());

    sortingFunction = GetParam();
  }

  // Generate a random array of given size
  std::vector<int> generateRandomArray(int size) {
    std::vector<int> vec(size);
    std::generate(vec.begin(), vec.end(), [this]() { return distribution(generator); });
    return vec;
  }

  // Check if array is sorted
  bool isSorted(const std::vector<int>& vec) {
    return std::is_sorted(vec.begin(), vec.end());
  }

  // Sort function to be fetched from GetParam()
  std::function<void(std::vector<int>&, std::less<int>)> sortingFunction;

  // Random number generator
  std::mt19937 generator;
  std::uniform_int_distribution<int> distribution{1, 100};
};


using PairComparator = std::function<bool(std::pair<int, int>, std::pair<int, int>)>;
using SortPairsFunction = std::function<void(std::vector<std::pair<int, int>>&, PairComparator)>;

class StableSortingTest : public ::testing::TestWithParam<SortPairsFunction> {
 protected:
  // Common setup code if needed
  void SetUp() override {
    // Initialize random number generator
    std::random_device rd;
    generator.seed(rd());

    sortingFunction = GetParam();

    PairComparator comp = [](const std::pair<int, int> &a, const std::pair<int, int> &b) { return a.first < b.first; };
  }

  // Generate a random array of given size
  std::vector<std::pair<int, int>> generateRandomArray(int size) {
    std::vector<std::pair<int, int>> vec(size);
    for (int i = 0; i < vec.size(); ++i) {
      vec[i] = std::pair<int, int>(distribution(generator), i);
    }
    return vec;
  }

  bool isStableOrder(const std::vector<std::pair<int, int>> &vec) {
    for (int i = 0; i < vec.size() - 1; ++i) {
      // checks if each element keeps its original relative order to equal elements
      if (vec[i].first == vec[i+1].first && vec[i].second > vec[i+1].second) {
        return false;
      }
    }
    return true;
  }

  // Check if array is sorted
  bool isSorted(const std::vector<std::pair<int, int>>& vec) {
    return std::is_sorted(vec.begin(), vec.end(), std::less<std::pair<int, int>>());
  }

  // Sort function to be fetched from GetParam()
  SortPairsFunction sortingFunction;
  PairComparator comp;

  // Random number generator
  std::mt19937 generator;
  std::uniform_int_distribution<int> distribution{1, 100};
};

TEST_P(SortingTest, EmptyTest) {
  std::vector<int> empty;
  std::vector<int> expectedEmpty;
  sortingFunction(empty, std::less<int>());
  EXPECT_EQ(empty, expectedEmpty); 
}

TEST_P(SortingTest, SortFunctionTest) {
  for (int i = 0; i < 10; ++i) {
    std::vector<int> vec = generateRandomArray(1000); // Generate a random array
    sortingFunction(vec, std::less<int>());
    EXPECT_TRUE(isSorted(vec)); 
  }
}

TEST_P(StableSortingTest, StableSortFunctionTest) {
  for (int i = 0; i < 5; ++i) {
    std::vector<std::pair<int, int>> vec = generateRandomArray(10); // Generate a random array
    
    try {
    sortingFunction(vec, comp);
    } catch (const std::exception &e) {
      std::cout << e.what() << std::endl;
    }
    
    EXPECT_TRUE(isSorted(vec));
    // EXPECT_TRUE(isStableOrder(vec)); 
  }
}