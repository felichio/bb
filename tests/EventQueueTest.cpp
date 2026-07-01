#include <future>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include <core/EventQueue.hpp>
#include <core/events/PriceEvent.hpp>

class EventQueueTest : public testing::Test
{
protected:
  bb::EventQueue<bb::PriceEvent, 5> m_eq;

  EventQueueTest()
  {

    spdlog::info("EventQueueTest()");
  }

  ~EventQueueTest()
  {
    spdlog::info("~EventQueueTest()");
  }

  void SetUp()
  {
    spdlog::info("SetUp()");
  }

  void TearDown()
  {
    spdlog::info("TearDown()");
  }
};

TEST_F(EventQueueTest, some)
{
  std::promise<bb::PriceEvent> actualPromise;
  std::thread t1([&] () {
    const bb::PriceEvent actual = m_eq.pop();
    actualPromise.set_value(actual);
  });
  bb::PriceEvent expected {1};
  m_eq.push(expected);
  const bb::PriceEvent actual = actualPromise.get_future().get();
  t1.join();
  EXPECT_EQ(expected, actual);
}

TEST_F(EventQueueTest, another)
{
  EXPECT_EQ(3, 3);
}