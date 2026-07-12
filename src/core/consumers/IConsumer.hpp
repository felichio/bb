#pragma once

namespace bb
{
  template <typename T>
  class IConsumer
  {
  public:
    virtual void preConsume() {}
    virtual void consume(const T &event)
    {
      preConsume();
      receiveEvent(event);
      postConsume();
    }
    virtual void postConsume() {}

    virtual void receiveEvent(const T &event) = 0;
  };
} // bb