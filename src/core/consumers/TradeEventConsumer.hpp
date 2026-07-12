#pragma once
#include <core/consumers/IConsumer.hpp>

namespace bb
{
  template <typename T>
  class TradeEventConsumer : public IConsumer<T>
  {
    public:
    TradeEventConsumer() = default;

    private:
    virtual void receiveEvent(const T &event) override;
  };


} // bb