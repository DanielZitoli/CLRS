#pragma once

#include <vector>
#include <functional>

namespace Chapter2 {
  template <typename T>
  int linearSearch(const std::vector<T> &arr, const T &target) {
    int n = arr.size();
    for (int i = 0; i < n; ++i) {
      if (arr[i] == target) {
        return i;
      }
    }
    return -1;
  }  

  template <typename T>
  int binarySearch(const std::vector<T> &arr, const T &target) {
    int left = 0;
    int right = arr.size() - 1;
    while (left <= right) {
      int mid = left + (right - left) / 2;
      if (arr[mid] < target) {
        left = mid + 1;
      } else if (arr[mid] > target) {
        right = mid - 1;
      } else {
        return mid;
      }
    }
    return -1;
  }
}