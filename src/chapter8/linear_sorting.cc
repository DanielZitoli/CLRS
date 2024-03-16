#include "chapter8/linear_sorting.h"

namespace Chapter8 {

  void countingSort(std::vector<int> &vec, int k, std::function<int(int)> transform) {
    std::vector<int> copy = vec;
    std::vector<int> count(k+1, 0);
    
    // count[i] holds # of elements == i
    for (auto x : vec) ++count[transform(x)];
    // count[i] holds # of elements <= i
    for (int i = 1; i <= k; ++i) count[i] += count[i-1];
    // Iterate backwards through vector and insert all equal elements in their place
    // in reverse order to keep stability
    for (int i = vec.size()-1; i >= 0; --i) {
      int transformedValue = transform(copy[i]);
      vec[count[transformedValue] - 1] = copy[i];
      --count[transformedValue];
    }
  }

  void radixSort(std::vector<int> &vec) {
    if (vec.empty()) return;

    int maxNumber = *(std::max_element(vec.begin(), vec.end()));
    
    // sorts each digit from least -> most significant
    // (x / exp) % 10 captures the nth digit from right if exp = 10^(n-1)
    for (int exp = 1; maxNumber / exp > 0; exp *= 10) 
      countingSort(vec, 10, [exp](int x) { return (x / exp) % 10; });
  }

  RangeQuery::RangeQuery(const std::vector<int> &vec, int k) : range{k}, rangeInfo{std::vector<int>(range+1, 0)} {
    for (auto x : vec) {
      ++rangeInfo[x];
    }
    for (int i = 1; i <= range; ++i) {
      rangeInfo[i] += rangeInfo[i-1];
    }
  }

  int RangeQuery::query(int low, int high) {
    return rangeInfo[high] - ((low == 0) ? 0 : rangeInfo[low-1]);
  }
}