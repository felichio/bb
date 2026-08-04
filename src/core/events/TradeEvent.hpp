#pragma once
#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace bb
{

  struct TradeEvent
  {
    std::string type;
    uint64_t eventTime;
    std::string symbol;
    uint64_t tradeId;
    std::string price;
    std::string quantity;
    uint64_t tradeTime;
    bool marketMaker;

    explicit TradeEvent(const nlohmann::json& te)
    {
      type = te["e"].get<std::string>();
      eventTime = te["E"].get<uint64_t>();
      symbol = te["s"].get<std::string>();
      tradeId = te["t"].get<uint64_t>();
      price = te["p"].get<std::string>();
      quantity = te["q"].get<std::string>();
      tradeTime = te["T"].get<uint64_t>();
      marketMaker = te["m"].get<bool>();
    }

    operator nlohmann::json() const
    {
      nlohmann::json j;
      j["e"] = this->type;
      j["E"] = this->eventTime;
      j["s"] = this->symbol;
      j["t"] = this->tradeId;
      j["p"] = this->price;
      j["q"] = this->quantity;
      j["T"] = this->tradeTime;
      j["m"] = this->marketMaker;
      j["M"] = true; // ignored value
      return j;
    }

    bool operator==(const TradeEvent &other) const
    {
      return type        == other.type      &&
             eventTime   == other.eventTime &&
             symbol      == other.symbol    &&
             tradeId     == other.tradeId   &&
             price       == other.price     &&
             tradeTime   == other.tradeTime &&
             marketMaker == other.marketMaker;
    }

    bool operator<(const TradeEvent &other) const
    {
      return eventTime > other.eventTime;
    }

  };

} // bb

template <>
struct fmt::formatter<bb::TradeEvent> : fmt::formatter<std::string>
{
  auto format(bb::TradeEvent event, format_context &ctx) const -> decltype(ctx.out())
  {
    return fmt::format_to(ctx.out(), "type: {}  |  symbol: {}", event.type, event.symbol);
  }
};