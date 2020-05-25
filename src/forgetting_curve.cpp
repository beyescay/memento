#include "forgetting_curve.h"

using namespace std::chrono;

//Constructor
ForgettingCurve::ForgettingCurve() { 

  _repitition_n = RepititionNum::Start;

}

RepititionNum ForgettingCurve::getCurrentRepitition() {

  return _repitition_n;

}

void ForgettingCurve::notify(){
  
  auto start_time = system_clock::now();

  while(_repitition_n != RepititionNum::End){
    
    auto curr_time = system_clock::now();
    auto time_elaspsed = duration_cast<seconds>(curr_time-start_time).count();

    if (time_elaspsed > (_repitition_n+1)*_k_rep_interval_s){
      _repitition_n = static_cast<RepititionNum>(_repitition_n+1);
      _message_q.send(_repitition_n);
    }

    std::this_thread::sleep_for(milliseconds(1));
    
  }
  return;
}

void ForgettingCurve::waitForNotification(){
  while(true){
    if (_message_q.receive() != RepititionNum::End){
      return;
    }
  }
}