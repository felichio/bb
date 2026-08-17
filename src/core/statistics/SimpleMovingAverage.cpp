#include <core/statistics/SimpleMovingAverage.hpp>
#include <spdlog/spdlog.h>
#include <chrono>
#include <iostream>

bb::SimpleMovingAverage::SimpleMovingAverage(uint64_t dt): MovingAverage(dt) {};

void bb::SimpleMovingAverage::update(const std::pair<uint64_t, double>& item)
{
  spdlog::info("[SimpleMovingAverage] updating..");
  std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  uint64_t nowU = now.count();
  uint64_t window = nowU - m_dt;
  m_buffer.push_back(item);
  size_t n = m_buffer.size() - 1 - m_oldest_index; // previous count

  std::vector<std::pair<uint64_t, double>> removed;

  while (m_oldest_index < m_buffer.size() && m_buffer[m_oldest_index].first < window)
  {
    spdlog::info("m_buffer[m_oldest_index].first:  {}    window:   {}", m_buffer[m_oldest_index].first, window);
    removed.push_back(m_buffer[m_oldest_index]);
    m_oldest_index++;
  }

  m_average = (n * m_average + item.second - sum(removed)) / (n - removed.size() + 1);
  spdlog::info("average: {}", m_average);
}

double bb::SimpleMovingAverage::sum(const std::vector<std::pair<uint64_t, double>>& items)
{
  double s = 0;
  std::cout << "sum running" << std::endl;
  for (auto i = items.cbegin(); i != items.cend(); i++)
  {
    s += i->second;
  }
  return s;
}