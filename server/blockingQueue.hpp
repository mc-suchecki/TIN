/// @author Jacek Witkowski
/// 
/// Definition of blocking queue called MessageQueue.

#pragma once

#include <queue>
#include <deque>
#include <boost/thread.hpp>

/// Blocking queue: blocks on pop() when is empty.
template <class T, class Container = std::deque<T*> >
class BlockingQueue {
  public:
    void push(T *elem){
      {
        boost::mutex::scoped_lock lock(mut);
        queue.push(elem);
      }
      empty.notify_one();
    }

    T* pop(){
      boost::mutex::scoped_lock lock(mut);
      while(queue.empty())
        empty.wait(lock);

      T *resultPtr = queue.front();
      queue.pop();

      return resultPtr;
    }

    bool isEmpty() const{
      boost::mutex::scoped_lock lock(mut);
      return queue.empty();
    }

  private:
    std::queue<T*,Container> queue;
    mutable boost::mutex mut;
    boost::condition_variable empty; 
};
