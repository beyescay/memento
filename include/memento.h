#ifndef MEMENTO_H
#define MEMENTO_H

#include "gui.h"
#include "knowledge.h"

#include <thread>
#include <memory>
#include <vector>
#include <unordered_map>
#include <mutex>


// Forward declaration
class MementoDialogWindow;

class Memento {

  public:

  //Constructor
  Memento(MementoDialogWindow* dialog_window);

  // Getter
  void getResponse(std::string user_txt);

  // Function to restart all threads in case of crash
  void restart();

  // Destructor
  ~Memento();

  private:

  // Control Function
  void start(std::shared_ptr<BaseKnowledge> know);

  MementoDialogWindow* _dialog_window;
  std::vector<std::thread> _threads;
  std::string _last_response;
  static std::unordered_map<std::string, std::string> _responses;
  static const std::string _data_dir_path;
  std::mutex _mtx;

};

#endif
