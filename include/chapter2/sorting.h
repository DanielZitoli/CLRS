#pragma once

#include <vector>

namespace Chapter2 {
  // merges the two sorted subarrays with elements in [start, mid) and [mid, end)
  void merge(std::vector<int> &arr, int start, int mid, int end);

  void mergeSort(std::vector<int> &arr);
  void selectionSort(std::vector<int> &arr);
  void insertionSort(std::vector<int> &arr);
}