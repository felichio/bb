#include <gtest/gtest.h>
#include <core/statistics/MovingAverage.hpp>
#include <core/statistics/SimpleMovingAverage.hpp>
#include <core/statistics/TimeLiterals.hpp>
#include <chrono>

class SimpleMovingAverageTest : public testing::Test
{
};

TEST_F(SimpleMovingAverageTest, SimpleMovingAverageTestUpdate)
{
  uint64_t dt = 15_s;
  bb::SimpleMovingAverage sma(dt);
  std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  uint64_t nowU = now.count();

  // Update within the window (+1)  |  nowU - dt + 1
  double expected = 77133.87;
  double actual = sma.update({nowU - dt + 1, expected});

  EXPECT_EQ(expected, actual);
}