#include "gtest/gtest.h"
#include "chapter2/searching.h"

TEST(Chapter2, LinearSearch1) {
  std::vector<int> arr = {1, 2, 4, 7, 10, 13, 20};
  std::vector<std::pair<int, int>> tests = {{1, 0}, {2, 1}, {10, 4}, {20, 6}, {30, -1}, {-1, -1}, {9, -1}};
  
  for (auto pair: tests) {
    int target = pair.first;
    int result = pair.second;
    int index = Chapter2::linearSearch(arr, target);
    EXPECT_EQ(index, result);
  }
}

TEST(Chapter2, LinearSearch2) {
  std::vector<char> arr = {'e', 'f', 'j', 'a', 'z', 'e'};
  std::vector<std::pair<char, int>> tests = {{'f', 1}, {'y', -1}, {'e', 0}};
  
  for (auto pair: tests) {
    char target = pair.first;
    int result = pair.second;
    int index = Chapter2::linearSearch(arr, target);
    EXPECT_EQ(index, result);
  }
}

// testing binarySearch function under assumption that linearSearch is correct, since they should behave the same 
// if lists are sorted
TEST(Chapter2, BinarySearch) {
  std::vector<int> arr = {1, 2, 4, 7, 10, 13, 20, 33, 50, 100, 200, 301};

  for (int i = 1; i < 10; ++i) {
    int targetIndex = (31 * i) % arr.size();
    int dummyValue = (31 * i) % 100;
    int target = arr[targetIndex];
    EXPECT_EQ(Chapter2::linearSearch(arr, target), Chapter2::binarySearch(arr, target));
    EXPECT_EQ(Chapter2::linearSearch(arr, dummyValue), Chapter2::binarySearch(arr, dummyValue));
  }
}