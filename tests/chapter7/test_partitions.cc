#include "gtest/gtest.h"

#include "chapter7/partitions.h"
#include "helpers/random_generators.h"

#include <utility>


TEST(Chapter7, SimplePartition) {
  std::vector<int> input1{3, 1, 7, 5, 8, 4};
  std::vector<int> input2{2, 8, 7, 1, 3, 5, 6, 4};  
  int pivot1 = Chapter7::partition(input1, 0, 5);
  int pivot2 = Chapter7::partition(input2, 0, 7);
  std::vector<int> partitionedVector1{3, 1, 4, 5, 8, 7};
  std::vector<int> partitionedVector2{2, 1, 3, 4, 7, 5, 6, 8};

  EXPECT_EQ(input1, partitionedVector1);
  EXPECT_EQ(pivot1, 2);
  EXPECT_EQ(input2, partitionedVector2);
  EXPECT_EQ(pivot2, 3);
}

TEST(Chapter7, ValidPartitionsByValue) {
  int range = 50;
  for (int i = 1; i < 10; ++i) {
    int size = 3*i;
    int pivotValue = (477*i + i) % range;

    std::vector<int> input = generateRandomIntVector(size, range);
    int pivotIndex = Chapter7::partitionByValue(input, pivotValue, 0, size - 1, std::less<int>());
  
    bool validPartition = true;
    for (int j = 0; j <= pivotIndex; ++j) {
      validPartition = validPartition && (input[j] <= pivotValue);
    }
    for (int j = pivotIndex+1; j < size; ++j) {
      validPartition = validPartition && (input[j] > pivotValue);
    }
    EXPECT_TRUE(validPartition);
  }
}

TEST(Chapter7, ValidPartitions) {
  std::vector<std::function<int(std::vector<int>&, int, int, std::less<int>)>> partitionFunctions{
    Chapter7::partition<int>,
    Chapter7::randomPartition<int>,
    Chapter7::medianOfThreePartition<int>
  };
  int partitionCount = partitionFunctions.size();
  std::vector<std::vector<int>> inputs(3);
  std::vector<int> pivots(3);

  int range = 50;
  for (int i = 1; i < 10; ++i) {
    int size = 3*i;
    for (int j = 0; j < partitionCount; ++j) {
      inputs[j] = generateRandomIntVector(size, range);
      pivots[j] = partitionFunctions[j](inputs[j], 0, size - 1, std::less<int>());
    }
    for (int j = 0; j < partitionCount; ++j) {
      bool validPartition = true;
      int pivot = pivots[j];
      for (int k = 0; k <= pivot; ++k) {
        validPartition = validPartition && (inputs[j][k] <= inputs[j][pivot]);
      }
      for (int k = pivot+1; k < size; ++k) {
        validPartition = validPartition && (inputs[j][k] > inputs[j][pivot]);
      }
      EXPECT_TRUE(validPartition);
    }
  }
}

TEST(Chapter7, SimpleThreeWayPartition) {
  int lessPivot1, lessPivot2, equalPivot1, equalPivot2;
  std::vector<int> input1{2, 1, 3, 3, 4, 4, 2, 1, 1, 3};
  std::vector<int> input2{6, 4, 4, 5, 2, 3, 2, 4, 5, 1, 4};  
  std::tie(lessPivot1, equalPivot1) = Chapter7::threeWayPartition(input1, 0, input1.size() - 1);
  std::tie(lessPivot2, equalPivot2) = Chapter7::threeWayPartition(input2, 0, input2.size() - 1);
  std::vector<int> partitionedVector1{2, 1, 2, 1, 1, 3, 3, 3, 4, 4};
  std::vector<int> partitionedVector2{2, 3, 2, 1, 4, 4, 4, 4, 5, 6, 5};

  EXPECT_EQ(input1, partitionedVector1);
  EXPECT_EQ(lessPivot1, 4);
  EXPECT_EQ(equalPivot1, 7);
  EXPECT_EQ(input2, partitionedVector2);
  EXPECT_EQ(lessPivot2, 3);
  EXPECT_EQ(equalPivot2, 7);
}

TEST(Chapter7, ValidThreeWayPartitions) {
  std::vector<std::function<std::pair<int, int>(std::vector<int>&, int, int, std::less<int>)>> partitionFunctions{
    Chapter7::threeWayPartition<int>,
    Chapter7::randomThreeWayPartition<int>,
  };
  int partitionCount = partitionFunctions.size();
  std::vector<std::vector<int>> inputs(2);
  std::vector<std::pair<int, int>> pivots(2);

  for (int i = 1; i < 10; ++i) {
    int size = i;
    int range = 55 - size;
    for (int i = 0; i < partitionCount; ++i) {
      inputs[i] = generateRandomIntVector(size, range);
      pivots[i] = partitionFunctions[i](inputs[i], 0, size - 1, std::less<int>());
    }
    for (int i = 0; i < partitionCount; ++i) {
      bool validPartition = true;
      int lessPivot, equalPivot;
      std::tie(lessPivot, equalPivot) = pivots[i];
      for (int j = 0; j <= lessPivot; ++j) {
        validPartition = validPartition && (inputs[i][j] < inputs[i][equalPivot]);
      }
      for (int j = lessPivot+1; j <= equalPivot; ++j) {
        validPartition = validPartition && (inputs[i][j] == inputs[i][equalPivot]);
      }
      for (int j = equalPivot+1; j < size; ++j) {
        validPartition = validPartition && (inputs[i][j] > inputs[i][equalPivot]);
      }
      EXPECT_TRUE(validPartition);
    }
  }
}