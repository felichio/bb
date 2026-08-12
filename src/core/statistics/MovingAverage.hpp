#pragma once
#include <vector>
#include <cstdint>

namespace bb
{
  class MovingAverage
  {
  public:
    MovingAverage(uint64_t dt);

  protected:
    std::vector<std::pair<uint16_t, double>> m_buffer;
    uint64_t m_dt;
  };
} // bb