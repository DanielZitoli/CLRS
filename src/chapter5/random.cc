#include "chapter5/random.h"

namespace Chapter5 {
  Random::Random(): initialSeed{rd()} {
    resetSeed();
  }

  int Random::operator()(int low, int high) {
    std::uniform_int_distribution<int> distribution{low, high};
    return distribution(generator);
  }

  void Random::resetSeed() {
    generator.seed(initialSeed);
  }
}