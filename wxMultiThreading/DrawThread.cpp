#include "DrawThread.h"
#include "MultiThreadingMain.h"
#include <wx/dcclient.h>


DrawThread::DrawThread(): wxThread(wxTHREAD_DETACHED)
{
    //ctor
}

DrawThread::~DrawThread()
{
    //dtor
}

void *DrawThread::Entry()
{
    wxClientDC dc(m_frame);
    const wxSize& sz = dc.GetSize();
    //wxPen pen(*wxRED, 1);
    wxPen pen(wxColor((255/6) * m_no, 255 - (255/6) * m_no, 0), 1);
    dc.SetPen(pen);

    int sY = 0;

    wxRect rc = wxRect(0,0,dc.GetSize().GetWidth(), dc.GetSize().GetHeight());

    while( MultiThreadingFrame::sX < rc.GetRight() )
    {
#ifdef USING_MUTEX
        m_pMutex->Lock();
#endif
        sY = rc.GetBottom() * MultiThreadingFrame::sX / rc.GetRight();
        dc.DrawLine(MultiThreadingFrame::sX,
                    rc.GetBottom() - sY - 1,
                    MultiThreadingFrame::sX,
                    sY);

        wxMilliSleep(1);
        MultiThreadingFrame::sX++;

#ifdef USING_MUTEX
        m_pMutex->Unlock();
#endif

    }



    dc.SetPen(wxNullPen);

    return (ExitCode)0;
}
