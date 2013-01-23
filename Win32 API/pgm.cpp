//
// pgm.cpp
//
// (c)Copyright Spacesoft corp., 2002 All rights reserved.
//                               Hiro KITAYAMA
//
#include <windows.h>
#include <stdlib.h>
#include "resource.h"

#define	NUM_OF_THREADS	6

HINSTANCE g_hInstance ;			// instance handle

LPCSTR szTitle = "範例";	// 標題列名稱
LPCSTR szWindowClass = "MENU";	// Window class名稱

int sX ;

HANDLE mutex;

struct ThreadArgument
{
	HWND hWnd;
	int NO;
};


//
// draw line
//
void drawThread( LPVOID arg )
{
	HDC hDC ;
	RECT rc ;
	int sY  ;
	HANDLE hOldPen, hPen ;

	ThreadArgument *pArg = (ThreadArgument *)arg;

	hDC = GetDC( pArg->hWnd ) ;
	GetClientRect( pArg->hWnd, &rc ) ;	// get window size
	hPen = CreatePen( PS_SOLID, 1, (COLORREF)RGB( (255/6) * pArg->NO, 255 - (255/6) * pArg->NO, 0 ) ) ;
	hOldPen = SelectObject( hDC, hPen ) ;
	SetROP2( hDC, R2_XORPEN ) ;

	while( sX < rc.right ) {
	    
		WaitForSingleObject( mutex, INFINITE ) ;	// <---------------

		sY = rc.bottom * sX / rc.right ;

		MoveToEx( hDC, sX, rc.bottom - sY - 1, NULL ) ;
		Sleep( 1 ) ;
		LineTo( hDC, sX, sY ) ;
		
		Sleep(1);
		sX++ ;

		ReleaseMutex( mutex ) ;							// <------------------
	}
	SelectObject( hDC, hOldPen ) ;
	DeleteObject( hPen ) ;
	ReleaseDC( pArg->hWnd, hDC ) ;

	ExitThread(0);
}


//
//  window procedure
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HANDLE hThread[NUM_OF_THREADS] ;
	static ThreadArgument arg[NUM_OF_THREADS] ;
	static DWORD threadID ;
	int wmId, wmEvent ;
	unsigned int loop ;

	switch( message ) {

		case WM_CREATE:
			mutex = CreateMutex( 0, FALSE, 0 ) ;
			break ;

		case WM_COMMAND:			// user menu
			wmId    = LOWORD( wParam ) ;
			wmEvent = HIWORD( wParam ) ;
			switch( wmId ) {
				case IDM_TEST:
					sX = 0 ;
					for( loop = 0 ; loop < NUM_OF_THREADS ; loop++ ) 
					{
						arg[loop].hWnd = hWnd;
						arg[loop].NO = loop;

						hThread[loop] = CreateThread( 0, 0,	// create a thread
								(LPTHREAD_START_ROUTINE)drawThread, 
												(VOID *)&(arg[loop]), 0, &threadID ) ;
					}
					while( WaitForMultipleObjects( NUM_OF_THREADS, hThread, 
														TRUE, 0) == WAIT_TIMEOUT )
						Sleep( 10 ) ;
					break;

				case IDM_CLEAR:
					InvalidateRgn( hWnd, NULL, TRUE ) ;
					break ;

				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc( hWnd, message, wParam, lParam ) ;
			}
			break ;
			
		case WM_DESTROY:
			CloseHandle( mutex ) ;
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc( hWnd, message, wParam, lParam ) ;
	}
	return 0 ;
}



//
// register main window calss
//
BOOL InitApplication( HINSTANCE hInstance )
{
	WNDCLASSEX wcex ;

	ZeroMemory( &wcex, sizeof wcex ) ;
	wcex.cbSize        = sizeof( WNDCLASSEX ) ;
	wcex.style         = CS_HREDRAW | CS_VREDRAW ;
	wcex.lpfnWndProc   = (WNDPROC)WndProc ;
	wcex.cbClsExtra    = 0 ;
	wcex.cbWndExtra    = 0 ;
	wcex.hInstance     = hInstance ;
	wcex.hIcon         = LoadIcon( hInstance, "ICON" ) ;
	wcex.hIconSm       = LoadIcon( hInstance, "SICON" ) ;
	wcex.hCursor       = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH ) ;
	wcex.lpszMenuName  = szWindowClass ;
	wcex.lpszClassName = szWindowClass ;

	return RegisterClassEx( &wcex ) ;
}



//
// careate main window
//
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	HWND hWnd ;

	// instance handle存到全域變數中
	g_hInstance = hInstance ;

	hWnd = CreateWindow( szWindowClass,
						 szTitle,
						 WS_OVERLAPPEDWINDOW,
						 CW_USEDEFAULT,
						 0,
						 CW_USEDEFAULT,
						 0,
						 NULL,
						 NULL,
						 hInstance,
						 NULL);
	if( !hWnd )
		return FALSE ;

	ShowWindow( hWnd, nCmdShow ) ;
	UpdateWindow( hWnd ) ;

	return TRUE ;
}



//
// WinMain
//
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow )
{
	MSG msg ;

	// register main window calss
	if( !InitApplication( hInstance ) )
		return FALSE ;

	// create main window
	if( !InitInstance( hInstance, nCmdShow ) )
		return FALSE ;

	// dispatch messages
	while( GetMessage(&msg, NULL, 0, 0) ) {
		TranslateMessage( &msg ) ;
		DispatchMessage( &msg ) ;
	}

	//return msg.wParam ;
	return 0;
}
