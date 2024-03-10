#pragma once

#include <random>

namespace Chapter5 {
  class Random {
   public:
    Random();
    int operator()(int low, int high);
    void resetSeed();
   private:
    std::random_device rd;
    unsigned initialSeed;
    std::mt19937 generator;
  };

  template <typename T>
  void shuffle(std::vector<T> &vec) {
    int n = vec.size();
    Random rand;
    for (int i = 0; i < n; ++i)
      std::swap(vec[i], vec[rand(i, n-1)]);
  }
}