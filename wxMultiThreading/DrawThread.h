#ifndef DRAWTHREAD_H
#define DRAWTHREAD_H
#include <wx/thread.h>

#define	NUM_OF_THREADS	6

#define USING_MUTEX 1


class MultiThreadingFrame;

class DrawThread : public wxThread
{
    public:
        DrawThread();
        void SetFrame(wxMutex* pMutex, MultiThreadingFrame* frame, int no)
        {
            m_pMutex = pMutex;
            m_frame = frame;
            m_no = no;
        }
        virtual ~DrawThread();
        virtual void *Entry();
    protected:
    private:
        wxMutex* m_pMutex;
        MultiThreadingFrame*    m_frame;
        int m_no;
};

#endif // DRAWTHREAD_H
