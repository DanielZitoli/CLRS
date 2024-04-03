#pragma once

#include <vector>
#include <random>

std::vector<int> generateRandomIntVector(int size, int range);

template <typename Dist = std::uniform_int_distribution<int>>
class RandomValue {
  using T = typename Dist::result_type;
 public:
  RandomValue();
  template <typename... Args>
  T operator()(Args &&... args); // returns random int in range [low, high]
  void resetSeed();
 private:
  std::random_device rd;
  unsigned initialSeed;
  std::mt19937 generator;
};

template <typename Dist>
RandomValue<Dist>::RandomValue(): initialSeed{rd()} {
  resetSeed();
}

template <typename Dist>
template <typename... Args>
typename RandomValue<Dist>::T RandomValue<Dist>::operator()(Args &&... args) {
  Dist distribution(std::forward<Args>(args)...);
  return distribution(generator);
}

template <typename T>
void RandomValue<T>::resetSeed() {
  generator.seed(initialSeed);
}