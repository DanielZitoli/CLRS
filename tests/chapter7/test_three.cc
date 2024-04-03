#include "chapter7/partitions.h"
#include "chapter7/quick_sort.h"
#include "helpers/printing_helpers.h"

#include <vector>
#include <utility>
#include <iostream>

int main() {
  std::vector<std::pair<int, int>> input{
    {3, 6},
    {9, 12},
    {2, 8},
    {6, 15},
    {10, 18},
    {3, 11},
    {7, 8},
    {3, 4},
    {1, 2},
    {13, 16},
    {3, 14},
    {5, 6}
  };

  std::function<bool(int, std::pair<int, int>)> compareLeft = [](const int &a, const std::pair<int, int> &b){
    return a < b.first; 
  };
  std::function<bool(int, std::pair<int, int>)> compareRight = [](const int &a, const std::pair<int, int> &b){
    return a < b.second;
  };
  auto comp = std::less<int>();
  std::pair<int, int> interval = Chapter7::overlapping(input, 0, input.size() - 1, comp);
  int q = Chapter7::partitionByValue(input, interval.first, 0, input.size() - 1, compareLeft);
  int t = Chapter7::partitionByValue(input, interval.second, 0, q, compareRight);

  std::cout << "intersection: " << interval << std::endl;
  std::cout << "pivots: {" << q << ", " << t << "}\n";

  printVec(input);
  Chapter7::fuzzySortHelper(input, 0, 2, comp, compareLeft, compareRight);
  Chapter7::fuzzySortHelper(input, 8, input.size() - 1, comp, compareLeft, compareRight);
  printVec(input);

}