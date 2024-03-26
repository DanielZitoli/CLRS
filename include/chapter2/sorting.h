#pragma once

#include <vector>
#include <functional>

namespace Chapter2 {
  // merges the two sorted subarrays with elements in [start, mid) and [mid, end)
  template <typename T, typename Compare = std::less<T>>
  void merge(std::vector<T> &arr, int start, int mid, int end, Compare comp = Compare());

  template <typename T, typename Compare = std::less<T>>
  void mergeSort(std::vector<T> &arr, Compare comp = Compare());

  template <typename T, typename Compare = std::less<T>>
  void selectionSort(std::vector<T> &arr, Compare comp = Compare());

  template <typename T, typename Compare = std::less<T>>
  void insertionSort(std::vector<T> &arr, Compare comp = Compare());

  template <typename T, typename Compare = std::less<T>>
  void rangeInsertionSort(std::vector<T> &arr, int p, int r, Compare comp = Compare());

  // ########  DEFINITIONS ########

  // comp(a, b) acts as (a < b) so these sort functions sort in increasing order by default

  template <typename T, typename Compare>
  void merge(std::vector<T> &arr, int start, int mid, int end, Compare comp) {
    std::vector<T> L(arr.begin()+start, arr.begin()+mid);
    std::vector<T> R(arr.begin()+mid, arr.begin()+end);

    int i = 0; int j = 0;
    for (int index = start; index < end; ++index) {
      if (i == L.size()) {
        while (j < R.size()) {
          arr[index] = R[j];
          ++j; ++index;  
        }
      } else if (j == R.size()) {
        while (i < L.size()) {
          arr[index] = L[i];
          ++i; ++index;  
        }
      } else if (!comp(R[j], L[i])) { // if L <= R
        arr[index] = L[i];
        ++i;
      } else {
        arr[index] = R[j];
        ++j;
      }
    }
  }

  template<typename T, typename Compare>
  void mergeSortHelp(std::vector<T> &arr, int start, int end, const Compare &comp) {
    if (end - start <= 1) return;
    int mid = (start + end) / 2;
    mergeSortHelp(arr, start, mid, comp);
    mergeSortHelp(arr, mid, end, comp);
    Chapter2::merge(arr, start, mid, end, comp);
  }

  template <typename T, typename Compare>
  void mergeSort(std::vector<T> &arr, Compare comp) {
    mergeSortHelp(arr, 0, arr.size(), comp);
  }

  template <typename T, typename Compare>
  void selectionSort(std::vector<T> &arr, Compare comp) {
    int n = arr.size();
    for (int i = 0; i < n; ++i) {
      int maxIndex = i;
      for (int j = i+1; j < n; ++j) {
        if (comp(arr[j], arr[maxIndex]))
          maxIndex = j;
      }
      std::swap(arr[i], arr[maxIndex]);
    }
  }

  template <typename T, typename Compare>
  void insertionSort(std::vector<T> &arr, Compare comp) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
      T key = arr[i];
      int j = i - 1;
      while (j >= 0 && comp(key, arr[j])) {
        arr[j+1] = arr[j];
        --j;
      }
      arr[j+1] = key;
    }
  }

  template <typename T, typename Compare>
  void rangeInsertionSort(std::vector<T> &arr, int p, int r, Compare comp) {
    for (int i = p+1; i <= r; ++i) {
      T key = arr[i];
      int j = i - 1;
      while (j >= p && comp(key, arr[j])) {
        arr[j+1] = arr[j];
        --j;
      }
      arr[j+1] = key;
    }
  }
}