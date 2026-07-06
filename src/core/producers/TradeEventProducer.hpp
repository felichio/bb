#include <core/producers/IProducer.hpp>
#include <core/wsclient/WebSocketClient.hpp>
#include <core/EventQueue.hpp>
#include <nlohmann/json.hpp>

namespace bb
{
  template <typename T>
  class TradeEventProducer : public IProducer<T>
  {
  public:
    TradeEventProducer(bb::EventQueue<T, 5> &eventQueue, bb::WebSocketClient<T> &webSocketClient)
        : m_eq{eventQueue},
          m_wsc{webSocketClient}
    {
      m_wsc.registerProducer(this);

      m_wsc.connect();
      nlohmann::json intent = nlohmann::json::parse(R"({
        "method" : "SUBSCRIBE",
        "params" : ["btcusdt@trade"],
        "id" : "1"
      })");
      m_wsc.subscribe(intent);
      m_wsc.run();
    }

  private:
    bb::EventQueue<T, 5> &m_eq;
    bb::WebSocketClient<T> &m_wsc;
    virtual void delegateEvent(const T &event) override;
  };
} // bb