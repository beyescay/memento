#include "memento.h"

Memento::Memento(MementoDialogWindow* dialog_window) : _dialog_window(dialog_window) {

}

std::unordered_map<std::string, std::string> Memento::_responses{ 
  
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

void Memento::getResponse(std::string user_txt){

  if(_last_response == "1") {
    user_txt = "files";
  }
  else if (_last_response == "2"){
    user_txt = "links";
  }
  else if (_last_response == "3"){
    user_txt = "text";
  }  
  
  _last_response = user_txt;
  
  _dialog_window->PrintMementoResponse(_responses[user_txt]);
  
  if (user_txt == "files" || user_txt == "links" || user_txt == "text"){
    _dialog_window->PrintMementoResponse(_responses["start_over"]);

  }

}
void Memento::start(Knowledge* know){

}

Memento::~Memento(){
  //_t.join();
}