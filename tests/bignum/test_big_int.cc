#include "gtest/gtest.h"

#include "bignum/big_int.h"

#include "helpers/speed_profiler.h"
#include "helpers/printing_helpers.h"


using namespace BigNum;

TEST(BigIntTests, Constructors) {
  BigInt intNum = 12345;
  BigInt stringNum("12345");

  EXPECT_EQ(intNum.toNum(), 12345);
  EXPECT_EQ(stringNum.toNum(), 12345);
  EXPECT_EQ(intNum.toString(), "12345");
  EXPECT_EQ(stringNum.toString(), "12345");
}

TEST(BigIntTests, CopyConstructor) {
  BigInt num1 = 12345;
  BigInt num2 = num1;
  EXPECT_EQ(num1.toNum(), 12345);
  
  BigInt num3 = std::move(num2);
  num3 -= BigInt(100);
  EXPECT_EQ(num3.toNum(), 12245);
  EXPECT_EQ(num2.toNum(), 0);
  num2 += BigInt(100);
  EXPECT_EQ(num2.toNum(), 100);
}

TEST(BigIntTests, StringInputOutput) {
  BigInt num("123456789");
  EXPECT_EQ(num.toString(), "123456789");
  BigInt pow = 1;
  for (int i = 0; i < 10; ++i) pow *= num;
  BigInt(1).toString();
  // BigInt(1).binaryScaleUp(1000);
  // EXPECT_EQ(pow.toString(), "822526259147102579504761143661535547764137892295514168093701699676416207799736601");

  // EXPECT_EQ(BigInt(1).binaryScaleUp(200).toString(), "1606938044258990275541962092341162602522202993782792835301376");
}

TEST(BigIntTests, Operations) {
  for (long long value1 = -50000000000000000; value1 < 50000000000000000; value1+=19977777777777777) {
    for (long long value2 = -50000000000000000; value2 < 50000000000000000; value2+=17355555555555555) {
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

TEST(BigIntTests, SpeedProfiling) {
  std::unordered_map<std::string, std::unordered_map<int, int>> times;
  std::vector<int> bases{1, 2, 4, 8};
  for (int base : bases) {
    BigInt::BASE = base;
    BigInt::BIT_SIZE = static_cast<uint64_t>(1) << base;

    auto startTime = std::chrono::high_resolution_clock::now();
    BigInt num("123456789");
    auto endTime = std::chrono::high_resolution_clock::now();
    times["Building BigInt from strig took"][base] = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
  
    startTime = std::chrono::high_resolution_clock::now();
    BigInt scaledNum = 1;
    scaledNum = scaledNum.binaryScaleUp(100);
    endTime = std::chrono::high_resolution_clock::now();
    times["Scaling long string took"][base] = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

    startTime = std::chrono::high_resolution_clock::now();
    scaledNum.toString();
    endTime = std::chrono::high_resolution_clock::now();
    times["Printing large num to string took"][base] = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

    startTime = std::chrono::high_resolution_clock::now();
    BigInt result = scaledNum * num;
    endTime = std::chrono::high_resolution_clock::now();
    times["Multiplying two large nums took"][base] = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

    startTime = std::chrono::high_resolution_clock::now();
    result = scaledNum / num;
    endTime = std::chrono::high_resolution_clock::now();
    times["Dividing two large nums took"][base] = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

    startTime = std::chrono::high_resolution_clock::now();
    result = scaledNum + num;
    endTime = std::chrono::high_resolution_clock::now();
    times["Adding two large nums took"][base] = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

    startTime = std::chrono::high_resolution_clock::now();
    long long val = scaledNum.binaryScaleDown(50).toNum();
    ++val;
    endTime = std::chrono::high_resolution_clock::now();
    times["Transforming to int value took"][base] = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
  }
  
  TableDisplay table{40, 12, 12, 12, 12};
  std::vector<std::string> baseHeaders(bases.size());
  std::transform(bases.begin(), bases.end(), baseHeaders.begin(), [](int base) {return "Base == " + std::to_string(base); });
  table.printHeader("Operation", baseHeaders.begin(), baseHeaders.end());

  for (auto& time : times) {
    std::vector<std::string> operationTimes(bases.size());
    std::transform(bases.begin(), bases.end(), operationTimes.begin(), [&time](int base) {return std::to_string(time.second[base]) + " ms"; });
    table.printRow(time.first, operationTimes.begin(), operationTimes.end());
  }
  std::cout << std::endl;

  // PROFILE_RESULTS("Speed Benchmark");
}
