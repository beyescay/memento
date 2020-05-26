#ifndef FORGETTING_CURVE_H
#define FORGETTING_CURVE_H

#include <chrono>
#include <mutex>
#include <thread>
#include <vector>

#include "message_queue.h"


class ForgettingCurve {

  public:
  // Constructor
  ForgettingCurve();
  
  // Control Functions
  void start();
  void notify();
  void waitForNotification();

  // Getters
  int getCurrentRep() {return _rep_num;}
  static int getMaxReps() {return _k_max_reps;}

  float getRetention() {return _retention;}
  float getMinRetention(int rep_num);

  int getTotalElapsedTime() {return _total_time_elapsed;}

  // Update functions
  void updateRetention();
  void updateStability();

  //Destructor
  ~ForgettingCurve();

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