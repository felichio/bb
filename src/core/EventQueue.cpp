#include <core/EventQueue.hpp>
#include <core/events/TradeEvent.hpp>
#include <spdlog/spdlog.h>
#include <optional>
#include <iostream>

namespace bb
{
  template <typename T, int MAX_SLOTS>
  void EventQueue<T, MAX_SLOTS>::push(EventQueue::EventType &&event)
  {
    std::unique_lock<std::mutex> lk(m_mutex);
    m_full.wait(lk, [this]
                { return (m_eventQueue.size() < MAX_SLOTS) || m_stop; });
    if (m_stop) return;
    m_eventQueue.push(std::move(event));
    m_empty.notify_one();
  }

  template <typename T, int MAX_SLOTS>
  void EventQueue<T, MAX_SLOTS>::push(EventQueue::const_reference event)
  {
    std::unique_lock<std::mutex> lk(m_mutex);
    m_full.wait(lk, [this]
                { return (m_eventQueue.size() < MAX_SLOTS); });
    m_eventQueue.push(event);
    m_empty.notify_one();
  }

  template <typename T, int MAX_SLOTS>
  typename std::optional<typename EventQueue<T, MAX_SLOTS>::EventType> EventQueue<T, MAX_SLOTS>::pop()
  {
    std::unique_lock<std::mutex> lk(m_mutex);
    m_empty.wait(lk, [this]
                 { return (m_eventQueue.size() != 0) || m_stop; });

    if (m_stop)
      return std::nullopt;

    EventQueue<T, MAX_SLOTS>::EventType event = m_eventQueue.top();
    m_eventQueue.pop();
    m_full.notify_one();
    return event;
  }

  template <typename T, int MAX_SLOTS>
  void EventQueue<T, MAX_SLOTS>::registerReceiver(IConsumer<T> *consumer)
  {
    m_receivers.push_back(consumer);
  }

  template <typename T, int MAX_SLOTS>
  void EventQueue<T, MAX_SLOTS>::run()
  {
    spdlog::info("Running EventQueue dispatcher loop");
    while (true)
    {
      if (auto event = this->pop() /* blocking*/)
      {
        for (typename std::vector<IConsumer<T> *>::iterator it = m_receivers.begin(); it != m_receivers.end(); it++)
        {
          (*it)->receiveEvent(event.value());
        }
      }
      else
      {
        std::cout << "breaking" << std::endl;
        break;
      }
    }
  }

  template <typename T, int MAX_SLOTS>
  void EventQueue<T, MAX_SLOTS>::stop()
  {
    std::unique_lock<std::mutex> lk(m_mutex);
    m_stop = true;
    m_empty.notify_all();
    m_full.notify_all();
  }

  template class EventQueue<TradeEvent, 5>;

} // bb