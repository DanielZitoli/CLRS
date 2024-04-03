#include "gtest/gtest.h"

#include "chapter7/quick_sort.h"
#include "fixtures/sorting_fixture.h"
#include "helpers/printing_helpers.h"

#include <random>
#include <algorithm>

GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(StableSortingTest);

INSTANTIATE_TEST_SUITE_P(QuickSorts, SortingTest, ::testing::Values(
  Chapter7::quickSort<int>,
  Chapter7::randomQuickSort<int>
));

// TEST(Chapter7, FuzzySort) {
//   // Random generators / distributions
//   std::random_device rd;
//   std::mt19937 gen;
//   gen.seed(rd());
//   std::uniform_int_distribution<int> range(1, 100);
//   std::uniform_int_distribution<int> size(5, 40);

//   // Comparison function for intervals
//   auto intervalComp = [](const std::pair<int, int> &a, const std::pair<int, int> &b) {
//     return a.second < b.first;
//   };

//   for (int i = 1; i < 10; ++i) {
//     std::vector<std::pair<int, int>> intervals(5 * i);
//     std::generate(intervals.begin(), intervals.end(), [&range, &size, &gen](){ 
//       int start = range(gen);
//       int diff = size(gen);
//       return std::pair<int, int>(start, start+diff);
//     });
//     std::vector<std::pair<int, int>> copy = intervals;
//     // std::cout << "Before: ";
//     // printVec(intervals);
//     Chapter7::fuzzySort(intervals);
//     // std::cout << "After: ";
//     // printVec(intervals);
//     EXPECT_TRUE(std::is_permutation(intervals.begin(), intervals.end(), copy.begin()));
//     // EXPECT_TRUE(std::is_sorted(intervals.begin(), intervals.end(), intervalComp));
//   }
// }
