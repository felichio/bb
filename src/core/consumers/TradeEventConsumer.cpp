#include <core/consumers/TradeEventConsumer.hpp>
#include <core/events/TradeEvent.hpp>
#include <nlohmann/json.hpp>

namespace bb
{
  template <typename T>
  void TradeEventConsumer<T>::receiveEvent(const T &event)
  {
    spdlog::info("[receiveEvent] {}", static_cast<nlohmann::json>(event).dump());
  }

  template class TradeEventConsumer<TradeEvent>;
} // bb