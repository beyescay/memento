#ifndef FORGETTING_CURVE_H
#define FORGETTING_CURVE_H

#include <chrono>
#include <mutex>
#include <thread>
#include <vector>

#include "message_queue.h"
#include "knowledge.h"

class BaseKnowledge;

class ForgettingCurve {

  public:
  // Constructor
  ForgettingCurve();
  
  // Control Functions
  void start();
  void notify();
  void waitForNotification();

  // Getters & Setters
  int getCurrentRep() {return _rep_num;}
  void setCurrentRep(int rep) {_rep_num = rep;}

  static int getMaxReps() {return _k_max_reps;}

  float getRetention() {return _retention;}
  void setRetention(float retention) {_retention = retention;}
  
  float getStability() {return _stability;}
  void setStability(float stability) {_stability = stability;}

  float getMinRetention(int rep_num);

  int getTotalElapsedTime() {return _total_time_elapsed;}

  // Setters
  void setKnowledgePtr(std::shared_ptr<BaseKnowledge> know_ptr) { _knowledge = know_ptr; }
  
  // Update functions
  void updateRetention();
  void updateStability();

  //Destructor
  ~ForgettingCurve();

  private:
  MessageQueue<int> _message_q;
  std::mutex _mtx;
  std::vector<std::thread> _threads;
  std::shared_ptr<BaseKnowledge> _knowledge;

  /// Forgetting curve parameters

  // time elapsed in days beteween repititions
  int _interval_time_elapsed = 0;
  
  // time elapsed since initial learning
  int _total_time_elapsed = 0;
  
  // Memory stability refers to how stable a particular memory is. Higher stability means slower rate of forgetting and
  // will stay in memory for longer duration. Stability increases after every repitition.
  float _stability = 0.87;
  
  // Fraction of a particular memory that can be successfully recalled. (Value between 0 - 1). Retention goes down with time from 1
  // after every repitition.
  float _retention;
  
  // Current repitition number
  int _rep_num = 0;

  // Max number of repititions
  static constexpr int _k_max_reps = 4;

}; 

#endif