#include <gtest/gtest.h>

#include "chapter9/order_statistics.h"

#include "helpers/random_generators.h"
#include "helpers/printing_helpers.h"

#include <vector>
#include <algorithm>
#include <functional>
#include <random>
#include <string>

TEST(Chapter9, BasicMinMax) {
  std::vector<int> input{6, 7, 3, 2, 9, 1, 3, 5};
  std::pair<int, int> minMax = Chapter9::minMax(input);
  int &min = Chapter9::minimum(input);
  EXPECT_EQ(min, 1);
  EXPECT_EQ(minMax.first, 1);
  int max = Chapter9::maximum(input);
  EXPECT_EQ(max, 9);
  EXPECT_EQ(minMax.second, 9);
  min = 100;
  max = Chapter9::maximum(input);
  EXPECT_EQ(max, 100);
}

TEST(Chapter9, CustomMinMax) {
  std::vector<std::string> input{"hi", "bye", "A", "long_string", "end"};
  std::pair<std::string&, std::string&> minAndMax = Chapter9::minMax(input, [](const std::string &a, const std::string &b) { return a.size() < b.size(); });
  minAndMax.first = "MIN";
  minAndMax.second = "MAX";
  std::vector<std::string> expected{"hi", "bye", "MIN", "MAX", "end"};
  EXPECT_EQ(input, expected);
}

TEST(Chapter9, ValidMinMax) {
  for (int i = 0; i < 10; ++i) {
    std::vector<int> input = generateRandomIntVector(50, 100);
    int myMin = Chapter9::minimum(input);
    int myMax = Chapter9::maximum(input);
    std::pair<int, int> myMinMax = Chapter9::minMax(input);
    int realMin = *(std::min_element(input.begin(), input.end()));
    int realMax = *(std::max_element(input.begin(), input.end()));
    EXPECT_EQ(myMin, realMin);
    EXPECT_EQ(myMax, realMax);
    EXPECT_EQ(myMinMax.first, realMin);
    EXPECT_EQ(myMinMax.second, realMax);
  }
}

TEST(Chapter9, BasicSelect) {
  std::vector<int> input{6, 0, 3, 7, 3, 2, 17, 9, 1, 3, 5, 12};
  int third = Chapter9::select(input, 2);
  int fifth = Chapter9::select(input, 4);
  int ninth = Chapter9::select(input, 8);
  EXPECT_EQ(third, 2);
  EXPECT_EQ(fifth, 3);
  EXPECT_EQ(ninth, 7);
}

TEST(Chapter9, ValidSelects) {
  int size = 10;
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_int_distribution<int> dist{0, size-1};

  std::vector<int> input1(size);
  std::vector<int> input2(size);
  for (int i = 0; i < 10; ++i) {
    std::iota(input1.begin(), input1.end(), 0);
    std::iota(input2.begin(), input2.end(), 0);
    std::shuffle(input1.begin(), input1.end(), gen);
    std::shuffle(input2.begin(), input2.end(), gen);
    int index = dist(gen);
    EXPECT_EQ(Chapter9::select(input1, index), index);
    EXPECT_EQ(Chapter9::worstCaseLinearSelect(input1, index), index);
  }
}

TEST(Chapter9, BasicMedianTwoSorted) {
  std::vector<int> input1{1, 4, 5, 6, 8, 11, 23, 35};
  std::vector<int> input2{4, 7, 9, 10, 20, 21, 33, 44};
  int medianOfTwo = Chapter9::medianOfTwoSortedLists(input1, input2);
  EXPECT_EQ(medianOfTwo, 9);
}

TEST(Chapter9, ValidMedianTwoSorted) {
  for (int i = 0; i < 10; ++i) {
    int size = (i+1)*10; int range = (i+1)*20;
    std::vector<int> input1 = generateRandomIntVector(size, range);
    std::sort(input1.begin(), input1.end());
    std::vector<int> input2 = generateRandomIntVector(size, range);
    std::sort(input2.begin(), input2.end());

    int medianOfTwo = Chapter9::medianOfTwoSortedLists(input1, input2);

    input1.insert(input1.end(), input2.begin(), input2.end());
    std::sort(input1.begin(), input1.end());
    int manualMedian = input1[(2*size - 1) / 2];

    EXPECT_EQ(medianOfTwo, manualMedian);
  }
}
