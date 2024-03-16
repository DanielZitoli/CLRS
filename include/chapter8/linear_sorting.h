#pragma once

#include "chapter8/linear_sorting.h"
#include "chapter2/sorting.h"

#include <vector>
#include <functional>
#include <type_traits>
#include <cmath>
#include <numeric>
#include <iostream>

namespace Chapter8 {
  void countingSort(std::vector<int> &vec, int k, std::function<int(int)> transform = [](int x) { return x; });

  class RangeQuery {
    int range; // vec contains int in range [0, range]
    std::vector<int> rangeInfo; // (# of elements <= i) == (rangeInfo[i])
   public:
    RangeQuery(const std::vector<int> &vec, int k);
    int query(int low, int high);
  };

  void radixSort(std::vector<int> &vec);

  template <typename T>
  void bucketSort(std::vector<T> &vec, std::function<double(T)> cdf = [](T x) { return x; });

  // Helper
  template <typename T>
  void printBucketStats(const std::vector<std::vector<T>> &buckets);


  // Definitions

  // Bucket sort by default accepts input in [0, 1]
  // For custom usage, provide a CDF function that maps T -> [0, 1] which is meant to be used
  // as if the input to the vec array were drawn from a distribution given by the cdf
  template <typename T>
  void bucketSort(std::vector<T> &vec, std::function<double(T)> cdf) {
    static_assert(std::is_arithmetic<T>::value, "bucketSort template type must be of numeric type");
    int n = vec.size();
    std::vector<std::vector<T>> buckets(n + 1, std::vector<T>{});
    for (auto x : vec) buckets[static_cast<int>(floor(n*cdf(x)))].push_back(x);
    for (auto &bucket : buckets) Chapter2::insertionSort(bucket);
    // printBucketStats(buckets);
    int progress = 0;
    for (auto bucket : buckets) {
      int bucketSize = bucket.size();
      std::copy_n(bucket.begin(), bucketSize, vec.begin()+progress);
      progress += bucketSize;
    }
  }

  template <typename T>
  void printBucketStats(const std::vector<std::vector<T>> &buckets) {
    std::cout << "Number of buckets: " << buckets.size() << std::endl;
    auto maxBucket = std::max_element(buckets.begin(), buckets.end(), [](const std::vector<T> &bucket1, const std::vector<T> &bucket2) { return bucket1.size() < bucket2.size(); });
    std::cout << "Largest bucket: " << (*maxBucket).size() << std::endl;
    std::cout << "Number of buckets with size >= 2: " << std::count_if(buckets.begin(), buckets.end(), [](const std::vector<T> &bucket) { return bucket.size() >= 2; }) << std::endl;
    std::cout << "Number of buckets with size >= 3: " << std::count_if(buckets.begin(), buckets.end(), [](const std::vector<T> &bucket) { return bucket.size() >= 3; }) << std::endl;
    int squaredSum = std::accumulate(buckets.begin(), buckets.end(), 0, [](int a, const std::vector<T> &b) { return a + std::pow(b.size(), 2);});
    std::cout << "Sum of N^2 for each bucket: " << squaredSum << " --- " << static_cast<double>(squaredSum) / buckets.size()  << std::endl;
  }
}