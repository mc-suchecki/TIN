#pragma once

#include "events/event.hpp"
#include <boost/thread.hpp>
#include <queue>

class EventQueue {
  public:
    void push(Event inputEvent);
    Event pop();

    bool isEmpty();

    boost::condition_variable cond; 
    boost::mutex mut;

  private:
    std::queue<Event> queue;
    boost::shared_mutex queueMutex;
};
