#include "chapter9/order_statistics.h"

namespace Chapter9 {
  int medianOfTwoSortedLists(const std::vector<int> &vec1, const std::vector<int> &vec2) {
    int n = vec1.size(); // assume length of vectors are the same
    int left1 = 0, left2 = 0; int right1 = n-1, right2 = n-1;

    while (left1 + 1 < right1 && left2 + 1 < right2) {
      int mid1 = (left1+right1) / 2; // lower median
      int mid2 = (left2+right2+1) / 2; // upper median

      if (vec1[mid1] < vec2[mid2]) {
        left1 = mid1;
        right2 = mid2;
      } else if (vec1[mid1] > vec2[mid2]) {
        right1 = mid1;
        left2 = mid2;
      } else {
        return vec1[mid1];
      }
    }
    
    if (left1 == right1) {
      return std::max(vec1[left1], vec2[left2]);
    } else {
      if (vec1[left1] < vec2[left2]) {
        return (vec2[left2] < vec1[right1]) ? vec2[left2] : vec1[right1];
      } else {
        return (vec1[left1] < vec2[right2]) ? vec1[left1] : vec2[right2];
      }
    }
  }
}
