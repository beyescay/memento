#ifndef FORGETTING_CURVE_H
#define FORGETTING_CURVE_H

#include <chrono>
#include <deque>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "message_queue.h"

using namespace std::chrono_literals;

//Leitner system based Forgetting curve
enum RepititionNum {
  Start = 0,
  One,
  Two,
  Three,
  Four,
  End
};

class ForgettingCurve {

  public:
  //Constructor
  ForgettingCurve();

  RepititionNum getCurrentRepitition();
  
  void notify();
  
  void waitForNotification();

  private:
  std::chrono::seconds _time_elapsed{0s};
  MessageQueue<RepititionNum> _message_q;
  std::mutex _mtx;

  float _retention = 1.0;
  RepititionNum _repitition_n;
  static constexpr int _k_rep_interval_s = 1;


}; 

#endif