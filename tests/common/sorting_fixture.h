#pragma once 

#include "gtest/gtest.h"
#include <vector>
#include <algorithm>
#include <random>

class SortingTest : public ::testing::TestWithParam<std::function<void(std::vector<int>&)>> {
 protected:
  // Common setup code if needed
  void SetUp() override {
    // Initialize random number generator
    std::random_device rd;
    generator.seed(rd());
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

  // Random number generator
  std::mt19937 generator;
  std::uniform_int_distribution<int> distribution{1, 100};
};