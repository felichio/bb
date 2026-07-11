#include <gmock/gmock.h>
#include <core/producers/IProducer.hpp>


namespace bb
{
  template <typename T>
  class IProducerMock : public IProducer<T>
  {
    MOCK_METHOD(void, preProduce, (), (override));
    MOCK_METHOD(void, produce, (const T&), (override));
    MOCK_METHOD(void, postProduce, (), (override));
    MOCK_METHOD(void, delegateEvent, (const T&), (override));
  };
} // bb