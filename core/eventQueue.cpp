#include "../include/eventQueue.hpp"
#include "../include/events/event.hpp"

void EventQueue::push(Event inputEvent) {
  boost::lock_guard<boost::mutex> cond_lock(mut);
  boost::unique_lock<boost::shared_mutex> lock(queueMutex);

  queue.push(inputEvent);
  cond.notify_all();

  return;
}

Event EventQueue::pop() {
  Event returnEvent;

  boost::unique_lock<boost::shared_mutex> lock(queueMutex);
  returnEvent = queue.front();
  queue.pop();

  return returnEvent;
}

bool EventQueue::isEmpty() {
  boost::shared_lock<boost::shared_mutex> lock(queueMutex);
  return queue.empty();
}  

