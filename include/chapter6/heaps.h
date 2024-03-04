#pragma once

#include <vector>

using std::vector;

// Heaps are represented as vectors

int left(int i) {
  return (2 * i) + 1;
}

int right(int i) {
  return (2 * i) + 2;
}

int parent(int i) {
  return (i - 1) / 2;
}


template <typename T>
void maxHeapify(vector<T> &heap, int index) {
  int l = left(index);
  int r = right(index);
  int largest;
  if (l < heap.size() && heap[l] > heap[index]) {
    largest = l;
  } else {
    largest = i;
  }
  if (r < heap.size() && heap[r] > heap[largest]) {
    largest = r;
  }
  if (largest != index) {
    T temp = heap[index];
    heap[index] = heap[largest];
    heap[largest] = temp;
    maxHeapify(heap, largest);
  }
}

template <typename T>
void buildMaxHeap(const vector<T> &input) {
  int start = input.size() / 2;
  for (int i = start; i >= 0; --i) {
    maxHeapify(input, i);
  }
}

template <typename T>
void heapSort(vector<T> &input) {
  buildMaxHeap(input);
  for (int i = input.size() - 1; i < )
}