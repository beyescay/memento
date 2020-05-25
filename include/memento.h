#ifndef MEMENTO_H
#define MEMENTO_H

#include "gui.h"
#include "knowledge.h"

#include <condition_variable>
#include <thread>
#include <memory>
#include <vector>
#include <unordered_map>

class MementoDialogWindow;

class Memento {
  
  public:
  Memento(MementoDialogWindow* dialog_window);
  
  ~Memento();

  void getResponse(std::string user_txt);

  private:
  void start(Knowledge* know);
  MementoDialogWindow* _dialog_window;
  std::vector<std::thread> _threads;
  static std::unordered_map<std::string, std::string> _responses;
  std::string _last_response;
};

#endif