
#include "forgetting_curve.h"
#include "notification.h"
#include <condition_variable>
#include <string>
#include <vector>

enum KnowledgeType {
  File,
  Link,
  Text
};

template<typename T>
class Knowledge {
  
  public:
  
  void setSubject(std::string subject) {_subject = subject;}
  std::string getSubject() const {return _subject;}
  
  std::string getNotification();
  
  private:
  std::string _subject;
  std::shared_ptr<ForgettingCurve> _fcPtr;
};

