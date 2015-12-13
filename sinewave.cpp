#include <windows.h>
#include <math.h>

#define TWO_PI (2.0*3.14159)
#define NUM 1000
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);


int WINAPI WinMain (HINSTANCE hInstance,HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,int iShowCmd)
{
	static TCHAR szAppName[]=TEXT("RandRect");
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

	hwnd=CreateWindow(szAppName,TEXT("Random Rectangles"),WS_OVERLAPPEDWINDOW,
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

LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	static int cxClient,cyClient;
	HDC hdc;
	int i;
	PAINTSTRUCT ps;
	POINT apt[NUM];
	switch(iMsg)
	{
	case WM_SIZE:
		cxClient=LOWORD(lParam);
		cyClient=HIWORD(lParam);
		return 0;

	case WM_PAINT:
		hdc=BeginPaint(hwnd,&ps);

        MoveToEx(hdc,0,cyClient/2,NULL);
		LineTo(hdc,cxClient,cyClient/2);

		for(i=0;i<NUM;++i)
		{
			apt[i].x=i*cxClient/NUM;
			apt[i].y=(int)(cyClient/2*(1-sin(TWO_PI*i/NUM)));
		}
		Polyline(hdc,apt,NUM);
		EndPaint(hwnd,&ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,iMsg,wParam,lParam);
}

