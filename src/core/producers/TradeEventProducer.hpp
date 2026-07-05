#include <core/producers/IProducer.hpp>
#include <core/EventQueue.hpp>

namespace bb
{
  template <typename T>
  class TradeEventProducer : public IProducer<T>
  {
  public:
    TradeEventProducer(bb::EventQueue<T, 5> &eventQueue) : m_eq{eventQueue} {}
    
    
  private:
    bb::EventQueue<T, 5> &m_eq;
    virtual void delegateEvent(const T &event) override;
  };
} // bb