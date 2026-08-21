#include <future>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include <core/EventQueue.hpp>
#include <core/events/TradeEvent.hpp>
#include <nlohmann/json.hpp>
#include <vector>
#include <semaphore>

class EventQueueTest : public testing::Test
{
protected:
  bb::EventQueue<bb::TradeEvent, 5> m_eq;

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

TEST_F(EventQueueTest, PushTradeEventAndPop)
{
  std::promise<std::optional<bb::TradeEvent>> actualPromise;
  std::thread t1([&]() {
    const std::optional<bb::TradeEvent> actual = m_eq.pop();
    actualPromise.set_value(actual);
  });

  bb::TradeEvent expected = static_cast<bb::TradeEvent>(nlohmann::json::parse(R"({
    "e": "trade",
    "E": 1672515782136,
    "s": "BNBBTC",
    "t": 12345,
    "p": "0.001",
    "q": "100",
    "T": 1672515782136,
    "m": true,
    "M": true
  })"));

  m_eq.push(expected);
  const std::optional<bb::TradeEvent> actual = actualPromise.get_future().get();
  t1.join();
  EXPECT_EQ(expected, actual);
}

TEST_F(EventQueueTest, TradeEventWeightOrder)
{
  const size_t numberOfEvents = 3;
  std::vector<bb::TradeEvent> expected;
  expected.reserve(numberOfEvents);

  // event 1
  bb::TradeEvent event1 = static_cast<bb::TradeEvent>(nlohmann::json::parse(R"({
    "e": "trade",
    "E": 1672515782136,
    "s": "BNBBTC",
    "t": 12345,
    "p": "0.001",
    "q": "100",
    "T": 1672515782136,
    "m": true,
    "M": true
  })"));

  // event 2
  bb::TradeEvent event2 = static_cast<bb::TradeEvent>(nlohmann::json::parse(R"({
    "e": "trade",
    "E": 1678515782136,
    "s": "BNBBTC",
    "t": 12345,
    "p": "0.001",
    "q": "100",
    "T": 1672515782136,
    "m": true,
    "M": true
  })"));

  // event 3
  bb::TradeEvent event3 = static_cast<bb::TradeEvent>(nlohmann::json::parse(R"({
    "e": "trade",
    "E": 1698515782136,
    "s": "BNBBTC",
    "t": 12345,
    "p": "0.001",
    "q": "100",
    "T": 1672515782136,
    "m": true,
    "M": true
  })"));

  // Time order for events oldest -> newest  (event1 -> event2 -> event3)
  expected.push_back(event1);
  expected.push_back(event2);
  expected.push_back(event3);


  // actual vector filled from pops
  std::vector<bb::TradeEvent> actual;
  actual.reserve(numberOfEvents);

  // cv for signaling end of main thread (not needed)
  std::binary_semaphore pushesFinished(0);
  // Thread running pops
  std::thread t([&actual, &pushesFinished, this] () {
    pushesFinished.acquire();
    for (size_t i = 0; i < numberOfEvents; i++)
    {
      std::optional<bb::TradeEvent> tradeEvent = this->m_eq.pop();
      if (tradeEvent)
        actual.push_back(tradeEvent.value());
    }
  });

  // push the events from main test thread
  for (std::vector<bb::TradeEvent>::iterator it = expected.begin(); it != expected.end(); it++)
  {
    m_eq.push(*it);
  }

  // signal finish
  pushesFinished.release();

  // wait for pops to happen
  t.join();

  // order should be the same
  EXPECT_EQ(expected, actual);

}