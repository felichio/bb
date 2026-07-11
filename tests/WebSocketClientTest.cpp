#include <core/wsclient/WebSocketClient.hpp>
#include <core/events/TradeEvent.hpp>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <mocks/IProducerMock.hpp>

class WebSocketClientTest : public testing::Test
{
};

TEST_F(WebSocketClientTest, WebSocketClientConnect)
{
  bb::WebSocketClient<bb::TradeEvent> wsc;
  EXPECT_NO_THROW(wsc.connect());
}

TEST_F(WebSocketClientTest, WebSocketClientRegister)
{
  bb::WebSocketClient<bb::TradeEvent> wsc;
  bb::IProducerMock<bb::TradeEvent> producerMock;
  wsc.registerProducer(&producerMock);
}