#include <core/EventQueue.hpp>
#include <core/events/PriceEvent.hpp>
#include <iostream>

namespace bb
{
  template <typename T, int MAX_SLOTS>
  void EventQueue<T, MAX_SLOTS>::push(EventQueue::EventType &&event)
  {
    std::cout << "push action " << event << std::endl;
    std::unique_lock<std::mutex> lk(m_mutex);
    m_full.wait(lk, [this] { return (m_eventQueue.size() < MAX_SLOTS); });
    m_eventQueue.push(event);
    m_empty.notify_one();
  }

  template <typename T, int MAX_SLOTS>
  void EventQueue<T, MAX_SLOTS>::push(EventQueue::const_reference event)
  {
    return this->push(std::move(event));
  }

  template <typename T, int MAX_SLOTS>
  typename EventQueue<T, MAX_SLOTS>::EventType EventQueue<T, MAX_SLOTS>::pop()
  {
    std::cout << "pop action" << std::endl;
    std::unique_lock<std::mutex> lk(m_mutex);
    m_empty.wait(lk, [this] { return (m_eventQueue.size() != 0); });
    EventQueue<T, MAX_SLOTS>::EventType eventType = m_eventQueue.top();
    m_eventQueue.pop();
    m_full.notify_one();
    return eventType;
  }

  template class EventQueue<PriceEvent, 5>;

  

} // b