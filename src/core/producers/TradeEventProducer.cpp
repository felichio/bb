#include <core/producers/TradeEventProducer.hpp>
#include <core/events/TradeEvent.hpp>
#include <spdlog/spdlog.h>

namespace bb
{

  template <typename T>
  TradeEventProducer<T>::TradeEventProducer(bb::EventQueue<T, 5> &eventQueue, bb::WebSocketClient<T> &webSocketClient)
      : m_eq{eventQueue},
        m_wsc{webSocketClient}
  {
    m_wsc.registerProducer(this);
    m_wsc.connect();
    m_wsc.subscribe(TradeEventProducer::intent);
    m_wsc.run();
  }

  template <typename T>
  void bb::TradeEventProducer<T>::delegateEvent(const T &event)
  {
    spdlog::info("[delegateEvent] {}", event);
    m_eq.push(event);
  }

  template <typename T>
  nlohmann::json TradeEventProducer<T>::intent = nlohmann::json::parse(R"({
        "method" : "SUBSCRIBE",
        "params" : ["btcusdt@trade"],
        "id" : "1"
      })");

  template class TradeEventProducer<TradeEvent>;
} // bb