#include <core/producers/TradeEventProducer.hpp>
#include <core/events/TradeEvent.hpp>
#include <spdlog/spdlog.h>

namespace bb
{
  template <typename T>
  void bb::TradeEventProducer<T>::delegateEvent(const T& event)
  {
    spdlog::info("[delegateEvent] {}", event);
    m_eq.push(event);
  }

  template class TradeEventProducer<TradeEvent>;
} // bb