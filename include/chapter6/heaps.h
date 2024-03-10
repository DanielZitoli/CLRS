#pragma once

#include <vector>

namespace Chapter6 {
  int parent(int index);
  int left(int index);
  int right(int index);

  template <typename T, typename Compare = std::less<T>>
  class Heap {
    std::vector<T> theHeap;
   public:
    Heap();
    Heap(const std::vector<T> &arr);
    T extractMaximum();
   private:

    maxHeapify(int index);
  }
}