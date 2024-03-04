#include "chapter2/insertion_sort.h"


void Chapter2::insertionSort(std::vector<int> &arr) {
  int n = arr.size();
  for (int i = 1; i < n; ++i) {
    int key = arr[i];
    int j = i - 1;
    while (j >= 0 && arr[j] > key) {
      arr[j+1] = arr[j];
      --j;
    }
    arr[j+1] = key;
  }
}