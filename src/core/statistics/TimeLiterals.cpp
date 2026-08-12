#include <core/statistics/TimeLiterals.hpp>

uint64_t operator"" _m(unsigned long long minutes)
{
  return minutes * 60;
}

uint64_t operator"" _h(unsigned long long hours)
{
  return  operator""_m(hours * 60);
}

uint64_t operator"" _d(unsigned long long days)
{
  return  operator""_h(days * 24);
}