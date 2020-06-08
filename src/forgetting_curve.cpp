#include "forgetting_curve.h"
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace std::chrono;

ForgettingCurve::ForgettingCurve() { }

void ForgettingCurve::start(){

  _threads.emplace_back(std::thread(&ForgettingCurve::notify, this));

}

void ForgettingCurve::notify(){

  constexpr int hours_in_day = 24;

  auto start_time = system_clock::now() - hours(_knowledge->getStartTimeHours());  
  auto init_time = start_time - hours(_knowledge->getInitTimeHours());
  auto last_write_time = system_clock::now();
  
  while(_rep_num < _k_max_reps){

    auto curr_time = system_clock::now();
    _interval_time_elapsed = duration_cast<hours>(curr_time-start_time).count()/hours_in_day;
    _total_time_elapsed = duration_cast<hours>(curr_time-init_time).count()/hours_in_day;
    auto write_time_elapsed = duration_cast<hours>(curr_time-last_write_time).count()/hours_in_day;
    
    updateRetention();

    if (write_time_elapsed > 1) {
      _knowledge->setInitTimeHours(_total_time_elapsed);
      _knowledge->setStartTimeHours(_interval_time_elapsed);
      _knowledge->writeData();
      last_write_time = system_clock::now();
    }
  
    if (_retention < getMinRetention(_rep_num)){
      _rep_num++;
      _message_q.send(_rep_num);
      updateStability();
      start_time = curr_time;
    }
    
    std::this_thread::sleep_for(hours(1));
       
  }
  
  _knowledge->deleteData();

  return;
}

void ForgettingCurve::waitForNotification(){
  
  while(true){
  
    if (_message_q.receive()){ return; }
  
  }

}

float ForgettingCurve::getMinRetention(int rep_num){

  // First revision after 1 day, subsequent revisions whenever the retention
  // goes below 0.8
  return rep_num == 0 ? 0.32 : 0.8;

}

void ForgettingCurve::updateRetention(){

  // Retention is an exponential curve that depends on time since last repitition and memory stability.
  _retention = exp(-_interval_time_elapsed/_stability);

}

void ForgettingCurve::updateStability() {
  
  // Stability increases after every repitition. It depends on current retention, current stability, 
  // and constants Max stability increase ratio (26.31) and Gain constant (2.96)
  _stability *= (26.31 * exp(-2.96* _retention));

}

ForgettingCurve::~ForgettingCurve(){
  
  std::for_each(_threads.begin(), _threads.end(), [](std::thread& t){ 
    t.join();
  });

}