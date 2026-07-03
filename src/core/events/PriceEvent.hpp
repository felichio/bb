#pragma once

#include <string>
#include <cstdint>
#include <iostream>
#include <spdlog/spdlog.h>

namespace bb
{
  struct PriceEvent
  {
    std::string tag = "PriceEvent";
    uint32_t payload;
    PriceEvent(uint32_t data) : payload{data}
    {
    }

    bool operator==(const PriceEvent &other) const
    {
      return tag == other.tag && payload == other.payload;
    }
  };

  inline std::ostream &operator<<(std::ostream &out, const PriceEvent &event)
  {
    out << event.tag << " with payload -> " << event.payload << std::endl;
    return out;
  }

  inline bool operator<(const PriceEvent &ev1, const PriceEvent &ev2)
  {
    return ev1.payload < ev2.payload;
  }

} // bb

template <>
struct fmt::formatter<bb::PriceEvent> : fmt::formatter<std::string>
{
  auto format(bb::PriceEvent event, format_context &ctx) const -> decltype(ctx.out())
  {
    return fmt::format_to(ctx.out(), "tag: {}  |  price: {}", event.tag, event.payload);
  }
};
