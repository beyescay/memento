#include "knowledge.h"

#include <sstream>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>

#include <boost/filesystem.hpp>

const std::string BaseKnowledge::_data_dir_path = "../data/";
namespace bfs = boost::filesystem;

BaseKnowledge::BaseKnowledge(std::string info, std::string file_id) : _info(info), _file_id(file_id) {

  _fcPtr = std::make_shared<ForgettingCurve>();

}

void BaseKnowledge::writeData() {
  
  std::string file_path = _data_dir_path + _file_id;

  std::ofstream ofs(file_path, std::ofstream::trunc);

  ofs << "Info: " << _info << "\n";
  ofs << "Hours_since_start: " << _init_time_hours << "\n";
  ofs << "Hours_since_last_interval: " << _start_time_hours << "\n";;
  ofs << "Rep_num: " << _fcPtr->getCurrentRep() << "\n";
  ofs << "Retention: " << _fcPtr->getRetention() << "\n";
  ofs << "Stability: " << _fcPtr->getStability() << "\n";

  ofs.close();

}

void BaseKnowledge::loadData() {
  
  std::string file_path = _data_dir_path + _file_id;
  
  std::ifstream ifs(file_path);
  std::string key;
  
  ifs >> key >> _info;
  ifs >> key >> _init_time_hours;
  ifs >> key >> _start_time_hours;

  int rep_num;
  float retention;
  float stability;

  ifs >> key >> rep_num;
  ifs >> key >> retention;
  ifs >> key >> stability;

  _fcPtr->setCurrentRep(rep_num);
  _fcPtr->setRetention(retention);
  _fcPtr->setStability(stability);

  ifs.close();

}

void BaseKnowledge::deleteData() {
  
  std::string file_path = _data_dir_path + _file_id;
  
  std::remove(file_path.c_str());

}

FileKnowledge::FileKnowledge(std::string info, std::string file_id) : BaseKnowledge(info, file_id) { }

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

LinkKnowledge::LinkKnowledge(std::string info, std::string file_id) : BaseKnowledge(info, file_id) { }

std::string LinkKnowledge::getNotification() {

  auto curve = CurveInfoFormatter(_fcPtr.get()).format();
  
  std::string res = curve->curr_time + "\n" +
                    "Time to revisit this link: " + _info + "\n" +
                    "Num days since initial learning: " + curve->elapsed_days + "\n" +
                    "Currently you remember: " + curve->ret + "% of your link. Revisit it now to take it to 100%\n" +
                    "This will be your revision number: " + curve->rep_num;

  return res;

}

TextKnowledge::TextKnowledge(std::string info, std::string file_id) : BaseKnowledge(info, file_id) { }

std::string TextKnowledge::getNotification() {

  auto curve = CurveInfoFormatter(_fcPtr.get()).format();
  
  std::string res = curve->curr_time + "\n" +
                    "Time to revise this text: " + _info + "\n" +
                    "Num days since initial learning: " + curve->elapsed_days + "\n" +
                    "Currently, you remember: " + curve->ret + "% of this text. Revise it now to take it to 100%\n" +
                    "This will be your revision number: " + curve->rep_num;
                    
  return res;

}