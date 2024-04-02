#pragma once

#include "gtest/gtest.h"

#include "random_generators.h"

#include <list>
#include <vector>
#include <string>
#include <algorithm>

template <typename T>
class TreeTest : public ::testing::Test {
 protected:
  using BaseType = std::set<int>;
  using TestType = T;

  BaseType base;
  TestType test;

  void checkElements(const std::string &message = std::string()) {
    std::vector<int> baseContents(base.begin(), base.end());
    std::vector<int> testContents;
    for (auto val : test) testContents.push_back(val);
    EXPECT_EQ(baseContents, testContents) << message;
    EXPECT_EQ(base.empty(), test.empty());
    EXPECT_EQ(base.size(), test.size());
  }

  void testModifiers(const std::string &message = std::string()) {
    auto baseIter = base.begin();
    auto testIter = test.begin();
    bool baseInserted = false;
    bool testInserted = false;

    std::tie(baseIter, baseInserted) = base.insert(100);
    std::tie(testIter, testInserted) = test.insert(100);
    EXPECT_EQ(*baseIter, *testIter);
    EXPECT_EQ(baseInserted, testInserted);
    checkElements(message);

    std::tie(baseIter, baseInserted) = base.insert(101);
    std::tie(testIter, testInserted) = test.insert(101);
    EXPECT_EQ(*baseIter, *testIter);
    EXPECT_EQ(baseInserted, testInserted);
    checkElements(message);

    std::tie(baseIter, baseInserted) = base.insert(102);
    std::tie(testIter, testInserted) = test.insert(102);
    EXPECT_EQ(*baseIter, *testIter);
    EXPECT_EQ(baseInserted, testInserted);
    checkElements(message);

    int baseRemoved = base.erase(101);
    int testRemoved = test.erase(101);
    EXPECT_EQ(baseRemoved, testRemoved);
    checkElements(message);

    baseIter = base.erase(base.begin());
    testIter = test.erase(test.begin());
    EXPECT_EQ(*baseIter, *testIter);
    checkElements(message);
  }
};

template <typename Iter>
Iter &advance(Iter &it, int n) {
  if (n > 0) {
    for (int i = 0; i < n; ++i) ++it;
  } else {
    for (int i = 0; i < -n; ++i) ++it;
  }
}

TYPED_TEST_SUITE_P(TreeTest);

TYPED_TEST_P(TreeTest, DestructEmpty) {
  // do nothing, ensures that a default constructed object is valid, and is destroyed properly when empty
  typename TestFixture::TestType emptyTest;
}

TYPED_TEST_P(TreeTest, DefaultConstructor) {
  typename TestFixture::BaseType newBase;
  typename TestFixture::TestType newTest;
  this->base = newBase; 
  this->test = newTest;
  this->testModifiers();
}

TYPED_TEST_P(TreeTest, InitListConstructor) {
  typename TestFixture::BaseType newBase{4, 2, 6, 8, 1, 5};
  typename TestFixture::TestType newTest{4, 2, 6, 8, 1, 5};
  this->base = newBase; this->test = newTest;
  this->testModifiers();
}

TYPED_TEST_P(TreeTest, MoveConstructor) {
  typename TestFixture::BaseType newBase{4, 2, 6, 8, 1, 5};
  typename TestFixture::TestType newTest{4, 2, 6, 8, 1, 5};
  this->base = std::move(newBase); this->test = std::move(newTest);
  this->testModifiers("beforeMove");
  this->base = newBase; this->test = newTest; // check if moved objects were left in a valid state
  this->testModifiers("afterMove");
}

TYPED_TEST_P(TreeTest, Insert) {
  this->base = typename TestFixture::BaseType{1, 2, 3, 4, 5};
  this->test = typename TestFixture::TestType{1, 2, 3, 4, 5};

  auto baseIter = this->base.begin();
  auto testIter = this->test.begin();

  baseIter = this->base.insert(10).first;
  testIter = this->test.insert(10).first;
  this->checkElements("first");
  EXPECT_EQ(*baseIter, *testIter);

  // ++baseIter; ++baseIter; ++testIter; ++testIter;

  baseIter = this->base.insert(12).first;
  testIter = this->test.insert(12).first;
  this->checkElements("second");
  EXPECT_EQ(*baseIter, *testIter);

  baseIter = this->base.insert(14).first;
  testIter = this->test.insert(14).first;
  this->checkElements("third");
  EXPECT_EQ(*baseIter, *testIter);
}

TYPED_TEST_P(TreeTest, Erase) {
  this->base = typename TestFixture::BaseType{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  this->test = typename TestFixture::TestType{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

  auto baseIter = this->base.begin();
  auto testIter = this->test.begin();

  baseIter = this->base.erase(baseIter);
  testIter = this->test.erase(testIter);
  this->checkElements("first");
  EXPECT_EQ(*baseIter, *testIter);
}

TYPED_TEST_P(TreeTest, InsertAndEraseByValue) {
  RandomValue rand;
  for (int i = 0; i < 10000; ++i) {
    this->checkElements();
    int randomAction = this->base.empty() ? 0 : rand(0, 1);
    int randomValue = rand(0, 500);
    if (randomAction == 0) {
      auto baseIter = this->base.insert(randomValue).first;
      auto testIter = this->test.insert(randomValue).first;
      EXPECT_EQ(*baseIter, *testIter);
    } else if (randomAction == 1) {
      int baseRemoved = this->base.erase(randomValue);
      int testRemoved = this->test.erase(randomValue);
      EXPECT_EQ(baseRemoved, testRemoved);
    }
  }
}

TYPED_TEST_P(TreeTest, FindAndCount) {
  RandomValue rand;
  int randomValue;
  for (int i = 0; i < 1000; ++i) {
    randomValue = rand(0, 2000);
    this->base.insert(randomValue);
    this->test.insert(randomValue);
  }

  for (int i = 0; i < 2000; ++i) {
    randomValue = rand(0, 2000);
    auto baseIter = this->base.insert(randomValue).first;
    auto testIter = this->test.insert(randomValue).first;
    if (baseIter != this->base.end()) {
      EXPECT_EQ(*baseIter, *testIter);
    }
    int baseCount = this->base.count(randomValue);
    int testCount = this->test.count(randomValue);
    EXPECT_EQ(baseCount, testCount);
  }
}



TYPED_TEST_P(TreeTest, UpperAndLowerBound) {
  RandomValue rand;
  int randomValue;
  for (int i = 0; i < 1000; ++i) {
    randomValue = rand(0, 1000);
    auto baseIter = this->base.lower_bound(randomValue);
    auto testIter = this->test.lower_bound(randomValue);
    if (baseIter != this->base.end()) {
      EXPECT_EQ(*baseIter, *testIter);
    }
    baseIter = this->base.upper_bound(randomValue);
    testIter = this->test.upper_bound(randomValue);
    if (baseIter != this->base.end()) {
      EXPECT_EQ(*baseIter, *testIter);
    }
    randomValue = rand(0, 1000);
    this->base.insert(randomValue);
    this->test.insert(randomValue);
  }
}

REGISTER_TYPED_TEST_SUITE_P(TreeTest, DestructEmpty, DefaultConstructor, InitListConstructor, MoveConstructor, Insert, Erase, InsertAndEraseByValue, FindAndCount, UpperAndLowerBound);
