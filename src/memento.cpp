#include "memento.h"

#include <algorithm>
#include <memory>
#include <thread>
#include <chrono>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

const std::string Memento::_data_dir_path = "../data";
namespace bfs = boost::filesystem;

Memento::Memento(MementoDialogWindow* dialog_window) : _dialog_window(dialog_window) { }

std::unordered_map<std::string, std::string> Memento::_responses { 
  
  {"start", "Hi, I am Memento! I will help you remember things you learn. As of now, I "
            "can help you remember the following.\n"
            "1. Files (For e.g., notes stored in a text file)\n"
            "2. Web links (For e.g. a youtube video on French Revolution)\n"
            "3. Generic text (Whatever you type in the text box. For e.g., Hola - Hello)\n"
            "Enter a number (1-3)!"},
  
  {"start_over", "Let's learn more. Enter a number (1-3)!\n"
            "1. Files (For e.g., notes stored in a text file)\n"
            "2. Web links (For e.g. a youtube video on French Revolution)\n"
            "3. Generic text (Whatever you type in the text box. For e.g., Hola - Hello)"
            },

  {"1", "Let's put those notes into use! Enter the path to your"
        " file."},

  {"files", "Great! I will remind you when it's time to revise those notes."},
  
  {"2", "Let's make sure you never forget the details of the cool story in your next happy hour!"
        " Enter the URL."},

  {"links", "Perfect! I will remind you when it's time to revisit that link."},
  
  {"3", "Let's learn something exciting! Type in whatever you want to learn."},

  {"text", "Wonderful! I will remind you when it's time to take a look at that Post-it notes again. And don't worry, these post-it notes won't fall off from the fridge door."},
};

void Memento::getResponse(std::string user_txt) {

  std::string status = user_txt;

  if(_last_response == "1") {
    status = "files"; 
  }

  else if (_last_response == "2") {
    status = "links";
  }

  else if (_last_response == "3") {
    status = "text";
  }  
  
  _last_response = user_txt;
  
  _dialog_window->PrintMementoResponse(_responses[status]);
  
  if (status == "files" || status == "links" || status == "text"){

    _dialog_window->PrintMementoResponse(_responses["start_over"]);
    
    auto t = std::chrono::system_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::seconds>(t.time_since_epoch()).count();

    if (status == "files"){

      auto know_f = std::make_shared<FileKnowledge>(user_txt, std::to_string(dur));
      know_f->getForgettingCurvePtr()->setKnowledgePtr(know_f);
      know_f->writeData();
      _threads.emplace_back(std::thread(&Memento::start, this, know_f));
    
    }

    else if (status == "links"){

      auto know_l = std::make_shared<LinkKnowledge>(user_txt, std::to_string(dur));
      know_l->getForgettingCurvePtr()->setKnowledgePtr(know_l);
      know_l->writeData();
      _threads.emplace_back(std::thread(&Memento::start, this, know_l));
    
    }

    else if (status == "text"){

      auto know_t = std::make_shared<TextKnowledge>(user_txt, std::to_string(dur));
      know_t->getForgettingCurvePtr()->setKnowledgePtr(know_t);
      know_t->writeData();
      
      _threads.emplace_back(std::thread(&Memento::start, this, know_t));
    
    }

  }

}
void Memento::start(std::shared_ptr<BaseKnowledge> know) {
  
  auto fc_ptr = know->getForgettingCurvePtr();
  fc_ptr->start();

  while(fc_ptr->getCurrentRep() < ForgettingCurve::getMaxReps()) {
  
    fc_ptr->waitForNotification();
    auto response = know->getNotification();
    _dialog_window->PrintMementoResponse(response, true);
  
  }
  
}

void Memento::restart() {

  if (!bfs::exists(_data_dir_path)) {
    bfs::create_directory(_data_dir_path);
  }
  else if (!bfs::is_empty(_data_dir_path)) {
    //std::cout << "Traversing" << std::endl;
    for(auto& entry : boost::make_iterator_range(bfs::directory_iterator(_data_dir_path), {})) {
      auto file_name = entry.path().filename().string();
      if (bfs::is_regular_file(entry) && file_name != ".DS_Store") {    
        //std::cout << "Loading: " << file_name << std::endl;
        auto know_t = std::make_shared<TextKnowledge>("", file_name);
        know_t->getForgettingCurvePtr()->setKnowledgePtr(know_t);
        know_t->loadData();
        _threads.emplace_back(std::thread(&Memento::start, this, know_t));
      }
    }
    //std::cout << "Finish Traversing" << std::endl;
  }
  else{
    //std::cout << "Doing nothing" << std::endl;
  }

}

Memento::~Memento(){
  
  std::for_each(_threads.begin(), _threads.end(), [](std::thread& t){ t.join();});

}