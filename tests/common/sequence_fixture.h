#include "gtest/gtest.h"

#include "random_generators.h"

#include <list>
#include <vector>
#include <string>
#include <algorithm>

template <typename T>
class SequenceTest : public ::testing::Test { //public InterfaceTest<SequenceTest<T>> {
 protected:
  using BaseType = std::list<int>;
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

    baseIter = base.insert(baseIter, 100);
    testIter = test.insert(testIter, 100);
    EXPECT_EQ(*baseIter, *testIter);
    checkElements(message);

    baseIter = base.insert(baseIter, 101);
    testIter = test.insert(testIter, 101);
    EXPECT_EQ(*baseIter, *testIter);
    checkElements(message);

    baseIter = base.insert(baseIter, 102);
    testIter = test.insert(testIter, 102);
    EXPECT_EQ(*baseIter, *testIter);
    checkElements(message);

    baseIter = base.erase(baseIter);
    testIter = test.erase(testIter);
    EXPECT_EQ(*baseIter, *testIter);
    checkElements(message);
  }
};

TYPED_TEST_SUITE_P(SequenceTest);

TYPED_TEST_P(SequenceTest, DestructEmpty) {
  // do nothing, ensures that a default constructed object is valid, and is destroyed properly when empty
  typename TestFixture::TestType emptyTest;
}

TYPED_TEST_P(SequenceTest, DefaultConstructor) {
  typename TestFixture::BaseType newBase;
  typename TestFixture::TestType newTest;
  this->base = newBase; this->test = newTest;
  this->testModifiers();
}

TYPED_TEST_P(SequenceTest, InitListConstructor) {
  typename TestFixture::BaseType newBase{4, 2, 6, 8, 1, 5};
  typename TestFixture::TestType newTest{4, 2, 6, 8, 1, 5};
  this->base = newBase; this->test = newTest;
  this->testModifiers();
}

TYPED_TEST_P(SequenceTest, MoveConstructor) {
  typename TestFixture::BaseType newBase{4, 2, 6, 8, 1, 5};
  typename TestFixture::TestType newTest{4, 2, 6, 8, 1, 5};
  this->base = std::move(newBase); this->test = std::move(newTest);
  this->testModifiers("before");
  this->base = newBase; this->test = newTest; // check if moved objects were left in a valid state
  this->testModifiers("after");
}

TYPED_TEST_P(SequenceTest, Insert) {
  this->base = typename TestFixture::BaseType{1, 2, 3, 4, 5};
  this->test = typename TestFixture::TestType{1, 2, 3, 4, 5};

  auto baseIter = this->base.begin();
  auto testIter = this->test.begin();

  baseIter = this->base.insert(baseIter, 10);
  testIter = this->test.insert(testIter, 10);
  this->checkElements("first");
  EXPECT_EQ(*baseIter, *testIter);

  ++baseIter; ++baseIter; ++testIter; ++testIter;

  baseIter = this->base.insert(baseIter, 12);
  testIter = this->test.insert(testIter, 12);
  this->checkElements("second");
  EXPECT_EQ(*baseIter, *testIter);

  baseIter = this->base.insert(baseIter, 14);
  testIter = this->test.insert(testIter, 14);
  this->checkElements("third");
  EXPECT_EQ(*baseIter, *testIter);
}

TYPED_TEST_P(SequenceTest, Erase) {
  this->base = typename TestFixture::BaseType{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  this->test = typename TestFixture::TestType{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

  auto baseIter = this->base.begin();
  auto testIter = this->test.begin();

  baseIter = this->base.erase(baseIter);
  testIter = this->test.erase(testIter);
  this->checkElements("first");
  EXPECT_EQ(*baseIter, *testIter);

  ++baseIter; ++baseIter; ++testIter; ++testIter;

  baseIter = this->base.erase(this->base.begin(), baseIter);
  testIter = this->test.erase(this->test.begin(), testIter);
  this->checkElements("second");
  EXPECT_EQ(*baseIter, *testIter);

  ++baseIter; ++baseIter; ++testIter; ++testIter;

  baseIter = this->base.erase(baseIter, this->base.end());
  testIter = this->test.erase(testIter, this->test.end());
  this->checkElements("third");
}

TYPED_TEST_P(SequenceTest, PushAndPop) {
  std::vector<int> integers(100);
  std::iota(integers.begin(), integers.end(), 1);
  RandomValue rand;

  for (auto val : integers) {
    this->checkElements();
    int randomAction = this->base.empty() ? 0 : rand(0, 1);
    if (randomAction == 0) {
      this->base.push_back(val);
      this->test.push_back(val);
    } else if (randomAction == 1) {
      this->base.pop_back();
      this->test.pop_back();
    }
  }
}

REGISTER_TYPED_TEST_SUITE_P(SequenceTest, DestructEmpty, DefaultConstructor, InitListConstructor, MoveConstructor, Insert, Erase, PushAndPop);


template <typename T>
class SequenceAdapter {
  using value_type = typename T::value_type;
  using iterator = typename T::iterator;
  using const_iterator = typename T::const_iterator;
  using size_type = typename T::size_type;

  value_type object;

  SequenceAdapter() {}
  SequenceAdapter(const T &other) : object{other} {}
  SequenceAdapter(T &&other) : object{std::move(other.object)} {}
  template <typename Iter>   
  SequenceAdapter(Iter first, Iter last) : object{first, last} {}
  explicit SequenceAdapter(size_type n, const value_type& val = value_type()) : object{n, val} {}
  SequenceAdapter(std::initializer_list<value_type> il) : object{il} {}
  SequenceAdapter &operator=(T other) { object = other; }

  // Capacity
  size_type size() const { return object.size(); }
  bool empty() const { return object.empty(); }

  // Iterators
  iterator begin() { return object.begin(); }
  const_iterator begin() const { return object.begin(); }
  iterator end() { return object.end(); }
  const_iterator end() const { return object.end(); }

  template <class... Args>  iterator insert(Args&&... args) { object.insert(std::forward<Args>(args)...); }
  template <class... Args>  iterator emplace(Args&&... args) { object.emplace(std::forward<Args>(args)...); }
  template <class... Args>  iterator erase(Args&&... args) { object.erase(std::forward<Args>(args)...); }
};


