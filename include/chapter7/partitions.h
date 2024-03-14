#pragma once

#include <vector>
#include <functional>

#include "chapter5/random.h"



namespace Chapter7 {
  template <typename T, typename U = T, typename Compare = std::less<T>>
  int partitionByValue(std::vector<T> &vec, const U &pivot, int p, int r, Compare comp = Compare());

  template <typename T, typename Compare = std::less<T>>
  int partition(std::vector<T> &vec, int p, int r, Compare comp = Compare());

  template <typename T, typename Compare = std::less<T>>
  int randomPartition(std::vector<T> &vec, int p, int r, Compare comp = Compare());

  template <typename T, typename Compare = std::less<T>>
  int medianOfThreePartition(std::vector<T> &vec, int p, int r, Compare comp = Compare());

  template <typename T, typename Compare = std::less<T>>
  std::pair<int, int> threeWayPartitionByValue(std::vector<T> &vec, const T &pivot, int p, int r, Compare comp = Compare());

  template <typename T, typename Compare = std::less<T>>
  std::pair<int, int> threeWayPartition(std::vector<T> &vec, int p, int r, Compare comp = Compare());

  template <typename T, typename Compare = std::less<T>>
  std::pair<int, int> randomThreeWayPartition(std::vector<T> &vec, int p, int r, Compare comp = Compare());

  // Helper 
  template <typename T, typename Compare = std::less<T>>
  T &median(T &a, T &b, T &c, const Compare &comp);


  //// DEFINITIONS ////

  template <typename T, typename U, typename Compare>
  int partitionByValue(std::vector<T> &vec, const U &pivot, int p, int r, Compare comp) {
    int q = p-1;
    // Invariant: vec[0...q] <= pivot, vec[q...i] > pivot
    for (int i = p; i <= r; ++i) {
      if (!comp(pivot, vec[i])) { // if pivot >= vec[i] i.e. (! (pivot < vec[i]))
        ++q;
        std::swap(vec[q], vec[i]);
      }
    }
    return q;
  }

  template <typename T, typename Compare>
  int partition(std::vector<T> &vec, int p, int r, Compare comp) {
    return partitionByValue(vec, vec[r], p, r, comp);
  }

  template <typename T, typename Compare>
  int randomPartition(std::vector<T> &vec, int p, int r, Compare comp) {
    Chapter5::Random rand;
    std::swap(vec[r], vec[rand(p, r)]);
    return partition(vec, p, r, comp);
  }

  template <typename T, typename Compare>
  T &median(T &a, T &b, T &c, const Compare &comp) {
    if (comp(a, b)) {
      if (comp(b, c)) {
        return b;
      }
      return comp(a, c)? c: a;
    } else {
      if (comp(a, c)) {
        return a;
      }
      return comp(b, c)? c: b;
    }
  }

  template <typename T, typename Compare>
  int medianOfThreePartition(std::vector<T> &vec, int p, int r, Compare comp) {
    Chapter5::Random rand;
    T &pivot = median(vec[rand(p, r)], vec[rand(p, r)], vec[rand(p, r)], comp);
    std::swap(vec[r], pivot);
    return partition(vec, p, r, comp);
  }

  template <typename T, typename Compare>
  std::pair<int, int> threeWayPartitionByValue(std::vector<T> &vec, const T &pivot, int p, int r, Compare comp) {
    int less = p-1;
    int equal = p-1;

    for (int i = p; i <= r; ++i) {
      if (comp(vec[i], pivot)) { // if vec[i] < pivot
        ++less;
        ++equal;
        std::swap(vec[equal], vec[i]);
        std::swap(vec[less], vec[equal]);
      } else if (!comp(pivot, vec[i])) { // if vec[i] == pivot
        ++equal;
        std::swap(vec[equal], vec[i]);
      } // otherwise, vec[i] > pivot, just increment i
    } 
    return std::pair<int, int>(less, equal);
  }

  template <typename T, typename Compare>
  std::pair<int, int> threeWayPartition(std::vector<T> &vec, int p, int r, Compare comp) {
    return threeWayPartitionByValue(vec, vec[r], p, r, comp);
  }

  template <typename T, typename Compare>
  std::pair<int, int> randomThreeWayPartition(std::vector<T> &vec, int p, int r, Compare comp) {
    Chapter5::Random rand;
    std::swap(vec[r], vec[rand(p, r)]);
    return threeWayPartition(vec, p, r, comp);
  }
}