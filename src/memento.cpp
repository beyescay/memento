#include "memento.h"

#include <algorithm>
#include <memory>
#include <thread>

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
    
    if (status == "files"){

      auto know_f = std::make_shared<FileKnowledge>(user_txt);
      _threads.emplace_back(std::thread(&Memento::start, this, know_f));
    
    }

    else if (status == "links"){

      auto know_l = std::make_shared<LinkKnowledge>(user_txt);
      _threads.emplace_back(std::thread(&Memento::start, this, know_l));
    
    }

    else if (status == "text"){

      auto know_t = std::make_shared<TextKnowledge>(user_txt);
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

Memento::~Memento(){
  
  std::for_each(_threads.begin(), _threads.end(), [](std::thread& t){ t.join();});

}