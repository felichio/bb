#pragma once
#include <cstdint>

uint64_t operator"" _s(unsigned long long seconds);
uint64_t operator"" _m(unsigned long long minutes);
uint64_t operator"" _h(unsigned long long hours);
uint64_t operator"" _d(unsigned long long days);
