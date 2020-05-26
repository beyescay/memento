
#ifndef KNOWLEDGE_H
#define KNOWLEDGE_H

#include <string>
#include <vector>
#include <memory>

#include "forgetting_curve.h"


class BaseKnowledge {
  
  public:
  BaseKnowledge(std::string info);
  
  ForgettingCurve* getForgettingCurvePtr(){ return _fcPtr.get(); }

  void setSubject(std::string subject) {_subject = subject;}
  std::string getSubject() const {return _subject;}
  
  virtual std::string getNotification() = 0;
  
  protected:
  std::shared_ptr<ForgettingCurve> _fcPtr;
  std::string _info;
  std::string _subject;
  
};

class FileKnowledge : public BaseKnowledge {

  public:
  FileKnowledge(std::string info);
  std::string getNotification() override;

};

class LinkKnowledge : public BaseKnowledge {

  public:
  LinkKnowledge(std::string info);
  std::string getNotification() override;

};

class TextKnowledge : public BaseKnowledge {

  public:
  TextKnowledge(std::string info);
  std::string getNotification() override;

};
#endif
