/***************************************************************
 * Name:      MultiThreadingMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Chester ()
 * Created:   2012-08-22
 * Copyright: Chester ()
 * License:
 **************************************************************/

#include "MultiThreadingMain.h"
#include <wx/msgdlg.h>
#include <wx/dcbuffer.h>
#include <wx/app.h>
#include <wx/thread.h>

//(*InternalHeaders(MultiThreadingFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(MultiThreadingFrame)
const long MultiThreadingFrame::idMenuTest = wxNewId();
const long MultiThreadingFrame::idMenuClear = wxNewId();
const long MultiThreadingFrame::idMenuQuit = wxNewId();
const long MultiThreadingFrame::idMenuAbout = wxNewId();
const long MultiThreadingFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(MultiThreadingFrame,wxFrame)
    //(*EventTable(MultiThreadingFrame)
    EVT_MOTION(MultiThreadingFrame::OnMotion)
    EVT_PAINT(MultiThreadingFrame::OnPaint)
    EVT_ERASE_BACKGROUND(MultiThreadingFrame::OnErase)
    //*)
END_EVENT_TABLE()



int MultiThreadingFrame::sX = 0;

MultiThreadingFrame::MultiThreadingFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(MultiThreadingFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    SetClientSize(wxSize(826,426));
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem3 = new wxMenuItem(Menu1, idMenuTest, _("測試(&T)"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem3);
    MenuItem4 = new wxMenuItem(Menu1, idMenuClear, _("清除畫面(&C)"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem4);
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("結束應用程式\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("檔案(&F)"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MultiThreadingFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MultiThreadingFrame::OnAbout);
    //*)

    Connect(idMenuTest,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MultiThreadingFrame::OnTest);
    Connect(idMenuClear,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MultiThreadingFrame::OnClear);

#ifdef USING_MUTEX
    wxMutexLocker lock(m_mutex);
#endif

    for(int i=0; i<NUM_OF_THREADS; i++)
    {
        m_pDrawThread[i] = NULL;
    }




}

MultiThreadingFrame::~MultiThreadingFrame()
{
    //(*Destroy(MultiThreadingFrame)
    //*)


}

void MultiThreadingFrame::OnTest(wxCommandEvent& event)
{
    MultiThreadingFrame::sX = 0;
    for(int i=0; i<NUM_OF_THREADS; i++)
    {
        m_pDrawThread[i] = new DrawThread();
#ifdef USING_MUTEX
        m_pDrawThread[i]->SetFrame(&m_mutex, this, i);
#else
        m_pDrawThread[i]->SetFrame(NULL, this, i);
#endif
        if(m_pDrawThread[i]->Create() !=  wxTHREAD_NO_ERROR)
        {
            wxLogError(wxT("Can not create a thread!"));
        }

        m_pDrawThread[i]->Run();
    }
}

void MultiThreadingFrame::OnClear(wxCommandEvent& event)
{
    wxClientDC dc(this);
    PaintBackground(dc);
}


void MultiThreadingFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void MultiThreadingFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}


void MultiThreadingFrame::OnMotion(wxMouseEvent& event)
{
    if(event.Dragging())
    {
        wxClientDC dc(this);
        wxPen pen(*wxRED, 1);
        dc.SetPen(pen);
        dc.DrawPoint(event.GetPosition());
        dc.SetPen(wxNullPen);
    }
}

void MultiThreadingFrame::OnPaint(wxPaintEvent& event)
{
    wxBufferedPaintDC dc(this);
    PrepareDC(dc);
    PaintBackground(dc);
}

void MultiThreadingFrame::PaintBackground(wxDC& dc)
{
    wxColour backgroundColour = GetBackgroundColour();
    if(!backgroundColour.Ok())
    {
        backgroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE);
    }

    dc.SetBrush(wxBrush(backgroundColour));
    dc.SetPen(wxPen(backgroundColour, 1));
    wxRect windowRect(wxPoint(0,0), GetClientSize());

    dc.DrawRectangle(windowRect);
}

void MultiThreadingFrame::OnErase(wxEraseEvent& event)
{


}
