#pragma once

#include "chapter6/heaps.h"

#include <functional>
#include <utility>
#include <limits>

namespace Chapter6 {

  template <typename T>
  using TypeComparator = std::function<bool(T, T)>;

  template <typename T>
  class HeapStack {
    using pair = std::pair<int, T>;

    Heap<std::pair<int, T>, TypeComparator<pair>> heap;
    size_t itemCounter = 0;
    size_t items = 0;

  public:
    HeapStack() : heap{[](const pair &a, const pair &b) { return a.first < b.first; }} {}
    void push(const T &x) { 
      heap.insert(std::pair<int, T>(itemCounter++, x));
      ++items;
    }
    void pop() { 
      heap.extractPeak(); 
      --items;
    }
    const T &top() const { return heap.getPeak().second; }
    T &top() { return heap.getPeak().second; }
    size_t size() const { return items; } 
  };

  template <typename T>
  class HeapQueue {
    using pair = std::pair<int, T>;

    Heap<std::pair<int, T>, TypeComparator<pair>> heap;
    size_t itemCounter = 0;
    size_t items = 0;

  public:
    HeapQueue() : heap{[](const pair &a, const pair &b) { return a.first > b.first; }} {}
    void push(const T &x) { 
      heap.insert(std::pair<int, T>(itemCounter++, x));
      ++items;
    }
    void pop() { 
      heap.extractPeak(); 
      --items;
    }
    const T &front() const { return heap.getPeak().second; }
    T &front() { return heap.getPeak().second; }
    size_t size() const { return items; } 
  };

  template <typename T>
  std::vector<T> mergeVectors(const std::vector<std::vector<T>> &listOfVecs) {
    std::vector<int> indices(listOfVecs.size(), 1); // indices start at 1, since each vector initially gets one element pushed to heap
    MinHeap<std::pair<T, int>> heap;

    int n = 0; // accumulates # of elements in each list
    for (int i = 0; i < indices.size(); ++i) {
      int listSize = listOfVecs[i].size();
      if (listSize != 0) {
        n += listSize;
        heap.insert(std::pair<T, int>(listOfVecs[i][0], i));
      }
    }

    std::vector<T> result(n);
    int vecIndex;
    for (int i = 0; i < n; ++i) {
      std::tie(result[i], vecIndex) = heap.extractPeak();
      if (indices[vecIndex] < listOfVecs[vecIndex].size()) {
        heap.insert(std::pair<T, int>(listOfVecs[vecIndex][indices[vecIndex]++], vecIndex));
      }
    }
    return result;
  }

  class YoungArray {
    static const int INF = std::numeric_limits<int>::max();
    int m; int n;
    std::vector<std::vector<int>> array;
    int items = 0;

   public:
    YoungArray(int m = 1, int n = 1);
    YoungArray(const std::vector<int> &vec);
    YoungArray(std::vector<int> &&vec);

    void insert(int val);
    int extractMin();
    int getMin() const;
    int size() const;
    bool empty() const;
    void printArray();

   private:
    void removeHelper(int i, int j);
    void insertHelper(int i, int j);
    int getRight(int i, int j) const;
    int getDown(int i, int j) const;
    int getLeft(int i, int j) const;
    int getUp(int i, int j) const;
    void checkEmpty() const;
    void checkFull() const;
  };

  template <typename T, typename Compare = std::less<T>>
  void youngSort(std::vector<T> &vec, Compare comp = Compare()) {
    YoungArray young{vec};
    for (int i = 0; i < vec.size(); ++i) {
      vec[i] = young.extractMin();
    }
  }

}