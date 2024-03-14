#include "chapter6/heap_based_structs.h"
#include <cmath>
#include <algorithm>
#include <iostream>

namespace Chapter6 {

  const int YoungArray::INF;
  YoungArray::YoungArray(int m, int n) : m{m}, n{n}, array(m, std::vector<int>(n, INF)) {}
  YoungArray::YoungArray(const std::vector<int> &vec) : m{static_cast<int>(sqrt(vec.size()) + 1)}, n{m}, array(m, std::vector<int>(n, INF)) {
    for (auto val : vec) {
      insert(val);
    }
  }
  YoungArray::YoungArray(std::vector<int> &&vec) : m{static_cast<int>(sqrt(vec.size()) + 1)}, n{m}, array(m, std::vector<int>(n, INF)) {
    for (auto val : vec) {
      insert(val);
    }
  }

  void YoungArray::insert(int val) {
    checkFull();
    array[m-1][n-1] = val;
    insertHelper(m-1, n-1);
    ++items;
  }

  int YoungArray::extractMin() {
    int min = getMin();
    removeHelper(0, 0);
    --items;
    return min;
  }

  int YoungArray::getMin() const {
    checkEmpty();
    return array[0][0];
  }

  int YoungArray::size() const { return items; }

  bool YoungArray::empty() const { return size() == 0; }

  void YoungArray::printArray() {
    std::cout << "YOUNG ARRAY\n";
    for (auto vec : array) {
      for (auto val : vec) {
        if (val == INF) {
          std::cout << "INF ";
        } else {
          std::cout << val << ' ';
        }
      }
      std::cout << std::endl;
    }
  }

  void YoungArray::removeHelper(int i, int j) {
    int down = getDown(i, j);
    int right = getRight(i, j);

    if (right < down) {
      array[i][j] = right;
      removeHelper(i, j+1);
    } else if (down < right || down < INF) {
      array[i][j] = down;
      removeHelper(i+1, j);
    } else {
      array[i][j] = INF;
    }
  }

  void YoungArray::insertHelper(int i, int j) {
    int up = getUp(i, j);
    int left = getLeft(i, j);
    int cur = array[i][j];
    
    if (up > cur || left > cur) {
      if (up > left) {
        std::swap(array[i][j], array[i-1][j]);
        insertHelper(i-1, j);
      } else {
        std::swap(array[i][j], array[i][j-1]);
        insertHelper(i, j-1);
      }
    }
  }

  int YoungArray::getRight(int i, int j) const {
    return (j+1 < n)? array[i][j+1]: INF;
  }

  int YoungArray::getDown(int i, int j) const {
    return (i+1 < m)? array[i+1][j]: INF;
  }

  int YoungArray::getLeft(int i, int j) const {
    return (j-1 >= 0)? array[i][j-1]: array[i][j];
  }

  int YoungArray::getUp(int i, int j) const {
    return (i-1 >= 0)? array[i-1][j]: array[i][j];
  }

  void YoungArray::checkEmpty() const {
    if (empty()) {
      throw std::out_of_range{"Young Array is empty"};
    }
  }

  void YoungArray::checkFull() const {
    if (size() == (m*n)) {
      throw std::out_of_range{"Young Array is full"};
    }
  }
}
