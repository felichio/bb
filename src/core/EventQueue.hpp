#pragma once

#include <queue>
#include <vector>
#include <mutex>
#include <optional>
#include <condition_variable>
#include <core/consumers/IConsumer.hpp>

namespace bb
{

  template<typename T, int MAX_SLOTS>
  class EventQueue
  {
    public:
      using EventType = T;
      using reference = typename std::priority_queue<EventType, std::vector<EventType>>::reference;
      using const_reference = const EventType&;
      
      void push(const_reference event);
      void push(EventType&& event);

      std::optional<EventType> pop();

      void registerReceiver(IConsumer<T>*);

      void run();

      void stop();

    private:
      std::priority_queue<EventType, std::vector<EventType>> m_eventQueue;
      std::mutex m_mutex;
      std::condition_variable m_full;
      std::condition_variable m_empty;
      std::vector<IConsumer<T>*> m_receivers;
      bool m_stop {false};
  };
} // bb