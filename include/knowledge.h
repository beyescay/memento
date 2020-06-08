
#ifndef KNOWLEDGE_H
#define KNOWLEDGE_H

#include <string>
#include <vector>
#include <memory>
#include <chrono>

#include "forgetting_curve.h"

class ForgettingCurve;

class BaseKnowledge {
  
  public:
  BaseKnowledge(std::string info, std::string file_id);
  
  ForgettingCurve* getForgettingCurvePtr(){ return _fcPtr.get(); }

  void setSubject(std::string subject) {_subject = subject;}
  std::string getSubject() const {return _subject;}

  void setStartTimeHours(int hours) {_start_time_hours = hours;}
  int getStartTimeHours() const {return _start_time_hours;}
  
  void setInitTimeHours(int hours) {_init_time_hours = hours;}
  int getInitTimeHours() const {return _init_time_hours;}

  virtual std::string getNotification() = 0;
  
  void writeData();
  void loadData();
  void deleteData();

  protected:
  std::shared_ptr<ForgettingCurve> _fcPtr;
  std::string _info;
  std::string _subject;
  int _start_time_hours {0};
  int _init_time_hours {0};
  
  static const std::string _data_dir_path;
  std::string _file_id;

};

class FileKnowledge : public BaseKnowledge {

  public:
  FileKnowledge(std::string info, std::string file_id);
  std::string getNotification() override;

};

class LinkKnowledge : public BaseKnowledge {

  public:
  LinkKnowledge(std::string info, std::string file_id);
  std::string getNotification() override;

};

class TextKnowledge : public BaseKnowledge {

  public:
  TextKnowledge(std::string info, std::string file_id);
  std::string getNotification() override;

};
#endif
