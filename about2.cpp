#include "resource.h"
#include <windows.h>

#define IDR_MENU1                       101
#define IDM_ABOUT                       103
#define IDM_ABOUT2                      104
#define IDC_Black                       1000
#define IDC_Blue                        1001
#define IDC_Green                       1002
#define IDC_Cyan                        1003
#define IDC_Red                         1004
#define IDC_Magenta                     1005
#define IDC_Yellow                      1006
#define IDC_White                       1007
#define IDC_Rectangle                   1008
#define IDC_Ellipse                     1009
#define IDC_Paint                       1010
#define About2                          40001
#define IDM_APP_ABOUT                   40001

BOOL CALLBACK AboutDlgProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

int iCurrentColor=IDC_Black;
int iCurrentFigure=IDC_Rectangle;

int WINAPI WinMain (HINSTANCE hInstance,HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,int iShowCmd)
{
	static TCHAR szAppName[]=TEXT("About2");
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

	wndclass.lpszMenuName=MAKEINTRESOURCE(IDM_ABOUT);
	wndclass.lpszClassName=szAppName;

	if(!RegisterClass(&wndclass))
	{
		MessageBox(NULL,TEXT("This program requies windows NT!"),szAppName,MB_ICONERROR);
		return 0;
	}

	hwnd=CreateWindow(szAppName,TEXT("About Dialog Demo"),WS_OVERLAPPEDWINDOW,
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

void PaintWindow(HWND hwnd,int iColor, int iFigure)
{
	static COLORREF crColor[8]={RGB(0,0,0),RGB(0,0,255),RGB(0,255,0),RGB(0,255,255),
		RGB(255,0,0),RGB(255,0,255),RGB(255,255,0),RGB(255,255,255)};
	HBRUSH hBrush;
	HDC hdc;
	RECT rect;

	hdc=GetDC(hwnd);
	GetClientRect(hwnd,&rect);
	hBrush=CreateSolidBrush(crColor[iColor-IDC_Black]);
	hBrush=(HBRUSH)SelectObject(hdc,hBrush);

	if(iFigure==IDC_Rectangle)
		Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);
	else
		Ellipse(hdc,rect.left,rect.top,rect.right,rect.bottom);

	DeleteObject(SelectObject(hdc,hBrush));
	ReleaseDC(hwnd,hdc);
}

void PaintTheBlock(HWND hCtrl,int iColor,int iFigure)
{
	InvalidateRect(hCtrl,NULL,TRUE);
	UpdateWindow(hCtrl);
	PaintWindow(hCtrl,iColor,iFigure);
}


LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	static HINSTANCE hInstance;
	PAINTSTRUCT ps;

	switch(message)
	{
	case WM_CREATE:
		hInstance=((LPCREATESTRUCT)lParam)->hInstance;
		return 0;
	
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		  case IDM_APP_ABOUT:
			if(DialogBox(hInstance,MAKEINTRESOURCE(IDM_ABOUT2),hwnd,AboutDlgProc))	
				InvalidateRect(hwnd,NULL,TRUE);
			return 0;
		}
		break;
    
	case WM_PAINT:
	    BeginPaint(hwnd,&ps);
		EndPaint(hwnd,&ps);

		PaintWindow(hwnd,iCurrentColor,iCurrentFigure);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}

BOOL CALLBACK AboutDlgProc(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
{
	static HWND hCtrlBlock;
	static int iColor,iFigure;

	switch(message)
	{
	case WM_INITDIALOG:
		iColor=iCurrentColor;
		iFigure=iCurrentFigure;

		CheckRadioButton(hDlg,IDC_Black,IDC_White,iColor);
		CheckRadioButton(hDlg,IDC_Rectangle,IDC_Ellipse,iFigure);

		hCtrlBlock=GetDlgItem(hDlg,IDC_Paint);

		SetFocus(GetDlgItem(hDlg,iColor));
		return FALSE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			iCurrentColor=iColor;
			iCurrentFigure=iFigure;
			EndDialog(hDlg,TRUE);
			return TRUE;

		case IDCANCEL:
			EndDialog(hDlg,FALSE);
			return TRUE;

		case IDC_Black:
		case IDC_Red:
		case IDC_Green:
		case IDC_Yellow:
		case IDC_Blue:
		case IDC_Magenta:
		case IDC_Cyan:
		case IDC_White:
			iColor=LOWORD(wParam);
			CheckRadioButton(hDlg,IDC_Black,IDC_White,LOWORD(wParam));
			PaintTheBlock(hCtrlBlock,iColor,iFigure);
			return TRUE;

		case IDC_Rectangle:
		case IDC_Ellipse:
			iFigure=LOWORD(wParam);
			CheckRadioButton(hDlg,IDC_Rectangle,IDC_Ellipse,LOWORD(wParam));
			PaintTheBlock(hCtrlBlock,iColor,iFigure);
			return TRUE;
		}
		break;

		case WM_PAINT:
			PaintTheBlock(hCtrlBlock,iColor,iFigure);
			break;
	}
	return FALSE;
}
