#pragma once

#include "chapter7/partitions.h"

#include <vector>
#include <functional>
#include <utility>
#include <iostream>

namespace Chapter7 {
  template <typename T, typename Compare = std::less<T>>
  void quickSort(std::vector<T> &vec, Compare comp = Compare());

  template <typename T, typename Compare = std::less<T>>
  void randomQuickSort(std::vector<T> &vec, Compare comp = Compare());

  template <typename T, typename Compare = std::less<T>>
  void fuzzySort(std::vector<std::pair<T, T>> &vec, Compare comp = Compare());

  // Helpers
  template <typename T, typename Compare>
  void quickSortHelper(std::vector<T> &vec, int p, int r, const Compare &comp);

  template <typename T, typename Compare>
  void randomQuickSortHelper(std::vector<T> &vec, int p, int r, const Compare &comp);

  template <typename T, typename Compare, typename CompareValue>
  void fuzzySortHelper(std::vector<std::pair<T, T>> &vec, int p, int r, const Compare &comp, const CompareValue &compareLeft, const CompareValue &compareRight);

  template <typename T, typename Compare>
  std::pair<T, T> overlapping(std::vector<std::pair<T, T>> &vec, int p, int r, const Compare &comp);


  //// DEFINITIONS ////

  template <typename T, typename Compare>
  void quickSort(std::vector<T> &vec, Compare comp) {
    quickSortHelper(vec, 0, vec.size() - 1, comp);
  }

  template <typename T, typename Compare>
  void quickSortHelper(std::vector<T> &vec, int p, int r, const Compare &comp) {
    if (p < r) {
      int q = partition(vec, p, r, comp);
      quickSortHelper(vec, p, q - 1, comp);
      quickSortHelper(vec, q + 1, r, comp); 
    }
  }

  template <typename T, typename Compare>
  void randomQuickSort(std::vector<T> &vec, Compare comp) {
    randomQuickSortHelper(vec, 0, vec.size() - 1, comp);
  }

  template <typename T, typename Compare>
  void randomQuickSortHelper (std::vector<T> &vec, int p, int r, const Compare &comp) {
    if (p < r) {
      int q = randomPartition(vec, p, r, comp);
      randomQuickSortHelper(vec, p, q - 1, comp);
      randomQuickSortHelper(vec, q + 1, r, comp); 
    }
  }

  // template <typename T, typename Compare>
  // void fuzzySort(std::vector<std::pair<T, T>> &vec, Compare comp) {
  //   auto intervalComp = [&comp](const std::pair<T, T> &a, const std::pair<T, T> &b){
  //     return comp(a.second, b.first); // a is less than b if its fully before it, not overlapping
  //   };
  //   auto overlapComp = [&comp](const std::pair<T, T> &a, const std::pair<T, T> &b){
  //     return !comp(b.first, a.first) && !comp(a.second, b.first); // a.first <= b.first <= a.second
  //   };
  //   fuzzySortHelper(vec, 0, vec.size() - 1, intervalComp, overlapComp);
  // }

  // template <typename T, typename CompareIntervals, typename CompareOverlaps>
  // void fuzzySortHelper(std::vector<std::pair<T, T>> &vec, int p, int r, const CompareIntervals &intervalComp, const CompareOverlaps &overlapComp) {
  //   if (p < r) {
  //     int less, equal;
  //     int leftOverlap, rightOverlap;
  //     std::tie(less, equal) = threeWayPartition(vec, p, r, intervalComp);
  //     std::cout << "Pivots: [" << less << ", " << equal << "] " << std::endl;
  //     fuzzySortHelper(vec, p, less, intervalComp, overlapComp);
  //     fuzzySortHelper(vec, equal+1, r, intervalComp, overlapComp); 
  //     std::tie(leftOverlap, rightOverlap) = threeWayPartition(vec, less+1, equal, overlapComp);
  //     fuzzySortHelper(vec, leftOverlap+1, rightOverlap, intervalComp, overlapComp);
  //   }
  // }

  template <typename T, typename Compare>
  std::pair<T, T> overlapping(std::vector<std::pair<T, T>> &vec, int p, int r, const Compare &comp) {
    std::pair<T, T> endpoints = vec[r];
    for (int i = p; i < r; ++i) {
      if (!comp(endpoints.second, vec[i].first) && !comp(vec[i].second, endpoints.first)) {
        endpoints.first = std::max(endpoints.first, vec[i].first);
        endpoints.second = std::min(endpoints.second, vec[i].second);
      }
    }
    return endpoints;
  }

  template <typename T, typename Compare>
  void fuzzySort(std::vector<std::pair<T, T>> &vec, Compare comp) {
    std::function<bool(T, std::pair<T, T>)> compareLeft = [&comp](const T &a, const std::pair<T, T> &b){
      return comp(a, b.first); 
    };
    std::function<bool(T, std::pair<T, T>)> compareRight = [&comp](const T &a, const std::pair<T, T> &b){
      return comp(a, b.second);
    };
    fuzzySortHelper(vec, 0, vec.size() - 1, comp, compareLeft, compareRight);
  }

  template <typename T, typename Compare>
  int partitionLeft(std::vector<std::pair<T, T>> &vec, const T &pivot, int p, int r, const Compare &comp) {
    int i = p - 1;
    for (int j = p; j <= r; ++j) {
      if (comp(vec[j].second, pivot)) {
        ++i;
        std::swap(vec[i], vec[j]);
      }
    }
    return i;
  }

  template <typename T, typename Compare, typename CompareValue>
  void fuzzySortHelper(std::vector<std::pair<T, T>> &vec, int p, int r, const Compare &comp, const CompareValue &compareLeft, const CompareValue &compareRight) {
    if (p < r) {
      std::pair<T, T> interval = overlapping(vec, p, r, comp);
      int q = partitionByValue(vec, interval.first, p, r, compareLeft);
      int t = partitionLeft(vec, interval.second, p, q, comp);
      // int t = partitionByValue(vec, interval.second, p, q, compareRight);
      fuzzySortHelper(vec, p, t-1, comp, compareLeft, compareRight);
      fuzzySortHelper(vec, q+1, r, comp, compareLeft, compareRight);
    }
  }

}