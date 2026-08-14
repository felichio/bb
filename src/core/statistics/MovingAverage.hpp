#pragma once
#include <vector>
#include <cstdint>

namespace bb
{
  class MovingAverage
  {
  public:
    MovingAverage(uint64_t dt);
    virtual ~MovingAverage();
    virtual void update(const std::pair<uint64_t, double>&) = 0;

  protected:
    std::vector<std::pair<uint64_t, double>> m_buffer;
    uint64_t m_dt;
  };
} // bb