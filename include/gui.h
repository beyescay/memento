#ifndef GUI_H
#define GUI_H

#include "notification.h"
#include "message_queue.h"
#include "memento.h"
#include "wx/wx.h"
#include <unordered_map>

class Memento;

class MementoApp : public wxApp{
  public:
  virtual bool OnInit();
  //static void init();
  //static void pushNotification(Notification* const notification);
  //static void displayNotification();
  
  private:
  //static MessageQueue<Notification> _message_q;
  //std::mutex _mtx;
};

class MementoDialogItem : public wxPanel {
  
  public:
  MementoDialogItem(wxPanel* parent, wxString text, bool from_user);

};
class MementoDialogWindow : public wxScrolledWindow {
  
  public:
  MementoDialogWindow(wxWindow* parent, wxWindowID id);

  void AddDialogItem(wxString text, bool from_user=true);
  void PrintMementoResponse(std::string response);
  Memento* GetMementoHandle();

  void paintEvent(wxPaintEvent& evt);
  void paintNow();
  void render(wxDC& dc);

  DECLARE_EVENT_TABLE()

  private:
  wxSizer* _dialog_window_sizer;
  std::unique_ptr<Memento> _memento;
};

class MementoBgImagePanel : public wxPanel {
  
  public:
  MementoBgImagePanel(wxFrame* parent);
  
  void paintEvent(wxPaintEvent& evt);
  void paintNow();
  void render(wxDC& dc);

  DECLARE_EVENT_TABLE()
};

class MementoFrame : public wxFrame {

  public:
  MementoFrame(const wxString& title);

  private:
  wxTextCtrl* _user_txt_ctrl;
  MementoDialogWindow* _dialog_window;
  void OnEnter(wxCommandEvent &WXUNUSEDevent);

};



#endif