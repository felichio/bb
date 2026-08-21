#pragma once
#include <core/statistics/MovingAverage.hpp>

namespace bb
{
  class SimpleMovingAverage : public MovingAverage
  {
    public:
      SimpleMovingAverage(uint64_t dt);

      double update(const std::pair<uint64_t, double>&) override;
      private:
      double sum(const std::vector<std::pair<uint64_t, double>>&) const;
  };
} // bb