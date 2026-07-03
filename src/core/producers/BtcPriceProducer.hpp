#include <core/producers/IProducer.hpp>
#include <core/EventQueue.hpp>

namespace bb
{
  template <typename T>
  class BtcPriceProducer : public IProducer<T>
  {
  public:
    BtcPriceProducer(bb::EventQueue<T, 5> &eventQueue) : m_eq{eventQueue} {}
    
    
  private:
    bb::EventQueue<T, 5> &m_eq;
    virtual void delegateEvent(const T &event) override;
  };
} // bb