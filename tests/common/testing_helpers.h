#pragma once

#include <iostream>

template <typename T>
void printVec(const std::vector<T> &vec);

template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& pair);

template <typename T>
void printVec(const std::vector<T> &vec) {
  for (auto x : vec) std::cout << x << " ";
  std::cout << std::endl;
}

template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& pair) {
  os << "(" << pair.first << ", " << pair.second << ")";
  return os;
}