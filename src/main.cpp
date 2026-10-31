#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/core.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <util/EnvReader.hpp>
#include <core/EventQueue.hpp>
#include <core/events/TradeEvent.hpp>
#include <core/producers/TradeEventProducer.hpp>
#include <core/wsclient/WebSocketClient.hpp>
#include <core/consumers/TradeEventConsumer.hpp>
#include <core/statistics/MovingAverage.hpp>
#include <core/statistics/SimpleMovingAverage.hpp>
#include <core/statistics/TimeLiterals.hpp>
#include <memory>

void init()
{
  #ifdef LOG_LEVEL
  spdlog::set_level(spdlog::level::LOG_LEVEL);
  #endif

  spdlog::debug("Env: " + bb::EnvReader::getReader()->getKey("SECRET_TEST"));
  spdlog::debug("Env: " + bb::EnvReader::getReader()->getKey("WEBSOCKET_MARKET_STREAM_TEST_URL"));
  spdlog::debug("Env: " + bb::EnvReader::getReader()->getKey("API_KEY_TEST"));
  spdlog::debug("Env: " + bb::EnvReader::getReader()->getKey("WEBSOCKET_STREAM_HOST"));
  spdlog::debug("Env: " + bb::EnvReader::getReader()->getKey("WEBSOCKET_STREAM_PORT"));
}

int main(int argc, char *argv[])
{
  
  init();
  bb::EventQueue<bb::TradeEvent, 5> eventQueue;
  bb::TradeEventConsumer<bb::TradeEvent> tec;

  eventQueue.registerReceiver(&tec);

  tec.attach(std::unique_ptr<bb::MovingAverage>(new bb::SimpleMovingAverage(15_s)));

  std::thread t([&eventQueue] {
    eventQueue.run();
  });

  bb::WebSocketClient<bb::TradeEvent> wsc;
  bb::TradeEventProducer<bb::TradeEvent> btcP(eventQueue, wsc);

  std::thread w([&btcP] {
    btcP.init();
  });

  spdlog::warn("[main thread] sleeping for 5s");
  std::this_thread::sleep_for(std::chrono::seconds(5));
  wsc.stop();
  w.join();
  eventQueue.stop();
  t.join();

  spdlog::warn("[main thread] Total events: {}", bb::TradeEventProducer<bb::TradeEvent>::eventCounter);

  spdlog::warn("[main thread] exiting..");
}