#pragma once

#include <tuple>

namespace NumberTheory {

template <typename T>
T gcd(const T& a, const T& b) {
  if (zero(b)) return T(0);
  return gcd(b, a % b);
}

template <typename T>
std::tuple<T, T, T> extendedEuclid(const T& a, const T& b) {
  using triple = std::tuple<T, T, T>;
  if (zero(b)) return triple(a, 1, 0);
  triple prev = extendedEuclid(b, a % b); // prev = (d, x, y)
  return triple(std::get<0>(prev),                                  // d
                std::get<2>(prev),                                  // y
                std::get<1>(prev) - ((a / b) * std::get<2>(prev))); // x - [a/b]*y
}


// Solves the equation ax ~ b (mod n) 
template <typename T>
std::vector<T> linearEquation(const T& a, const T& b, const T& n) {

}

}
