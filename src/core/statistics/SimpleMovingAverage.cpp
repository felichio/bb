#include <core/statistics/SimpleMovingAverage.hpp>
#include <spdlog/spdlog.h>

bb::SimpleMovingAverage::SimpleMovingAverage(uint64_t dt): MovingAverage(dt) {};

void bb::SimpleMovingAverage::update(const std::pair<uint64_t, double>& item)
{
  spdlog::info("[SimpleMovingAverage] updating..");
  m_buffer.push_back(item);
}