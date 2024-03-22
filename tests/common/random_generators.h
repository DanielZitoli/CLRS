#pragma once

#include <vector>
#include <random>

std::vector<int> generateRandomIntVector(int size, int range);

template <typename T>
class RandomValue {
  public:
  RandomValue();
  int operator()(T low, T high); // returns random int in range [low, high]
  void resetSeed();
  private:
  std::random_device rd;
  unsigned initialSeed;
  std::mt19937 generator;
};

template <typename T>
RandomValue<T>::RandomValue(): initialSeed{rd()} {
  resetSeed();
}

template <typename T>
int RandomValue<T>::operator()(T low, T high) {
  std::uniform_int_distribution<T> distribution{low, high};
  return distribution(generator);
}

template <typename T>
void RandomValue<T>::resetSeed() {
  generator.seed(initialSeed);
}