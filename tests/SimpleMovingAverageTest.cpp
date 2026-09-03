#include <gtest/gtest.h>
#include <core/statistics/MovingAverage.hpp>
#include <core/statistics/SimpleMovingAverage.hpp>
#include <core/statistics/TimeLiterals.hpp>
#include <chrono>
#include <cmath>
#include <thread>



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

  // Update within the window (+1s)  |  nowU - dt + 1000 ms
  uint64_t t1 = nowU - dt + 1000;
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

  // Update within the window (+1s)  |  nowU - dt + 1000 ms
  double value1 = 77133.87;
  uint64_t t1 = nowU - dt + 1;
  sma.update({t1, value1});
  double value2 = 77716.19;
  uint64_t t2 = t1 + 1000; // one second passed
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

  // Update within the window (-1s)  |  nowU - dt - 1000 ms
  uint64_t t1 = nowU - dt - 1000;
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

  // Update out of window (-1s)  |  nowU - dt - 1000 ms
  uint64_t t1 = nowU - dt - 1000;
  double value1 = 77133.87;
  double average1 = sma.update({t1, value1});

  EXPECT_TRUE(belowError(0, average1));

  // Update within the window (+1s)  |  nowU - dt + 1000 ms
  double value2 = 74516.41;
  uint64_t t2 = nowU - dt + 1000;
  double average2 = sma.update({t2, value2});
  EXPECT_TRUE(belowError(value2, average2));

  // Update within the window (+2s)  |  nowU - dt + 2000 ms
  double value3 = 79513.23;
  uint64_t t3 = t2 + 1000; // once second passed 
  double average3 = sma.update({t3, value3});

  double expectedAverage = (value2 + value3) / 2;
  EXPECT_TRUE(belowError(average3, expectedAverage));
}

TEST_F(SimpleMovingAverageTest, SimpleMovingAverageTestThreeUpdateWaitForOutOfWindow)
{
  uint64_t dt = 3_s;
  bb::SimpleMovingAverage sma(dt);
  std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  uint64_t nowU = now.count();

  // Update out of window (+1s)  |  nowU - dt + 1000 ms
  uint64_t t1 = nowU - dt + 1000;
  double value1 = 77133.87;
  double average1 = sma.update({t1, value1});

  EXPECT_TRUE(belowError(average1, value1));

  // Update within the window (+2s)  |  nowU - dt + 2000 ms
  double value2 = 74516.41;
  uint64_t t2 = nowU - dt + 2000;
  double average2 = sma.update({t2, value2});
  double expectedAverage2 = (value1 + value2) / 2;
  EXPECT_TRUE(belowError(average2, expectedAverage2));

  // Update within the window (+2.5s)  |  nowU - dt + 2500 ms
  double value3 = 79513.23;
  uint64_t t3 = nowU - dt + 2500; // once second passed 
  double average3 = sma.update({t3, value3});

  double expectedAverage3 = (value1 + value2 + value3) / 3;
  EXPECT_TRUE(belowError(average3, expectedAverage3));

  // sleep for 3s to stale the entries. A new update should flush the old elements
  std::this_thread::sleep_for(std::chrono::seconds(3));

  double value4 = 81315.16;
  now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  nowU = now.count();
  uint64_t t4 = nowU;

  double average4 = sma.update({t4, value4});
  EXPECT_TRUE(belowError(average4, value4));
}