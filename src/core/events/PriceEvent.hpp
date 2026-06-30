#pragma once

#include <string>
#include <cstdint>
#include <iostream>

namespace bb
{
  struct PriceEvent
  {
    std::string type = "PriceEvent";
    uint32_t payload;
    PriceEvent(uint32_t data) : payload{data}
    {
    }

    bool operator==(const PriceEvent &other) const
    {
      return type == other.type && payload == other.payload;
    }
  };

  inline std::ostream &operator<<(std::ostream &out, const PriceEvent &event)
  {
    out << event.type <<  " with payload -> " << event.payload << std::endl;
    return out;
  }

  inline bool operator<(const PriceEvent& ev1, const PriceEvent& ev2)
  {
    return ev1.payload < ev2.payload;
  }

} // bb