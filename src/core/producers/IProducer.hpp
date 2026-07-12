#pragma once

namespace bb
{
  template <typename T>
  class IProducer
  {
  public:
    virtual void preProduce() {}
    virtual void produce(const T &event)
    {
      preProduce();
      delegateEvent(event);
      postProduce();
    }
    virtual void postProduce() {}

    virtual void delegateEvent(const T &event) = 0;
  };
} // bb