#include <core/events/TradeEvent.hpp>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

class TradeEventTest : public testing::Test
{
};

TEST_F(TradeEventTest, TradeEventFromJson)
{
  nlohmann::json jsonEvent = nlohmann::json::parse(R"({
    "e": "trade",
    "E": 1672515782136,
    "s": "BNBBTC",
    "t": 12345,
    "p": "0.001",
    "q": "100",
    "T": 1672515782136,
    "m": true,
    "M": true
  })");

  bb::TradeEvent event = static_cast<bb::TradeEvent>(jsonEvent);

  EXPECT_EQ(event.type, jsonEvent["e"].get<std::string>());
  EXPECT_EQ(event.eventTime, jsonEvent["E"].get<uint64_t>());
  EXPECT_EQ(event.symbol, jsonEvent["s"].get<std::string>());
  EXPECT_EQ(event.tradeId, jsonEvent["t"].get<uint64_t>());
  EXPECT_EQ(event.price, jsonEvent["p"].get<std::string>());
  EXPECT_EQ(event.quantity, jsonEvent["q"].get<std::string>());
  EXPECT_EQ(event.tradeTime, jsonEvent["T"].get<uint64_t>());
  EXPECT_EQ(event.marketMaker, jsonEvent["m"].get<bool>());
}

TEST_F(TradeEventTest, TradeEventToJson)
{
  nlohmann::json jsonEventExpected = nlohmann::json::parse(R"({
    "e": "trade",
    "E": 1672515782136,
    "s": "BNBBTC",
    "t": 12345,
    "p": "0.001",
    "q": "100",
    "T": 1672515782136,
    "m": true,
    "M": true
  })");

  bb::TradeEvent event = static_cast<bb::TradeEvent>(jsonEventExpected);

  nlohmann::json jsonEventActual = event;

  EXPECT_EQ(jsonEventExpected, jsonEventActual);
}

TEST_F(TradeEventTest, TradeEventEquality)
{
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

  bb::TradeEvent event2 = static_cast<bb::TradeEvent>(nlohmann::json::parse(R"({
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

  bb::TradeEvent event3 = static_cast<bb::TradeEvent>(nlohmann::json::parse(R"({
    "e": "trade",
    "E": 16725157821361,
    "s": "BNBBTC",
    "t": 12345,
    "p": "0.001",
    "q": "100",
    "T": 1672515782136,
    "m": true,
    "M": true
  })"));

  EXPECT_TRUE(event1 == event2);
  EXPECT_FALSE(event1 == event3);
  EXPECT_EQ(event1, event2);
  EXPECT_NE(event1, event3);
}

TEST_F(TradeEventTest, TradeEventOrdering)
{
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

  bb::TradeEvent event2 = static_cast<bb::TradeEvent>(nlohmann::json::parse(R"({
    "e": "trade",
    "E": 1872515782136,
    "s": "BNBBTC",
    "t": 12345,
    "p": "0.001",
    "q": "100",
    "T": 1672515782136,
    "m": true,
    "M": true
  })"));

  // Time graph oldest first |  event1 -> event2
  EXPECT_TRUE(event2 < event1);
}

