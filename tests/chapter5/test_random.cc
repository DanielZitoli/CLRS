#include "gtest/gtest.h"
#include "chapter5/random.h"

#include <utility>
#include <set>

TEST(Chapter5, RandomInRange) {
  Chapter5::Random random;

  std::vector<std::pair<int, int>> ranges = {{-5, 5}, {0, 3}, {2, 2}};
  int low; int high;
  for (auto pair : ranges) {
    std::tie(low, high) = pair;
    for (int i = 0; i < (high - low) * 10; ++i) {
      int result = random(low, high);
      EXPECT_TRUE((low <= result && result <= high));
    }
  }
}

TEST(Chapter5, ResetSeed) {
  Chapter5::Random random;
  int n = 10;
  std::vector<int> vec1{n}, vec2{n};

  int low = 0; int high = 100;
  for (int i = 0; i < n; ++i) vec1[i] = random(low, high);
  random.resetSeed();
  for (int i = 0; i < n; ++i) vec2[i] = random(low, high);

  ASSERT_EQ(vec1, vec2);
}

TEST(Chapter5, ShuffleEmpty) {
  std::vector<int> vec;
  Chapter5::shuffle(vec);
  ASSERT_EQ(vec, std::vector<int>{});
}

TEST(Chapter5, UniqueShuffles) {
  std::vector<int> original{1, 2, 3, 4, 5};
  std::set<std::vector<int>> permutations;

  std::vector<int> vec;
  for (int i = 0; i < 100; ++i) {
    vec = original;
    Chapter5::shuffle(vec);
    permutations.insert(vec);
  }
  std::cout << permutations.size() << std::endl;
  ASSERT_GT(permutations.size(), 20);
}