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
    TradeEventProducer(bb::EventQueue<T, 5> &eventQueue, bb::WebSocketClient<T> &webSocketClient);
    void init();

  private:
    bb::EventQueue<T, 5> &m_eq;
    bb::WebSocketClient<T> &m_wsc;
    virtual void delegateEvent(const T &event) override;
    

    static nlohmann::json intent;
  };
} // bb