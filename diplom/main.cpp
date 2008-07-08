/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     main window
// Author:      Ralph Schurade
// Modified by:
// Created:     03/27/08
// RCS-ID:
// Copyright:
// Licence:
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <GL/glew.h>
#include "theScene.h"


#include "wx/mdi.h"
#include "wx/laywin.h"

#include "icons/fileopen.xpm"
#include "icons/view1.xpm"
#include "icons/view2.xpm"
#include "icons/view3.xpm"
#include "icons/mini_cat.xpm"
#include "icons/new.xpm"
#include "icons/quit.xpm"
#include "icons/toggleselbox.xpm"
#include "icons/toggleSurface.xpm"
#include "icons/gball.xpm"

#include "main.h"
#include "mainFrame.h"

MainFrame *frame = NULL;

IMPLEMENT_APP(MyApp)

// Initialise this in OnInit, not statically
bool MyApp::OnInit(void)
{
  // Create the main frame window

  frame = new MainFrame(NULL, wxID_ANY, _T("Viewer"), wxPoint(0, 0), wxSize(1200, 820),
                      wxDEFAULT_FRAME_STYLE |
                      wxHSCROLL | wxVSCROLL);

  // Give it an icon (this is ignored in MDI mode: uses resources)
#ifdef __WXMSW__
  frame->SetIcon(wxIcon(_T("sashtest_icn")));
#endif

  frame->SetMinSize(wxSize(800,600));
  // Make a menubar
  wxMenu *file_menu = new wxMenu;
  file_menu->Append(VIEWER_LOAD, _T("&Load"));
    file_menu->Append(VIEWER_QUIT, _T("&Exit"));

  wxMenu *help_menu = new wxMenu;
  help_menu->Append(VIEWER_ABOUT, _T("&About"));

  wxMenuBar *menu_bar = new wxMenuBar;
  menu_bar->Append(file_menu, _T("&File"));
  menu_bar->Append(help_menu, _T("&Help"));

  // Associate the menu bar with the frame
  frame->SetMenuBar(menu_bar);

  wxToolBar* toolBar = new wxToolBar( frame, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxNO_BORDER);
  wxBitmap bmpOpen (fileopen_xpm);
  wxBitmap bmpView1 (view1_xpm);
  wxBitmap bmpView2 (view2_xpm);
  wxBitmap bmpView3 (view3_xpm);
  wxBitmap bmpMiniCat (mini_cat_xpm);
  wxBitmap bmpNew (new_xpm);
  wxBitmap bmpQuit (quit_xpm);
  wxBitmap bmpGBALL (gball_xpm);
  wxBitmap bmpHideSelbox (toggleselbox_xpm);
  wxBitmap bmpNewSurface (toggle_surface_xpm);
  toolBar->AddTool(VIEWER_NEW, bmpNew, wxT("New"));
  toolBar->AddTool(VIEWER_LOAD, bmpOpen, wxT("Open"));
  toolBar->AddTool(VIEWER_QUIT, bmpQuit, wxT("Quit"));
  toolBar->AddSeparator();
  toolBar->AddTool(VIEWER_TOGGLE_AXIAL, bmpView1, wxT("Toggle Axial"));
  toolBar->AddTool(VIEWER_TOGGLE_CORONAL, bmpView2, wxT("Toggle Coronal"));
  toolBar->AddTool(VIEWER_TOGGLE_SAGITTAL, bmpView3, wxT("Toggle Sagittal"));
  toolBar->AddTool(VIEWER_TOGGLE_ALPHA, bmpNewSurface, wxT("Toggle alpha blending"));
  toolBar->AddSeparator();
  toolBar->AddTool(VIEWER_RELOAD_SHADER, bmpMiniCat, wxT("Reload Shaders"));
  toolBar->AddSeparator();
  toolBar->AddTool(VIEWER_NEW_SELBOX, bmpMiniCat, wxT("New Selection Box"));
  toolBar->AddTool(VIEWER_RENDER_SELBOXES, bmpMiniCat, wxT("Toggle Selection Boxes"));
  toolBar->AddTool(VIEWER_TOGGLE_SELBOX, bmpHideSelbox , wxT("Toggle activation status of selection box"));
  toolBar->AddSeparator();
  toolBar->AddTool(VIEWER_DRAW_POINTS, bmpGBALL, wxT("Toggle drawing of points"));
  toolBar->AddTool(VIEWER_NEW_SURFACE, bmpNewSurface, wxT("New Spline Surface"));
  toolBar->AddSeparator();

  toolBar->Realize();
  frame->SetToolBar(toolBar);

  wxStatusBar* statusBar = new wxStatusBar(frame, wxID_ANY, wxST_SIZEGRIP);
  frame->SetStatusBar(statusBar);
  int widths[] = {220, 150, -1};
  statusBar->SetFieldsCount(WXSIZEOF(widths), widths);
  frame->setMStatusBar(statusBar);

  frame->Show(true);

  SetTopWindow(frame);

  frame->loadStandard();

  return true;
}


