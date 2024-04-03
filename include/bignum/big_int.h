#pragma once

#include "bignum/operations.h"

#include <deque>
#include <string>
#include <iostream>
#include <concepts>
#include <compare>
#include <chrono>
#include <unordered_map>
#include <vector>


namespace BigNum {

class BigInt {
  using bit_type = uint32_t;
  using signed_bit_type = int64_t;
  using bit_vector = std::deque<bit_type>;

 public:
  static int BASE; // = 2; // each bit is a unsigned 32-bit integer i.e. in the range [0, 2^32 - 1]
  static uint64_t BIT_SIZE; // = static_cast<uint64_t>(1) << BASE; // 2 ^ BASE
  bit_vector bits;
  bool isPositive = true;


 public:
  template <std::integral Int> BigInt(Int num);
  BigInt(const std::string& str);
  BigInt(const BigInt& other);
  BigInt(BigInt&& other);
  BigInt &operator=(BigInt other);
  ~BigInt() {}
 private:
  BigInt(const bit_vector& bits);
  BigInt(bit_vector&& bits);

 public:
  // Operations
  BigInt operator+(const BigInt& other) const;
  BigInt &operator+=(const BigInt& other);
  BigInt &operator++();
  BigInt operator++(int);

  BigInt operator-() const;
  BigInt operator-(const BigInt& other) const;
  BigInt &operator-=(const BigInt& other);
  BigInt &operator--();
  BigInt operator--(int);

  BigInt operator*(const BigInt& other) const;
  BigInt &operator*=(const BigInt& other);
  
  BigInt operator/(const BigInt& other) const;
  BigInt &operator/=(const BigInt& other);
  BigInt operator%(const BigInt& other) const;
  BigInt &operator%=(const BigInt& other);


  // Special-case procedures
  BigInt twice() const;
  BigInt half() const;
  BigInt binaryScaleDown(int k) const;
  BigInt binaryScaleUp(int k) const;
  bool positive() const;
  bool negative() const;
  bool zero() const;
  bool one() const;
  bool even() const;
  bool odd() const;
  static bit_type leastBits(bit_type num, int k);


  // Comparison methods
  bool operator==(const BigInt& other) const;
  bool operator<(const BigInt& other) const;
  std::strong_ordering operator<=>(const BigInt& other) const;

  // Output methods
  long long toNum() const;
  std::string toString() const;
  friend std::ostream &operator<<(std::ostream &out, const BigInt& num);


 private:
  void repairBits();
  static bool compareBitMagnitude(const bit_vector& a, const bit_vector& b);
  BigInt unsignedAddition(const bit_vector& a, const bit_vector& b) const;
  BigInt unsignedSubtraction(const bit_vector& a, const bit_vector& b) const;
  BigInt unsignedMultiplication(const bit_vector& a, const bit_vector& b) const;
  std::pair<BigInt, BigInt> unsignedDivision(const bit_vector& a, const bit_vector& b) const;
};


template <std::integral Int>
BigInt::BigInt(Int num) {
  if (num < 0) {
    num *= -1;
    isPositive = false;
  }
  while (num != 0) {
    bit_type firstBit = num % BIT_SIZE; // get least significant bit
    bits.push_back(firstBit);
    num /= BIT_SIZE;
  }
  repairBits(); // removes trailing 0's for proper representation
}

} // end namespace BigNum
