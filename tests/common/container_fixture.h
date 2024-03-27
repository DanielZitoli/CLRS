#include "gtest/gtest.h"

template <typename DerivedInterface>
class InterfaceTest : public testing::Test {
  using BaseType = typename DerivedInterface::BaseType;
  using TestType = typename DerivedInterface::TestType;

 protected:
  BaseType base;
  TestType test;

  virtual void checkElements() = 0;
  virtual void testModifiers() = 0;
};
