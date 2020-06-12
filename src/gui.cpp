#include "gui.h"
#include <wx/filename.h>
#include <wx/colour.h>
#include <wx/image.h>

constexpr int height = 600;
constexpr int width = 400;
constexpr int wrap = width/1.25;
constexpr int box_width = wrap+10;

wxIMPLEMENT_APP(MementoApp);

std::string images_folder = "../images/";
std::string logo = "logo.png";

bool MementoApp::OnInit(){
  MementoFrame* frame =  new MementoFrame(wxT("Memento"));
  frame->Show(true);
  return true;
}

// Memento Frame - START
MementoFrame::MementoFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(width, height)) {

  auto memento_bg_img = new MementoBgImagePanel(this);

  _dialog_window = new MementoDialogWindow(memento_bg_img, wxID_ANY);

  int txt_ctrl_id = 1;
  
  _user_txt_ctrl = new wxTextCtrl(memento_bg_img, txt_ctrl_id, "", wxDefaultPosition, wxSize(width, 5), wxTE_PROCESS_ENTER | wxTE_CHARWRAP | wxTE_AUTO_URL, wxDefaultValidator, wxTextCtrlNameStr);
  
  Connect(txt_ctrl_id, wxEVT_TEXT_ENTER, wxCommandEventHandler(MementoFrame::OnEnter));

  wxBoxSizer* vert_box_sizer = new wxBoxSizer(wxVERTICAL);
  vert_box_sizer->AddSpacer(0);
  std::unique_lock<std::mutex> u_lock(_mtx);
  vert_box_sizer->Add(_dialog_window, 15, wxEXPAND | wxALL, 0);
  vert_box_sizer->Add(_user_txt_ctrl, 1, wxEXPAND | wxALL, 5);
  memento_bg_img->SetSizer(vert_box_sizer);
  this->Center();
}

void MementoFrame::OnEnter(wxCommandEvent &WXUNUSED(event)) {

  std::unique_lock<std::mutex> u_lock(_mtx);

  wxString user_txt = _user_txt_ctrl->GetLineText(0);
  
  _dialog_window->AddDialogItem(user_txt, true);

  _user_txt_ctrl->Clear();

  _dialog_window->GetMementoHandle()->getResponse(std::string(user_txt.mb_str()));

}
// Memento Frame - END

// Memento Image Window - Start
BEGIN_EVENT_TABLE(MementoBgImagePanel, wxPanel)
EVT_PAINT(MementoBgImagePanel::paintEvent)
END_EVENT_TABLE()

MementoBgImagePanel::MementoBgImagePanel(wxFrame* parent) : wxPanel(parent) {

}

void MementoBgImagePanel::paintEvent(wxPaintEvent& evt) {

  wxPaintDC dc(this);
  render(dc);

}

void MementoBgImagePanel::paintNow() {

  wxClientDC dc(this);
  render(dc);
}

void MementoBgImagePanel::render(wxDC& dc) {

  wxImage image;
  image.LoadFile(images_folder + logo);
  wxSize sz = this->GetSize();
  wxImage img_resized = image.Rescale(sz.GetWidth(), sz.GetHeight(), wxIMAGE_QUALITY_HIGH);
  auto bitmap = wxBitmap(img_resized);
  dc.DrawBitmap(bitmap, 0, 0, false);

}

// Memento Dialog Window - START
BEGIN_EVENT_TABLE(MementoDialogWindow, wxPanel)
EVT_PAINT(MementoDialogWindow::paintEvent)
END_EVENT_TABLE()

MementoDialogWindow::MementoDialogWindow(wxWindow* parent, wxWindowID id) : wxScrolledWindow(parent, id) {

  _dialog_window_sizer = new wxBoxSizer(wxVERTICAL);
  _dialog_window_sizer->AddSpacer(500);
  this->SetSizer(_dialog_window_sizer);
  wxInitAllImageHandlers();

  _memento = std::make_unique<Memento>(this);
  
  _memento->getResponse("start");
  
  _memento->restart();

}

void MementoDialogWindow::AddDialogItem(wxString text, bool from_user, bool is_notification) {
  
  std::unique_lock<std::mutex> u_lock(_mtx);
  
  MementoDialogItem* item = new MementoDialogItem(this, text, from_user, is_notification);
  _dialog_window_sizer->Add(item, 0, wxALL | (from_user == true ? wxALIGN_RIGHT : wxALIGN_LEFT), 8);
  _dialog_window_sizer->Layout();

  this->FitInside();
  this->SetScrollRate(5, 5);
  this->Layout();

  int dx, dy;
  this->GetScrollPixelsPerUnit(&dx, &dy);
  int sy = dy * this->GetScrollLines(wxVERTICAL);
  this->DoScroll(0, sy);

}

void MementoDialogWindow::PrintMementoResponse(std::string response, bool is_notification){

  wxString txt(response.c_str());
  AddDialogItem(txt, false, is_notification);

}

Memento* MementoDialogWindow::GetMementoHandle(){
  
  std::unique_lock<std::mutex> u_lock(_mtx);
  return _memento.get();

}


void MementoDialogWindow::paintEvent(wxPaintEvent& evt){

  wxPaintDC dc(this);
  render(dc);

}

void MementoDialogWindow::paintNow(){

  wxClientDC dc(this);
  render(dc);

}

void MementoDialogWindow::render(wxDC& dc) {

  wxImage image;
  image.LoadFile(images_folder + logo);
  wxSize sz = this->GetSize();
  wxImage img_resized = image.Rescale(sz.GetWidth(), sz.GetHeight(), wxIMAGE_QUALITY_HIGH);
  auto bitmap = wxBitmap(img_resized);
  dc.DrawBitmap(bitmap, 0, 0, false);

}

// Memento Dialog Window - END

// Memento Dialog Item - START
MementoDialogItem::MementoDialogItem(wxPanel* parent, wxString text, bool from_user, bool is_notification)
 : wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_NONE) {

   //wxImage img;
   //img.LoadFile(from_user ? images_folder + user : images_folder + bot);
   //wxSize sz = this->GetSize();
   //wxImage img_rescaled = img.Rescale(sz.GetWidth(), sz.GetHeight(), wxIMAGE_QUALITY_NEAREST);
   //auto sbm = new wxStaticBitmap(this, wxID_ANY, wxBitmap(img_rescaled, wxBITMAP_TYPE_PNG), wxPoint(100, 30), wxSize(100, 100));

   auto txt = new wxStaticText(this, wxID_ANY, text, wxPoint(-1, -1), wxSize(box_width, -1), wxALIGN_LEFT | wxBORDER_NONE);
   txt->SetForegroundColour(is_notification == true ? wxColor(*wxWHITE) : wxColor(*wxBLACK));

   wxBoxSizer* hor_box_sizer = new wxBoxSizer(wxHORIZONTAL);

   if (from_user) {
    hor_box_sizer->Add(txt, 8, wxEXPAND | wxALL, 2);
    //hor_box_sizer->Add(sbm, 1, wxEXPAND | wxALL, 1);
   }
   else{
    //hor_box_sizer->Add(sbm, 1, wxEXPAND | wxALL, 1);
    hor_box_sizer->Add(txt, 8, wxEXPAND | wxALL, 2);
   }

   this->SetSizer(hor_box_sizer);

   txt->Wrap(wrap);

   if (is_notification) {
      this->SetBackgroundColour(wxColor(0, 0, 0));
   }
   else{
      this->SetBackgroundColour((from_user == true ? wxColor(255, 1, 217) : wxT("YELLOW")));
   }
  }
 // Memento Dialog Item - START
