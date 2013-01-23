/***************************************************************
 * Name:      MultiThreadingMain.h
 * Purpose:   Defines Application Frame
 * Author:    Chester ()
 * Created:   2012-08-22
 * Copyright: Chester ()
 * License:
 **************************************************************/

#ifndef MULTITHREADINGMAIN_H
#define MULTITHREADINGMAIN_H

//(*Headers(MultiThreadingFrame)
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)


#include <wx/frame.h>
#include <wx/dynarray.h>

#include "DrawThread.h"




//class DrawThread;

//WX_DEFINE_ARRAY_PTR(DrawThread *, wxArrayThread);




class MultiThreadingFrame: public wxFrame
{
    public:

        MultiThreadingFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~MultiThreadingFrame();

#ifdef USING_MUTEX
        wxMutex m_mutex;
#endif
        DrawThread *m_pDrawThread[NUM_OF_THREADS];
        static int sX;


    private:

        //(*Handlers(MultiThreadingFrame)
        void OnTest(wxCommandEvent& event);
        void OnClear(wxCommandEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        //*)

        //(*Identifiers(MultiThreadingFrame)
        static const long idMenuTest;
        static const long idMenuClear;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(MultiThreadingFrame)
        wxMenuItem* MenuItem4;
        wxMenuItem* MenuItem3;
        wxStatusBar* StatusBar1;
        //*)

        void PaintBackground(wxDC& dc);
        void OnMotion(wxMouseEvent& event);
        void OnPaint(wxPaintEvent& event);
        void OnErase(wxEraseEvent& event);

        DECLARE_EVENT_TABLE()
};






#endif // MULTITHREADINGMAIN_H
