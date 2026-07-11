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

int main(int argc, char *argv[])
{
  boost::asio::io_context io_context;
  boost::beast::flat_buffer buffer;

  nlohmann::json t = {
      {"kappa", "alg"}};

  std::cout << t.dump() << std::endl;
  spdlog::info("OK!");

  // bb::EnvReader::getReader()->populateEnv();

  spdlog::info("Env: " + bb::EnvReader::getReader()->getKey("SECRET_TEST"));
  spdlog::info("Env: " + bb::EnvReader::getReader()->getKey("WEBSOCKET_MARKET_STREAM_TEST_URL"));
  spdlog::info("Env: " + bb::EnvReader::getReader()->getKey("API_KEY_TEST"));
  spdlog::info("Env: " + bb::EnvReader::getReader()->getKey("WEBSOCKET_STREAM_HOST"));
  spdlog::info("Env: " + bb::EnvReader::getReader()->getKey("WEBSOCKET_STREAM_PORT"));

  bb::EventQueue<bb::TradeEvent, 5> eventQueue;
  bb::WebSocketClient<bb::TradeEvent> wsc;
  bb::TradeEventProducer<bb::TradeEvent> btcP(eventQueue, wsc);
  
}