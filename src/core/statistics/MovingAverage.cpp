#include <core/statistics/MovingAverage.hpp>
#include <iostream>
#include <iomanip>

bb::MovingAverage::MovingAverage(uint64_t dt): 
  m_dt {dt},
  m_average {0},
  m_oldest_index {0}
{

};

bb::MovingAverage::~MovingAverage()
{
  std::cout << "MovingAverage::~MovingAverage" << std::endl;

  for (const std::pair<uint64_t, double> &p : m_buffer)
  {
    std::cout << "{" << p.first << ", " << std::fixed << std::setprecision(3) << p.second << "}" << std::endl;
  }
}