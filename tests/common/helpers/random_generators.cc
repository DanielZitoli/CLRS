#include "helpers/random_generators.h"
#include <random>
#include <algorithm>

std::vector<int> generateRandomIntVector(int size, int range) {
  std::vector<int> result(size);
  std::random_device rd;
  std::mt19937 gen;
  gen.seed(rd());
  std::uniform_int_distribution<int> dist{1, range};
  std::generate(result.begin(), result.end(), [&gen, &dist]() { return dist(gen); });
  return result;
}
