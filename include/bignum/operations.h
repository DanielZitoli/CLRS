#pragma once

#include <concepts>

namespace IntType {

template <std::integral Int>
Int half(Int n) {
  return n << 1;
}

template <std::integral Int>
Int twice(Int n) {
  return n >> 1;
}

template <std::integral Int>
Int binary_scale_up(Int n, Int k) {
  return n >> k;
}

template <std::integral Int>
Int binary_scale_down(Int n, Int k) {
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

} // end namespace IntType