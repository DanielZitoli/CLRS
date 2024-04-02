#include "bignum/big_int.h"
#include "bignum/operations.h"

#include <cctype>
#include <algorithm>
#include <bitset>
#include <cmath>
#include <stdexcept>

namespace BigNum {

BigInt::BigInt(const std::string& str) {
  for (auto c : str) {
    if (!isdigit(c)) { throw std::out_of_range("String must be valid base ten number"); }
    int digit = c - '0';
    *this *= 10;
    *this += digit;
  }
}

BigInt::BigInt(const BigInt& other) : bits{other.bits}, isPositive{other.isPositive} {}

BigInt::BigInt(BigInt&& other) : bits{std::move(other.bits)}, isPositive{other.isPositive} {
  other.bits.clear();
  other.repairBits(); 
}

BigInt &BigInt::operator=(BigInt other) {
  std::swap(bits, other.bits);
  std::swap(isPositive, other.isPositive);
  return *this;
}

BigInt::BigInt(const bit_vector& bits) : bits{bits}, isPositive{true} {
  repairBits();
}
BigInt::BigInt(bit_vector&& bits) : bits{std::move(bits)}, isPositive{true} {
  repairBits();
}

// Operations

BigInt BigInt::operator+(const BigInt& other) const {
  if (negative() == other.negative()) {
    BigInt result = unsignedAddition(bits, other.bits);
    return negative()? -result: result;
  } else {
    bool isLessMagnitude = compareBitMagnitude(bits, other.bits); // if |a| < |b|
    if (isLessMagnitude) {
      BigInt result = unsignedSubtraction(other.bits, bits);
      return negative() ? result // -a + b == b - a
                        : -result; // a + (-b) == -(b - a)
    } else {
      BigInt result = unsignedSubtraction(bits, other.bits);
      return negative() ? -result // -a + b == -(a - b)
                        : result; // a + (-b) == a - b
    }
  }
}
BigInt &BigInt::operator+=(const BigInt& other) {
  *this = *this + other;
  return *this;
}
BigInt &BigInt::operator++() {
  *this += BigInt(1);
  return *this;
}
BigInt BigInt::operator++(int) {
  BigInt tmp(*this);
  ++(*this);
  return tmp;
}

// Unary negate operator
BigInt BigInt::operator-() const {
  BigInt copy(*this);
  if (!copy.zero()) copy.isPositive = !copy.isPositive;
  return copy; 
}
BigInt BigInt::operator-(const BigInt& other) const {
  if (negative() != other.negative()) {
    BigInt result = unsignedAddition(bits, other.bits);
    return negative()? -result: result;
  } else {
    bool isLessMagnitude = compareBitMagnitude(bits, other.bits); // if |a| < |b|
    if (isLessMagnitude) {
      BigInt result = unsignedSubtraction(other.bits, bits);
      return negative() ? result // -a - (-b) == b - a
                        : -result; // a - b == -(b - a)
    } else {
      BigInt result = unsignedSubtraction(bits, other.bits);
      return negative() ? -result // -a - (-b) == b - a = -(a - b)
                        : result; // a - b
    }
  }
}
BigInt &BigInt::operator-=(const BigInt& other) {
  *this = *this - other;
  return *this;
}
BigInt &BigInt::operator--() {
  *this -= BigInt(1);
  return *this;
}
BigInt BigInt::operator--(int) {
  BigInt tmp(*this);
  --(*this);
  return tmp;
}

BigInt BigInt::operator*(const BigInt& other) const {
  BigInt result = unsignedMultiplication(bits, other.bits);
  return (negative() == other.negative())? result: -result;
}
BigInt &BigInt::operator*=(const BigInt& other) {
  *this = *this * other;
  return *this;
}

BigInt BigInt::operator/(const BigInt& other) const {
  if (other.zero()) throw std::runtime_error("Division By 0 in BigInt");
  BigInt result = unsignedDivision(bits, other.bits).first;
  return (negative() == other.negative())? result: -result;
}
BigInt &BigInt::operator/=(const BigInt& other) {
  *this = *this / other;
  return *this;
}

BigInt BigInt::operator%(const BigInt& other) const {
  if (other.zero()) throw std::runtime_error("Modulo By 0 in BigInt");
  BigInt result = unsignedDivision(bits, other.bits).second;
  return negative()? -result: result;
}
BigInt &BigInt::operator%=(const BigInt& other) {
 *this = *this % other;
 return *this;
}

// Arithmetic Bit Operations Implementation
// These can return BigInts or bit_vectors since if a bit_vector is returned, it will
// construct into a BigInt and repair its bits

BigInt BigInt::unsignedAddition(const bit_vector& a, const bit_vector& b) const {
  bit_vector result;
  int n = a.size(); int m = b.size();
  bit_type carry = 0;
  for (int i = 0; i < std::max(n, m) || carry; ++i) {
    bit_type first = (i < n)? a[i]: 0;
    bit_type second = (i < m)? b[i]: 0;
    bit_type sum = first + second + carry;
    result.push_back(sum % BIT_SIZE);
    carry = sum / BIT_SIZE;
  }
  return result;
}

BigInt BigInt::unsignedSubtraction(const bit_vector& a, const bit_vector& b) const {
  bit_vector result;
  int n = a.size(); int m = b.size();
  signed_bit_type carry = 0;
  for (int i = 0; i < n; ++i) {
    signed_bit_type first = a[i];
    signed_bit_type second = (i < m)? b[i]: 0;
    signed_bit_type diff = first - second - carry;

    if (diff < 0) {
      diff += BIT_SIZE;
      carry = 1;
    } else {
      carry = 0;
    }
    result.push_back(diff);
  }
  return result;
}

// BigInt BigInt::unsignedMultiplication(const bit_vector& a, const bit_vector& b) const {
//   BigInt result = 0;
//   BigInt first = a;
//   for (int i = 0; i < b.size(); ++i) {
//     bit_type bit = b[i];
//     bit_type power = 1 << (BASE-1);
//     signed_bit_type exp = BASE-1;
//     while (exp >= 0) {
//       if (bit >= power) {
//         bit -= power;
//         result += first.binaryScaleUp(BASE*i + exp);
//       }
//       --exp;
//       power /= 2;
//     }
//   }
//   return result;
// }

BigInt BigInt::unsignedMultiplication(const bit_vector& a, const bit_vector& b) const {
  // Algorithm from Knuth's TAOCP Volume 2, pg. 268
  int m = a.size(); int n = b.size();
  bit_vector result(m+n, 0);

  for (int j = 0; j < n; ++j) {
    bit_type carry = 0;
    for (int i = 0; i < m; ++i) {
      signed_bit_type t = (a[i] * b[j]) + result[i+j] + carry;
      result[i+j] = t % BIT_SIZE;
      carry = t / BIT_SIZE;
    }
    result[j+m] = carry;
  }
  return result;
}

std::pair<BigInt, BigInt> BigInt::unsignedDivision(const bit_vector& a, const bit_vector& b) const {
  BigInt quotient = 0;
  BigInt remainder = 0;
  BigInt bitsize = BIT_SIZE;
  int m = a.size();
  for (int i = m-1; i >= 0; --i) {
    quotient *= bitsize;
    remainder *= bitsize;
    remainder += BigInt(a[i]);
    while (remainder >= b) {
      remainder -= b;
      quotient += 1;
    }
  }
  return std::pair<BigInt, BigInt>(quotient, remainder);
}


// Special Operations

BigInt BigInt::twice() const { 
  return binaryScaleUp(1);
}
BigInt BigInt::half() const { 
  return binaryScaleDown(1);
}
BigInt BigInt::binaryScaleDown(int k) const { 
  int shiftedBits = k % BASE;
  int removeBits = k / BASE;
  bit_vector newBits = bits;
  for (int i = 0; i < removeBits && !newBits.empty(); ++i) {
    newBits.pop_front();
  }
  if (shiftedBits != 0) {
    bit_type lastShifted = 0;
    for (auto it = newBits.rbegin(); it != newBits.rend(); ++it) {
      bit_type shifted = leastBits(*it, shiftedBits);
      *it >>= shiftedBits;
      *it |= (lastShifted << (BASE-shiftedBits));
      lastShifted = shifted;
    }
  }
  BigInt result = newBits;
  return negative()? -result: result;
}
BigInt BigInt::binaryScaleUp(int k) const { 
  int shiftedBits = k % BASE;
  int addBits = k / BASE;
  bit_vector newBits = bits;

  if (shiftedBits != 0) {
    newBits.push_back(0);
    bit_type lastShifted = 0;
    for (auto it = newBits.begin(); it != newBits.end(); ++it) {
      bit_type shifted = leastBits(*it >> (BASE - shiftedBits), shiftedBits);
      *it <<= shiftedBits;
      *it &= (BIT_SIZE - 1); // takes the first BASE bits
      *it |= lastShifted;
      lastShifted = shifted;
    }
  }
  for (int i = 0; i < addBits; ++i) {
    newBits.push_front(0);
  }
  BigInt result = newBits;
  result.repairBits();
  return negative()? -result: result;
}
BigInt::bit_type BigInt::leastBits(bit_type num, int k) {
  return num & ((1 << k) - 1); // watch
}
bool BigInt::positive() const { return (isPositive && !zero()); }
bool BigInt::negative() const { return !isPositive; }
bool BigInt::zero() const { return (isPositive && bits.size() == 1 && bits.front() == 0); }
bool BigInt::one() const { return (isPositive && bits.size() == 1 && bits.front() == 1); }
bool BigInt::even() const { return IntType::even(bits.front()); }
bool BigInt::odd() const { return IntType::odd(bits.front()); }


// Comparison methods

bool BigInt::operator==(const BigInt& other) const {
  return (bits == other.bits) && (isPositive == other.isPositive);
}

bool BigInt::operator<(const BigInt& other) const {
  if (zero()) {
    return other.positive();
  } else {
    if (negative()) {
      return !other.negative() || compareBitMagnitude(other.bits, bits);
    } else {
      return other.positive() && (compareBitMagnitude(bits, other.bits));
    }
  }
}

std::strong_ordering BigInt::operator<=>(const BigInt& other) const {
  if (*this < other) return std::strong_ordering::less;
  if (other < *this) return std::strong_ordering::greater;
  return std::strong_ordering::equivalent;
}

// Helpers

// Ensures the bits are properly represented, which means:
// 1) bits isn't empty
// 2) bits has no trialing 0s
// The expected representation for 0 is a single 0 bit
void BigInt::repairBits() {
  if (bits.empty()) {
    bits.push_back(0);
    isPositive = true;
  }
  else {
    while (!zero() && bits.back() == 0) bits.pop_back();
  }
}
bool BigInt::compareBitMagnitude(const bit_vector& a, const bit_vector& b) {
  return (a.size() < b.size()) || (a.size() == b.size() && std::lexicographical_compare(a.rbegin(), a.rend(), b.rbegin(), b.rend()));
}

// Output methods

long long BigInt::toNum() const {
  long long result = 0;
  for (auto it = bits.rbegin(); it != bits.rend(); ++it) {
    // Shift the current result to the left by one position
    result *= BIT_SIZE;
    // If the current bit is set, OR it with the result 
    result += *it;
  }
  if (negative()) result *= -1;
  return result;
}

std::string BigInt::toString() const {
  std::string result = "";
  BigInt tmp = *this;
  while (!tmp.zero()) {
    int remainder = (tmp % 10).toNum();
    tmp /= 10;
    result.push_back('0'+remainder);
  }
  if (negative()) result.push_back('-');
  std::reverse(result.begin(), result.end());
  return result;
}

std::ostream &operator<<(std::ostream &out, const BigInt& num) {
  out << "Base 10: " << num.toNum();
  out << " - Bits: ";
  for (auto it = num.bits.rbegin(); it != num.bits.rend(); ++it) {
    typename BigInt::bit_type bit = *it;
    out << bit;
    out << ' ';
  }
  out << " - Binary: ";
  for (auto it = num.bits.rbegin(); it != num.bits.rend(); ++it) {
    typename BigInt::bit_type bit = *it;
    out << std::bitset<BigInt::BASE>(bit);
    out << ' ';
  }
  out << std::endl;
  return out;
}

} // end namespace BigNum
