#include <core/consumers/TradeEventConsumer.hpp>
#include <core/events/TradeEvent.hpp>
#include <core/statistics/MovingAverage.hpp>
#include <memory>
#include <nlohmann/json.hpp>

namespace bb
{
  template <typename T>
  void TradeEventConsumer<T>::receiveEvent(const T &event)
  {
    spdlog::info("[receiveEvent] {}", static_cast<nlohmann::json>(event).dump());

    for (const std::unique_ptr<MovingAverage>& ma : m_attachments)
    {
      ma->update({event.tradeTime, std::stod(event.price)});
    }
  }

  template <typename T>
  void TradeEventConsumer<T>::attach(std::unique_ptr<MovingAverage> attachment)
  {
    m_attachments.push_back(std::move(attachment));
  }

  template class TradeEventConsumer<TradeEvent>;
} // bb