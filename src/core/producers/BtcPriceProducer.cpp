#include <core/producers/BtcPriceProducer.hpp>
#include <core/events/PriceEvent.hpp>
#include <spdlog/spdlog.h>

namespace bb
{
  template <typename T>
  void bb::BtcPriceProducer<T>::delegateEvent(const T& event)
  {
    spdlog::info("[delegateEvent] {}", event);
    m_eq.push(event);
  }

  template class BtcPriceProducer<PriceEvent>;
} // bb