#include "knowledge.h"

#include <sstream>

BaseKnowledge::BaseKnowledge(std::string info) : _info(info) {

  _fcPtr = std::make_shared<ForgettingCurve>();
  
}

FileKnowledge::FileKnowledge(std::string info) : BaseKnowledge(info) { }

std::string FileKnowledge::getNotification() {

  auto ret = _fcPtr->getRetention()*100;
  std::stringstream sstrm;
  sstrm.precision(4);
  sstrm << ret;

  int rep_num = _fcPtr->getCurrentRep();
  int elapsed_time = _fcPtr->getTotalElapsedTime();
  
  std::string res = "It's time to revise these notes: " + _info + "\n" +
                    "Num days since initial learning: " + std::to_string(elapsed_time) + "\n" +
                    "Currently, you remember: " + sstrm.str() + "% of your notes. Revise it now to take it to 100%\n" +
                    "This will be your revision number: " + std::to_string(rep_num);

  return res;

}

LinkKnowledge::LinkKnowledge(std::string info) : BaseKnowledge(info) { }

std::string LinkKnowledge::getNotification() {

  auto ret = _fcPtr->getRetention()*100;
  std::stringstream sstrm;
  sstrm.precision(4);
  sstrm << ret;

  int rep_num = _fcPtr->getCurrentRep();
  int elapsed_time = _fcPtr->getTotalElapsedTime();
  
  std::string res = "It's time to revisit this link: " + _info + "\n" +
                    "Num days since initial learning: " + std::to_string(elapsed_time) + "\n" +
                    "Currently, you remember: " + sstrm.str() + "% of this story. Revisit it now to take it to 100%\n" +
                    "This will be your revision number: " + std::to_string(rep_num);

  return res;

}

TextKnowledge::TextKnowledge(std::string info) : BaseKnowledge(info) { }

std::string TextKnowledge::getNotification() {

  auto ret = _fcPtr->getRetention()*100;
  std::stringstream sstrm;
  sstrm.precision(4);
  sstrm << ret;

  int rep_num = _fcPtr->getCurrentRep();
  int elapsed_time = _fcPtr->getTotalElapsedTime();
  
  std::string res = "It's time to take a look at these notes: " + _info + "\n" +
                    "Num days since initial learning: " + std::to_string(elapsed_time) + "\n" +
                    "Currently, you remember: " + sstrm.str() + "% of this text. Review it now to take it to 100%\n" +
                    "This will be your revision number: " + std::to_string(rep_num);
                    
  return res;

}