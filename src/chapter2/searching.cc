#include "chapter2/searching.h"

int Chapter2::linearSearch(std::vector<int> &arr, int target) {
  int n = arr.size();
  for (int i = 0; i < n; ++i) {
    if (arr[i] == target) {
      return i;
    }
  }
  return -1;
}  

int Chapter2::binarySearch(std::vector<int> &arr, int target) {
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