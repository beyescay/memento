#ifndef GUI_H
#define GUI_H

#include "message_queue.h"
#include "memento.h"
#include "wx/wx.h"

#include <mutex>
#include <unordered_map>


// Forward declaration
class Memento;

class MementoApp : public wxApp{

  public:
  virtual bool OnInit();

};

class MementoDialogItem : public wxPanel {

  public:
  MementoDialogItem(wxPanel* parent, wxString text, bool from_user, bool is_notification=false);

};
class MementoDialogWindow : public wxScrolledWindow {

  public:
  MementoDialogWindow(wxWindow* parent, wxWindowID id);

  void AddDialogItem(wxString text, bool from_user=true, bool is_notification=false);
  void PrintMementoResponse(std::string response, bool is_notification=false);
  Memento* GetMementoHandle();

  void paintEvent(wxPaintEvent& evt);
  void paintNow();
  void render(wxDC& dc);

  DECLARE_EVENT_TABLE()

  private:
  std::mutex _mtx;
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
  std::mutex _mtx;

};

#endif
