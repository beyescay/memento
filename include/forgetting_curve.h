#ifndef FORGETTING_CURVE_H
#define FORGETTING_CURVE_H

#include <chrono>
#include <deque>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

#include "message_queue.h"

using namespace std::chrono_literals;

/*Leitner system based Forgetting curve
enum RepititionNum {
  Start = 0,
  One,
  Two,
  Three,
  Four,
  End
};
*/

class ForgettingCurve {

  public:
  //Constructor
  ForgettingCurve();

  ~ForgettingCurve();
 
  void start();

  void notify();
  
  void waitForNotification();

  int getCurrentRep();

  static int getMaxReps(); 

  float getRetention();

  float getMinRetention(int rep_num);

  void updateRetention();

  void updateStability();

  int getTotalElapsedTime();

  private:
  MessageQueue<int> _message_q;
  std::mutex _mtx;
  std::vector<std::thread> _threads;

  // Forgetting curve parameters
  int _interval_time_elapsed = 0;
  int _total_time_elapsed = 0;
  float _stability = 0.87;
  float _retention;
  
  int _rep_num = 0;
  static constexpr int _k_max_reps = 4;


}; 

#endif