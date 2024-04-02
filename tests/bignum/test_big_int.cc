#include "gtest/gtest.h"

#include "bignum/big_int.h"
#include "random_generators.h"

using namespace BigNum;

TEST(BigIntTests, Constructors) {
  BigInt intNum = 12345;
  BigInt stringNum("12345");

  EXPECT_EQ(intNum.toNum(), 12345);
  EXPECT_EQ(stringNum.toNum(), 12345);
}

TEST(BigIntTests, StringInputOutput) {
  BigInt num("123456789");
  EXPECT_EQ(num.toString(), "123456789");
  BigInt pow = 1;
  for (int i = 0; i < 10; ++i) pow *= num;
  EXPECT_EQ(pow.toString(), "822526259147102579504761143661535547764137892295514168093701699676416207799736601");
  EXPECT_EQ(BigInt(1).binaryScaleUp(200).toString(), "1606938044258990275541962092341162602522202993782792835301376");
}

TEST(BigIntTests, Operations) {
  for (int value1 = -50; value1 < 50; ++value1) {
    for (int value2 = -50; value2 < 50; ++value2) {
      BigInt num1(value1);
      BigInt num2(value2);
      BigInt additionResult = num1 + num2;
      BigInt subtractionResult = num1 - num2;
      BigInt multiplicationResult = num1 * num2;
      EXPECT_EQ(additionResult.toNum(), value1+value2) << value1 << " + " << value2 << " is not properly handled\n";
      EXPECT_EQ(subtractionResult.toNum(), value1-value2) << value1 << " - " << value2 << " is not properly handled\n";
      EXPECT_EQ(multiplicationResult.toNum(), value1*value2) << value1 << " * " << value2 << " is not properly handled\n";
      if (value2 != 0) {
        BigInt divisionResult = num1 / num2;
        BigInt moduloResult = num1 % num2;
        EXPECT_EQ(divisionResult.toNum(), value1/value2) << value1 << " / " << value2 << " is not properly handled\n";
        EXPECT_EQ(moduloResult.toNum(), value1%value2) << value1 << " % " << value2 << " is not properly handled\n";
      }
    }
  }
}

TEST(BigIntTests, Comparisons) {
  for (int i = -25; i < 25; ++i) {
    for (int j = -25; j < 25; ++j) {
      EXPECT_EQ(BigInt(i) < BigInt(j), i < j) << i << " < " << j << " is not properly handled\n";
      EXPECT_EQ(BigInt(i) == BigInt(j), i == j) << i << " == " << j << " is not properly handled\n";
      EXPECT_EQ(BigInt(i) <= BigInt(j), i <= j) << i << " <= " << j << " is not properly handled\n";
    }
  }
}

TEST(BigIntTests, SpecialOperations) {
  for (int i = -10; i < 100; ++i) {
    EXPECT_EQ(BigInt(i).twice().toNum(), i * 2);
    if (i >= 0) {
      EXPECT_EQ(BigInt(i).half().toNum(), i / 2);
      for (int k = 0; k < 5; ++k) {
        EXPECT_EQ(BigInt(i).binaryScaleDown(k).toNum(), i >> k);
        EXPECT_EQ(BigInt(i).binaryScaleUp(k).toNum(), i << k);
      }
    }
    EXPECT_EQ(BigInt(i).positive(), i > 0) << "i > 0 failed for i = " << i;
    EXPECT_EQ(BigInt(i).negative(), i < 0) << "i < 0 failed for i = " << i;
    EXPECT_EQ(BigInt(i).zero(), i == 0) << "i == 0 failed for i = " << i;
    EXPECT_EQ(BigInt(i).one(), i == 1) << "i == 1 failed for i = " << i;
    EXPECT_EQ(BigInt(i).even(), i % 2 == 0) << "i % 2 == 0 failed for i = " << i;
    EXPECT_EQ(BigInt(i).odd(), i % 2 != 0) << "i % 2 == 1 failed for i = " << i;
  }
}
