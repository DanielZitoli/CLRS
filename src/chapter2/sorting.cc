#include "chapter2/sorting.h"

void Chapter2::merge(std::vector<int> &arr, int start, int mid, int end) {
  std::vector<int> L(arr.begin()+start, arr.begin()+mid);
  std::vector<int> R(arr.begin()+mid, arr.begin()+end);

  // add eachothers last element plus one, so that when one list runs out, 
  // the other list is emptied out. This is equivalent to the INF sentinel in the book
  L.push_back(R[end-mid-1]+1);
  R.push_back(L[mid-start-1]+1);
  int i = 0; int j = 0;
  for (int index = start; index < end; ++index) {
    if (L[i] < R[j]) {
      arr[index] = L[i];
      ++i;
    } else {
      arr[index] = R[j];
      ++j;
    }
  }
}

void mergeSortHelp(std::vector<int> &arr, int start, int end) {
  if (arr.size() <= 1) return;
  int mid = start + (end - start)/2;
  mergeSortHelp(arr, start, mid);
  mergeSortHelp(arr, mid, end);
  Chapter2::merge(arr, start, mid, end);
}

void Chapter2::mergeSort(std::vector<int> &arr) {
  mergeSortHelp(arr, 0, arr.size());
}

void Chapter2::selectionSort(std::vector<int> &arr) {
  int n = arr.size();
  for (int i = 0; i < n; ++i) {
    int maxIndex = i;
    for (int j = i+1; j < n; ++j) {
      if (arr[j] > arr[maxIndex])
        maxIndex = j;
    }
    std::swap(arr[i], arr[maxIndex]);
  }
}

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