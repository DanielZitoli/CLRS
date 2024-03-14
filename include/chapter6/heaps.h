#pragma once

#include <vector>
#include <functional>
#include <iostream>

namespace Chapter6 {
  int parent(int index);
  int left(int index);
  int right(int index);

  template <typename T, typename Compare = std::less<T>>
  void heapify(std::vector<T> &vec, int heapSize, int index, const Compare &comp = Compare()) {
    int l = left(index);
    int r = right(index);

    int maxIndex = index;

    if (l < heapSize && comp(vec[index], vec[l])) {
      maxIndex = l;
    }
    if (r < heapSize && comp(vec[maxIndex], vec[r])) {
      maxIndex = r;
    }

    if (maxIndex != index) {
      std::swap(vec[index], vec[maxIndex]);
      heapify(vec, heapSize, maxIndex, comp);
    }
  }

  template <typename T, typename Compare = std::less<T>>
  void bubbleUp(std::vector<T> &vec, int index, const Compare &comp = Compare()) {
    while(index > 0 && comp(vec[parent(index)], vec[index])) {
      std::swap(vec[index], vec[parent(index)]);
      index = parent(index);
    }
  }  

  template <typename T, typename Compare = std::less<T>>
  void removeHeap(std::vector<T> &vec, int heapSize, int index, const Compare &comp = Compare()) {
    std::swap(vec[index], vec[--heapSize]);
    if (comp(vec[parent(index)], vec[index])) {
      bubbleUp(vec, index, comp);
    } else {
      heapify(vec, heapSize, index, comp);
    }
  }

  template <typename T, typename Compare = std::less<T>>
  void buildHeap(std::vector<T> &vec, const Compare &comp = Compare()) {
    int n = vec.size();
    for (int i = n/2; i >= 0; --i) {
      heapify(vec, n, i, comp);
    }
  }


  template <typename T, typename Compare = std::less<T>>
  void heapSort(std::vector<T> &vec, const Compare &comp = Compare()) {
    buildHeap(vec, comp);
    int n = vec.size();
    for (int i = n-1; i >= 0; --i) { 
      std::swap(vec[0], vec[i]);
      heapify(vec, i, 0, comp);
    }
  }

  template <typename T, typename Compare = std::less<T>>
  class Heap {
    // Heap Property: In a valid heap, for every 0 < i < heapSize we have
    // comp(theHeap[i], theHeap[parent(i)]) is true.
    // Heaps are max-heaps by default
    std::vector<T> theHeap;
    size_t heapSize = 0;
    Compare comp;

   public:
    Heap(Compare comp = Compare()) : comp(comp) {}
    Heap(const std::vector<T> &arr, Compare comp = Compare()) : theHeap{arr}, heapSize{arr.size()}, comp{comp} {
      buildHeap(theHeap, comp);
    }
    Heap(std::vector<T> &&arr, Compare comp = Compare()) : theHeap{std::move(arr)}, heapSize{theHeap.size()}, comp{comp} {
      buildHeap(theHeap, comp);
    }

    const T &getPeak() const { 
      checkEmpty();
      return theHeap[0];
    }
    T &getPeak() { 
      checkEmpty();
      return theHeap[0];
    }
    T extractPeak();
    void insert(const T &x);
    int size() const { return heapSize; }
    bool empty() const { return size() == 0; }
    void printHeap() const {
      for (int i = 0; i < heapSize; ++i) std::cout << theHeap[i] << " ";
      std::cout << std::endl;
    }

   private:
    void checkEmpty() const {
      if (empty()) {
        throw std::out_of_range{"Cannot access the peak of an empty Heap"};
      }
    }
  };
  
  template <typename T, typename Compare>
  T Heap<T, Compare>::extractPeak() {
    if (empty()) {
      throw std::out_of_range{"Cannot delete empty Heap"};
    }
    T maxElement = theHeap[0];
    theHeap[0] = theHeap[--heapSize];
    theHeap.pop_back();
    heapify(theHeap, heapSize, 0, comp);
    return maxElement;
  }

  template <typename T, typename Compare>
  void Heap<T, Compare>::insert(const T &x) {
    theHeap.push_back(x);
    bubbleUp(theHeap, heapSize++, comp);
  }

  template <typename T>
  using MinHeap = Heap<T, std::greater<T>>;

  template <typename T>
  using MaxHeap = Heap<T, std::less<T>>;

}