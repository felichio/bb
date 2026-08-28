#include <gtest/gtest.h>
#include <core/statistics/MovingAverage.hpp>
#include <core/statistics/SimpleMovingAverage.hpp>
#include <core/statistics/TimeLiterals.hpp>
#include <chrono>
#include <cmath>



class SimpleMovingAverageTest : public testing::Test
{
};

bool belowError(const double a, const double b)
{
  return std::abs(a - b) < .00000001;
}

TEST_F(SimpleMovingAverageTest, SimpleMovingAverageTestOneUpdate)
{
  uint64_t dt = 15_s;
  bb::SimpleMovingAverage sma(dt);
  std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  uint64_t nowU = now.count();

  // Update within the window (+1)  |  nowU - dt + 1
  uint64_t t1 = nowU - dt + 1;
  double expected = 77133.87;
  double actual = sma.update({t1, expected});

  EXPECT_EQ(expected, actual);
}

TEST_F(SimpleMovingAverageTest, SimpleMovingAverageTestTwoUpdate)
{
  uint64_t dt = 15_s;
  bb::SimpleMovingAverage sma(dt);
  std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  uint64_t nowU = now.count();

  // Update within the window (+1)  |  nowU - dt + 1
  double value1 = 77133.87;
  uint64_t t1 = nowU - dt + 1;
  sma.update({t1, value1});
  double value2 = 77716.19;
  uint64_t t2 = t1 + 1; // once second passed 
  double actualAverage = sma.update({t2, value2});

  double expectedAverage = (value1 + value2) / 2;
  EXPECT_TRUE(belowError(actualAverage, expectedAverage));
}

TEST_F(SimpleMovingAverageTest, SimpleMovingAverageTestOneUpdateOutOfWindow)
{
  uint64_t dt = 15_s;
  bb::SimpleMovingAverage sma(dt);
  std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  uint64_t nowU = now.count();

  // Update within the window (+1)  |  nowU - dt + 1
  uint64_t t1 = nowU - dt - 1;
  double value1 = 77133.87;
  double actual = sma.update({t1, value1});

  EXPECT_TRUE(belowError(0, actual));
}

TEST_F(SimpleMovingAverageTest, SimpleMovingAverageTestOneUpdateOneOutOfWindowTwoInWindow)
{
  uint64_t dt = 15_s;
  bb::SimpleMovingAverage sma(dt);
  std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  uint64_t nowU = now.count();

  // Update within the window (+1)  |  nowU - dt + 1
  uint64_t t1 = nowU - dt - 1;
  double value1 = 77133.87;
  double average1 = sma.update({t1, value1});

  EXPECT_TRUE(belowError(0, average1));

  // Update within the window (+1)  |  nowU - dt + 1
  double value2 = 74516.41;
  uint64_t t2 = nowU - dt + 1;
  double average2 = sma.update({t2, value2});
  EXPECT_TRUE(belowError(value2, average2));

  double value3 = 79513.23;
  uint64_t t3 = t2 + 1; // once second passed 
  double average3 = sma.update({t3, value3});

  double expectedAverage = (value2 + value3) / 2;
  EXPECT_TRUE(belowError(average3, expectedAverage));
}