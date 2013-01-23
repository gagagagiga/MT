/***************************************************************
 * Name:      MultiThreadingApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Chester ()
 * Created:   2012-08-22
 * Copyright: Chester ()
 * License:
 **************************************************************/

#include "MultiThreadingApp.h"

//(*AppHeaders
#include "MultiThreadingMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(MultiThreadingApp);

bool MultiThreadingApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	MultiThreadingFrame* Frame = new MultiThreadingFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
