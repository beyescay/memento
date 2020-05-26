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
  
  auto start_time = system_clock::now();
  auto init_time = start_time;

  while(_rep_num < _k_max_reps){

    auto curr_time = system_clock::now();
    _interval_time_elapsed = duration_cast<hours>(curr_time-start_time).count();
    _total_time_elapsed = duration_cast<hours>(curr_time-init_time).count();
    
    updateRetention();
  
    if (_retention < getMinRetention(_rep_num)){
      
      _rep_num++;
      _message_q.send(_rep_num);
      updateStability();
      start_time = curr_time;

    }
    
    std::this_thread::sleep_for(milliseconds(1));
    
  }

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

  _retention = exp(-_interval_time_elapsed/_stability);

}

void ForgettingCurve::updateStability() {
  
  _stability *= (26.31 * exp(-2.96* _retention));

}

ForgettingCurve::~ForgettingCurve(){
  
  std::for_each(_threads.begin(), _threads.end(), [](std::thread& t){ 
    t.join();
  });

}