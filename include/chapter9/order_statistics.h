#pragma once

#include "chapter2/sorting.h"
#include "chapter7/partitions.h"

#include <vector>
#include <utility>

namespace Chapter9 {
  template <typename T, typename Compare = std::less<T>>
  T &minimum(std::vector<T> &vec, Compare comp = Compare());

  template <typename T, typename Compare = std::less<T>>
  T &maximum(std::vector<T> &vec, Compare comp = Compare());

  template <typename T, typename Compare = std::less<T>>
  std::pair<T&, T&> minMax(std::vector<T> &vec, Compare comp = Compare());

  template <typename T, typename Compare = std::less<T>>
  T &select(std::vector<T> &vec, int index, Compare comp = Compare());

  template <typename T, typename Compare = std::less<T>>
  T &worstCaseLinearSelect(std::vector<T> &vec, int index, Compare comp = Compare());

  int medianOfTwoSortedLists(const std::vector<int> &vec1, const std::vector<int> &vec2);

  // Helpers
  template <typename T, typename Compare = std::less<T>>
  T &selectHelper(std::vector<T> &vec, int index, int p, int r, Compare comp = Compare());

  template <typename T, typename Compare = std::less<T>>
  T &worstCaseLinearSelectHelper(std::vector<T> &vec, int index, int p, int r, Compare comp = Compare());


  // Definitions
  template <typename T, typename Compare>
  T &minimum(std::vector<T> &vec, Compare comp) {
    int minIndex = 0;
    for (int i = 0; i < vec.size(); ++i) {
      if (comp(vec[i], vec[minIndex])) minIndex = i;
    }
    return vec[minIndex];
  }

  template <typename T, typename Compare>
  T &maximum(std::vector<T> &vec, Compare comp) {
    return minimum(vec, [&comp](const T &a, const T &b) { return comp(b, a); });
  }

  template <typename T, typename Compare>
  std::pair<T&, T&> minMax(std::vector<T> &vec, Compare comp) {
    int n = vec.size();
    int parity = n % 2;
    int minIndex, maxIndex;
    
    if (parity == 0) {
      if (comp(vec[0], vec[1])) {
        minIndex = 0; maxIndex = 1;
      } else {
        minIndex = 1; maxIndex = 0;
      }
    } else {
      minIndex = maxIndex = 0;
    }

    for (int i = 1; i < (n+1) / 2; ++i) {
      int less = 2*i-parity; int greater = 2*i-parity+1;
      if (comp(vec[greater], vec[less])) std::swap(less, greater);
      if (comp(vec[less], vec[minIndex])) minIndex = less;
      if (comp(vec[maxIndex], vec[greater])) maxIndex = greater;
    }

    return std::pair<T&, T&>(vec[minIndex], vec[maxIndex]);
  }

  template <typename T, typename Compare>
  T &select(std::vector<T> &vec, int index, Compare comp) {
    return selectHelper(vec, index, 0, vec.size() - 1, comp);
  }

  template <typename T, typename Compare>
  T &selectHelper(std::vector<T> &vec, int index, int p, int r, Compare comp) {
    if (p == r) return vec[p];
    int q = Chapter7::randomPartition(vec, p, r, comp);
    int k = q - p;
    if (index == k) return vec[q];
    else if (index < k) {
      return selectHelper(vec, index, p, q-1, comp);
    } else {
      return selectHelper(vec, index-k-1, q+1, r, comp);
    }
  }

  template <typename T, typename Compare>
  T &worstCaseLinearSelect(std::vector<T> &vec, int index, Compare comp) {
    return worstCaseLinearSelectHelper(vec, index, 0, vec.size() - 1, comp);
  }

  template <typename T, typename Compare>
  T &worstCaseLinearSelectHelper(std::vector<T> &vec, int index, int p, int r, Compare comp) {
    if (p == r) return vec[p];
    int n = r - p + 1;
    int sections = n / 5;

    std::vector<int> medians(sections);
    for (int i = 0; i < sections; ++i) {
      int start = p + 5*i;
      Chapter2::rangeInsertionSort(vec, start, start + 4);
      medians[i] = vec[start + 2];
    }
    if (n % 5) {
      medians.push_back(vec[r]);
    }

    int medianOfMedians = worstCaseLinearSelect(medians, sections / 2, comp);

    int q = Chapter7::partitionByValue(vec, medianOfMedians, p, r, comp);

    int k = q - p;
    if (index == k) return vec[q];
    else if (index < k) {
      return worstCaseLinearSelectHelper(vec, index, p, q-1, comp);
    } else {
      return worstCaseLinearSelectHelper(vec, index-k-1, q+1, r, comp);
    }
  }

} // end namespace Chapter9
