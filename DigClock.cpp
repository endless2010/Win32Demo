#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);

int WINAPI WinMain (HINSTANCE hInstance,HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,int iShowCmd)
{
	static TCHAR szAppName[]=TEXT("DigClock");
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

	hwnd=CreateWindow(szAppName,TEXT("Digital Clock Demostration"),WS_OVERLAPPEDWINDOW,
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

static bool fSevenSegment[10[7]={
								1,1,1,0,1,1,1
								0,0,0,0,0,1,0
								1,0,1,1,1,0,1
								1,0,1,1,0,1,1
								0,1,1,1,0,1,0
								1,1,0,1,0,1,1
								1,1,0,1,1,1,1
								1,0,1,0,0,1,0
								1,1,1,1,1,1,1
								1,1,1,1,0,1,1};
static POINT ptSegment[7][6]={
	7,6,11,2,31,2,35,6,31,10,11,10,
	6,7,10,11,10,31,6,35,2,31,2,11,
	36,7,40,11,40,31,36,35,32,31,32,11,
	7,36,11,32,31,32,35,36,31,40,11,40,
	6,37,10,41,10,61,6,65,2,61,2,41,
	36,37,40,41,40,61,36,65,32,61,32,41,
	7,66,11,62,31,62,35,66,31,70,11,70};
int iSeg;

for(

LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}
