#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <mutex>
#include <condition_variable>
#include <deque>

// Message queue to pass information between threads

template<typename T>
class MessageQueue {
  public:
  void send(T msg);
  T receive();

  private:
  std::deque<T> _queue;
  std::mutex _mtx;
  std::condition_variable _cond;

};

template<typename T>
void MessageQueue<T>::send(T msg) {
  
  std::lock_guard<std::mutex> lck(_mtx);
  _queue.push_back(msg);
  _cond.notify_one();

}

template<typename T>
T MessageQueue<T>::receive(){
  std::unique_lock<std::mutex> uLck(_mtx);
  _cond.wait(uLck, [this] {return !_queue.empty();});
  T t = _queue.front();
  _queue.pop_front();
  return t;
}

#endif