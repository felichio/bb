#pragma once
#include <core/consumers/IConsumer.hpp>
#include <core/statistics/MovingAverage.hpp>
#include <memory>

namespace bb
{
  template <typename T>
  class TradeEventConsumer : public IConsumer<T>
  {
    public:
    TradeEventConsumer() = default;
    void attach(std::unique_ptr<MovingAverage>);

    private:
    virtual void receiveEvent(const T &event) override;
    

    std::vector<std::unique_ptr<MovingAverage>> m_attachments;
  };


} // bb