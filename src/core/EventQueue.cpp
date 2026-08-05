#include <core/EventQueue.hpp>
#include <core/events/TradeEvent.hpp>
#include <spdlog/spdlog.h>

namespace bb
{
  template <typename T, int MAX_SLOTS>
  void EventQueue<T, MAX_SLOTS>::push(EventQueue::EventType &&event)
  {
    std::unique_lock<std::mutex> lk(m_mutex);
    m_full.wait(lk, [this]
                { return (m_eventQueue.size() < MAX_SLOTS); });
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
  typename EventQueue<T, MAX_SLOTS>::EventType EventQueue<T, MAX_SLOTS>::pop()
  {
    std::unique_lock<std::mutex> lk(m_mutex);
    m_empty.wait(lk, [this]
                 { return (m_eventQueue.size() != 0); });
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
      EventQueue<T, MAX_SLOTS>::EventType event = this->pop(); // blocking
      for (typename std::vector<IConsumer<T> *>::iterator it = m_receivers.begin(); it != m_receivers.end(); it++)
      {
        (*it)->receiveEvent(event);
      }
    }
  }

  template class EventQueue<TradeEvent, 5>;

} // bb