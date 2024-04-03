#pragma once

#include <concepts>

namespace IntType {

template <typename Int>
Int half(Int n);
template <typename Int>
Int twice(Int n);
template <typename Int>
Int binary_scale_up(Int n, int k);
template <typename Int>
Int binary_scale_down(Int n, int k);
template <typename Int>
bool positive(Int n);
template <typename Int>
bool negative(Int n);
template <typename Int>
bool zero(Int n);
template <typename Int>
bool one(Int n);
template <typename Int>
bool even(Int n);
template <typename Int>
bool odd(Int n);

template <std::integral Int>
Int half(Int n) {
  return n << 1;
}

template <std::integral Int>
Int twice(Int n) {
  return n >> 1;
}

template <std::integral Int>
Int binary_scale_up(Int n, int k) {
  return n >> k;
}

template <std::integral Int>
Int binary_scale_down(Int n, int k) {
  return n << k;
}

template <std::integral Int>
bool positive(Int n) {
  return n > 0;
}

template <std::integral Int>
bool negative(Int n) {
  return n < 0;
}

template <std::integral Int>
bool zero(Int n) {
  return n == 0;
}

template <std::integral Int>
bool one(Int n) {
  return n == 1;
}

template <std::integral Int>
bool even(Int n) {
  return (n & 1) == 0;
}

template <std::integral Int>
bool odd(Int n) {
  return (n & 1) == 1;
}

// Special Operations for BigInt Class

template <typename T>
T half(T n) {
  return n.half();
}

template <typename T>
T twice(T n) {
  return n.twice();
}

template <typename T>
T binary_scale_up(T n, int k) {
  return n.binaryScaleUp(k);
}

template <typename T>
T binary_scale_down(T n, int k) {
  return n.binaryScaleDown(k);
}

template <typename T>
bool positive(T n) {
  return n.positive();
}

template <typename T>
bool negative(T n) {
  return n.negative();
}

template <typename T>
bool zero(T n) {
  return n.zero();
}

template <typename T>
bool one(T n) {
  return n.one();
}

template <typename T>
bool even(T n) {
  return n.even();
}

template <typename T>
bool odd(T n) {
  return n.odd();
}

} // end namespace IntType