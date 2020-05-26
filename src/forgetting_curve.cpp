#include "forgetting_curve.h"
#include <cmath>
#include <iostream>
#include <algorithm>


using namespace std::chrono;

//Constructor
ForgettingCurve::ForgettingCurve() { }

void ForgettingCurve::start(){

  _threads.emplace_back(std::thread(&ForgettingCurve::notify, this));

}

void ForgettingCurve::notify(){
  
  //std::cout << "In notify:  " <<std::to_string(int(_repitition_n)) << std::endl;
  auto start_time = system_clock::now();
  auto init_time = start_time;
  //std::unique_lock<std::mutex> u_lck(_mtx);
  while(_rep_num < _k_max_reps){
    //std::cout << "In while " << std::endl;
    //u_lck.unlock();
    auto curr_time = system_clock::now();
    _interval_time_elapsed = duration_cast<minutes>(curr_time-start_time).count();
    _total_time_elapsed = duration_cast<minutes>(curr_time-init_time).count();
    
    updateRetention();
    //std::cout << "Time elapsed: " << _time_elapsed << " Ret: " << _retention << std::endl;
    //u_lck.lock();
    if (_retention < getMinRetention(_rep_num)){
      //std::cout << "In if " << std::endl;
      _rep_num++;
      //std::cout << "sending: " << std::to_string(int (_repitition_n))<< std::endl;
      std::cout << "Pushing notific at: " << _total_time_elapsed << " and " << _retention << std::endl;
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
    if (_message_q.receive()){
      return;
    }
  }
}

int ForgettingCurve::getCurrentRep(){

  return _rep_num;

}

int ForgettingCurve::getMaxReps() {

  return _k_max_reps;

}

float ForgettingCurve::getRetention(){

  return _retention;

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

int ForgettingCurve::getTotalElapsedTime() {
  
  return _total_time_elapsed;

}

ForgettingCurve::~ForgettingCurve(){
  
  std::for_each(_threads.begin(), _threads.end(), [](std::thread& t){ 
    t.join();
  });

}