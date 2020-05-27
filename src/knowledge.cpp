#include "knowledge.h"

#include <sstream>
#include <chrono>
#include <ctime>

BaseKnowledge::BaseKnowledge(std::string info) : _info(info) {

  _fcPtr = std::make_shared<ForgettingCurve>();
  
}

FileKnowledge::FileKnowledge(std::string info) : BaseKnowledge(info) { }

struct CurveInfoFormatter {
  
  std::string info;
  std::string ret;
  std::string rep_num;
  std::string elapsed_days;
  std::string curr_time;
  ForgettingCurve* fc_ptr;

  CurveInfoFormatter(ForgettingCurve* fcp) : fc_ptr(fcp) { }

  CurveInfoFormatter* format() {

    auto r = fc_ptr->getRetention()*100;
    std::stringstream sstrm;
    sstrm.precision(4);
    sstrm << r;
    ret = sstrm.str();

    rep_num = std::to_string(fc_ptr->getCurrentRep());
    elapsed_days = std::to_string(fc_ptr->getTotalElapsedTime());

    auto time_now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    curr_time = ctime(&time_now);

    return this;
  }

};
std::string FileKnowledge::getNotification() {

  auto curve = CurveInfoFormatter(_fcPtr.get()).format();
  
  std::string res = curve->curr_time + "\n" +
                    "Time to revise these notes: " + _info + "\n" +
                    "Num days since initial learning: " + curve->elapsed_days + "\n" +
                    "Currently you remember: " + curve->ret + "% of your notes. Revise it now to take it to 100%\n" +
                    "This will be your revision number: " + curve->rep_num;

  return res;

}

LinkKnowledge::LinkKnowledge(std::string info) : BaseKnowledge(info) { }

std::string LinkKnowledge::getNotification() {

  auto curve = CurveInfoFormatter(_fcPtr.get()).format();
  
  std::string res = curve->curr_time + "\n" +
                    "Time to revisit this link: " + _info + "\n" +
                    "Num days since initial learning: " + curve->elapsed_days + "\n" +
                    "Currently you remember: " + curve->ret + "% of your link. Revisit it now to take it to 100%\n" +
                    "This will be your revision number: " + curve->rep_num;

  return res;

}

TextKnowledge::TextKnowledge(std::string info) : BaseKnowledge(info) { }

std::string TextKnowledge::getNotification() {

  auto curve = CurveInfoFormatter(_fcPtr.get()).format();
  
  std::string res = curve->curr_time + "\n" +
                    "Time to revise this text: " + _info + "\n" +
                    "Num days since initial learning: " + curve->elapsed_days + "\n" +
                    "Currently, you remember: " + curve->ret + "% of this text. Revise it now to take it to 100%\n" +
                    "This will be your revision number: " + curve->rep_num;
                    
  return res;

}