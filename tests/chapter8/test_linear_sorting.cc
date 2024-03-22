#include <gtest/gtest.h>

#include "chapter8/linear_sorting.h"

#include "random_generators.h"
#include "testing_helpers.h"
#include "sorting_fixture.h"

#include <vector>
#include <algorithm>
#include <functional>
#include <random>

GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(StableSortingTest);
GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(SortingTest);

TEST(Chapter8, BasicCountingSort) {
  std::vector<int> input{3, 2, 5, 4, 1, 1, 0, 0, 6, 5, 4, 3, 0, 0, 5, 4, 2, 2, 3};
  Chapter8::countingSort(input, 6);
  std::vector<int> expected{0, 0, 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6};
  EXPECT_EQ(input, expected);
}

TEST(Chapter8, CustomCountingSort) {
  auto moduloCompare = [](int x) { return x % 5; };
  std::vector<int> input{17, 22, 1, 15, 18, 34, 9, 10, 13};
  Chapter8::countingSort(input, 4, moduloCompare);
  std::vector<int> expected{15, 10, 1, 17, 22, 18, 13, 34, 9};
  EXPECT_EQ(input, expected);
}

TEST(Chapter8, RangeQueryTest) {
  int range = 10;
  int size = 50;
  RandomValue rand;
  for (int i = 0; i < 1000; ++i) {
    std::vector<int> input = generateRandomIntVector(size, range);
    Chapter8::RangeQuery rangeQuery{input, range};
    int low = rand(0, range);
    int high = rand(low, range);

    int manualCount = std::count_if(input.begin(), input.end(), [low, high](int x) { return low <= x && x <= high; });

    EXPECT_EQ(rangeQuery.query(low, high), manualCount);
  }
}

TEST(Chapter8, BasicBucketSort) {
  std::vector<double> input{0.78, 0.17, 0.39, 0.26, 0.72, 0.94, 0.21, 0.12, 0.23, 0.68};
  Chapter8::bucketSort(input);
  std::vector<double> expected{0.12, 0.17, 0.21, 0.23, 0.26, 0.39, 0.68, 0.72, 0.78, 0.94};
  EXPECT_EQ(input, expected);
}

TEST(Chapter8, CustomBucketSort) {
  std::random_device rd;
  std::mt19937 gen;
  gen.seed(rd());

  std::vector<std::pair<double, double>> uniform_ranges{{3, 10}, {0, 20}, {30, 100}};
  std::vector<double> exponential_lambdas{1, 2.5, 10};
  std::vector<std::pair<double, double>> normal_params{{5, 2}, {2.5, 5.5}, {0, 1.0}};
  std::vector<int> sizes{10, 100, 1000};
   
  for (int i = 0; i < 3; ++i) {
    double low, high;
    double lambda;
    double mean, stddev;

    std::tie(low, high) = uniform_ranges[i];
    lambda = exponential_lambdas[i];
    std::tie(mean, stddev) = normal_params[i];

    std::uniform_real_distribution<double> uniform{low, high};
    std::exponential_distribution<double> exponential{lambda};
    std::normal_distribution<double> normal{mean, stddev};

    std::function<double(double)> uniformCdf = [low, high](double x) {
      if (x < low) return 0.0;
      else if (x < high) return x / high;
      else return 1.0;
    };
    std::function<double(double)> exponentialCdf = [lambda](double x) {
      return 1 - std::exp(-1*lambda*x);
    };
    std::function<double(double)> normalCdf = [mean, stddev](double x) {
      return 0.5 * (1 + std::erf((x - mean) / (stddev * std::sqrt(2.0))));
    };

    std::vector<double> uniformInput(sizes[i]);
    std::vector<double> exponentialInput(sizes[i]);
    std::vector<double> normalInput(sizes[i]);

    std::generate_n(uniformInput.begin(), sizes[i], [&uniform, &gen]() { return uniform(gen); });
    std::generate_n(exponentialInput.begin(), sizes[i], [&exponential, &gen]() { return exponential(gen); });
    std::generate_n(normalInput.begin(), sizes[i], [&normal, &gen]() { return normal(gen); });

    Chapter8::bucketSort(uniformInput, uniformCdf);
    Chapter8::bucketSort(exponentialInput, exponentialCdf);
    Chapter8::bucketSort(normalInput, normalCdf);

    EXPECT_TRUE(std::is_sorted(uniformInput.begin(), uniformInput.end()));
    EXPECT_TRUE(std::is_sorted(exponentialInput.begin(), exponentialInput.end()));
    EXPECT_TRUE(std::is_sorted(normalInput.begin(), normalInput.end()));
  }
}


// TODO: Fix this. It doesnt make sense to be sorting integers using bucketSort, since only integer
// indexed buckets will be used. Fix sorting_fixture to be more dynamic and allow doubles
std::function<double(int)> uniformBucketCDF = [](int x) {
  if (x < 0) return static_cast<double>(0);
  else if (x <= 100) return static_cast<double>(x) / 100;
  else return static_cast<double>(1);
};

INSTANTIATE_TEST_SUITE_P(LinearSorts, SortingTest, ::testing::Values(
  [](std::vector<int> &vec, std::less<int> lessThan) { Chapter8::countingSort(vec, 100); },
  [](std::vector<int> &vec, std::less<int> lessThan) { Chapter8::bucketSort(vec, uniformBucketCDF); },
  [](std::vector<int> &vec, std::less<int> lessThan) { Chapter8::radixSort(vec); }
));
