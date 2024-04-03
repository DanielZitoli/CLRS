#pragma once

#include <iostream>
#include <stdexcept>
#include <numeric>
#include <vector>
#include <algorithm>
#include <concepts>
#include <iomanip>
#include <sstream>

// Printing Helpers

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


template <typename T>
concept Arithmetic = std::integral<T> || std::floating_point<T>;

template <typename T>
concept BasicType = Arithmetic<T> || std::is_convertible_v<T, std::string>;

template <typename T>
concept Iterator = std::forward_iterator<T> && !std::is_convertible_v<T, std::string>;

// Formatting Floating point values to strings

template <Arithmetic Float>
std::string formatFloatPrecision(Float num, int decimalPrecision) {
  std::ostringstream stream;
  stream << std::fixed << std::setprecision(decimalPrecision) << num;
  return stream.str();
}


// Table display class

class TableDisplay {
  std::vector<int> widths;
 public:
  TableDisplay(std::initializer_list<int> il) : widths{il} {}

  template <typename... Args>
  void printHeader(Args... args) {
    iterateAndPrint(0, true, args...);
    int totalWidth = std::accumulate(widths.begin(), widths.end(), 0) + widths.size();
    std::cout << std::endl << std::string(totalWidth, '-') << std::endl;
  }

  template <typename... Args>
  void printRow(Args... args) {
    iterateAndPrint(0, true, args...);
    std::cout << std::endl;
  }

  template <BasicType T>
  void iterateAndPrint(int index, bool alignLeft, T first) {
    if (index >= widths.size()) throw std::out_of_range("More arguments supplied to Print___ than there are columns");
    printSingle(index, alignLeft, first);    
  }

  template <BasicType T, typename... Args>
  void iterateAndPrint(int index, bool alignLeft, T first, Args... rest) {
    if (index >= widths.size()) throw std::out_of_range("More arguments supplied to Print___ than there are columns");
    printSingle(index, alignLeft, first);
    iterateAndPrint(index+1, false, rest...);
  }

  template <Iterator Iter>
  void iterateAndPrint(int index, bool alignLeft, Iter first, Iter last) {
    printIterator(index, alignLeft, first, last);
  }

  template <Iterator Iter, typename... Args>
  void iterateAndPrint(int index, bool alignLeft, Iter first, Iter last, Args... rest) { 
    printIterator(index, alignLeft, first, last);
    iterateAndPrint(index, false, rest...);
  }

  template <Iterator Iter>
  void printIterator(int& index, bool alignLeft, Iter first, Iter last) {
    while (first != last) {
      if (index >= widths.size()) throw std::out_of_range("More arguments supplied to Print___ than there are columns");
      printSingle(index, alignLeft, *first);
      ++index;
      alignLeft = false;
      ++first;
    }
  }

  template <typename T>
  void printSingle(int index, bool alignLeft, const T& x) {
    printLine(index, alignLeft, static_cast<std::string>(x));
  }

  template <Arithmetic T>
  void printSingle(int index, bool alignLeft, const T& x) {
    printLine(index, alignLeft, std::to_string(x));
  }

  void printLine(int index, bool alignLeft, const std::string& s) {
    int n = widths[index];
    std::string trimmed = trimLength(s, n);
    if (alignLeft) {
      trimmed.append(n - trimmed.size(), ' ');
    } else {
      std::cout << "|";
      trimmed.insert(0, n - trimmed.size(), ' ');
    }
    std::cout << trimmed;
  }

  static std::string trimLength(const std::string&s, int n) {
    return s.substr(0, std::min(n, static_cast<int>(s.size())));
  }
};
