#include <windows.h>
#define ID_TIMER 1

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
void CALLBACK TimerProc(HWND,UINT,UINT,DWORD);


int WINAPI WinMain (HINSTANCE hInstance,HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,int iShowCmd)
{
	static TCHAR szAppName[]=TEXT("Beeper2");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style=CS_HREDRAW|CS_VREDRAW;
	wndclass.lpfnWndProc=WndProc;
	wndclass.cbClsExtra=0;
	wndclass.cbWndExtra=0;
	wndclass.hInstance=hInstance;
	wndclass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);
	wndclass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);

	wndclass.lpszMenuName=NULL;
	wndclass.lpszClassName=szAppName;

	if(!RegisterClass(&wndclass))
	{
		MessageBox(NULL,TEXT("This program requies windows NT!"),szAppName,MB_ICONERROR);
		return 0;
	}

	hwnd=CreateWindow(szAppName,TEXT("Beeper2 Demo"),WS_OVERLAPPEDWINDOW,
		              CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
					  NULL,NULL,hInstance,NULL);
	ShowWindow(hwnd,iShowCmd);
	UpdateWindow(hwnd);

	while(GetMessage(&msg,NULL,0,0))
	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{

	switch(message)
	{
	case WM_CREATE:
		SetTimer(hwnd,ID_TIMER,1000,TimerProc);
		return 0;

	case WM_DESTROY:
		KillTimer(hwnd,ID_TIMER);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}
void CALLBACK TimerProc(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime)
{
	static bool fFlipFlop=false;
	HBRUSH hBrush;
	HDC hdc;
	RECT rc;
	MessageBeep(-1);
	fFlipFlop=!fFlipFlop;

	GetClientRect(hwnd,&rc);

	hdc=GetDC(hwnd);
	hBrush=CreateSolidBrush(fFlipFlop?RGB(255,0,0):RGB(0,0,255));
	FillRect(hdc,&rc,hBrush);

	ReleaseDC(hwnd,hdc);
	DeleteObject(hBrush);
}

