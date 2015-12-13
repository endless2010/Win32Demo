#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

TCHAR szDefaultText[]=TEXT("hello world");
TCHAR szCaption[]=TEXT("how are you?");


int WINAPI WinMain (HINSTANCE hInstance,HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,int iShowCmd)
{
	static TCHAR szAppName[]=TEXT("cliptext");
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
	
	wndclass.lpszMenuName=szAppName;
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
	static PSTR pText;
	BOOL bEnable;
	HGLOBAL hGlobal;
	HDC hdc;
	PTSTR pGlobal;
	PAINTSTRUCT ps;
	RECT rect;
	
	switch(message)
	{
	case WM_CREATE:
		SendMessage(hwnd,WM_COMMAND,IDM_EDIT_RESET,0);
		return 0;
		
	case WM_INITMENUPOPUP:
		EnableMenuItem((HMENU)wParam,IDM_EDIT_PASTE,
			IsClipboardFormatAvailable(CF_TEXT)?MF_ENABLED:MF_GRAYED);
		
		bEnable=pText?MF_ENABLED:MF_GRAYED;
		
		EnableMenuItem((HMENU)wParam,IDM_EDIT_CUT,bEnable);
		EnableMenuItem((HMENU)wParam,IDM_EDIT_COPY,bEnable);
		EnableMenuItem((HMENU)wParam,IDM_EDIT_CLEAR,bEnable);
		
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDM_EDIT_PASTE:
			OpenClipboard(hwnd);
			
			if(hGlobal=GetClipboardData(CF_TEXT))
			{
				pGlobal=(PTSTR)GlobalLock(hGlobal);
				if(pText)
				{
					free(pText);
					pText=NULL;
				}
				pText=(PTSTR)malloc(GlobalSize(hGlobal));
				lstrcpy(pText,pGlobal);
				InvalidateRect(hwnd,NULL,TRUE);
			}
			CloseClipboard();
			return 0;
			
		case IDM_EDIT_CUT:
		case IDM_EDIT_COPY:
			if(!pText)
				return 0;
			
			hGlobal=GlobalAlloc(GHND|GMEM_SHARE,(lstrlen(pText)+1)*sizeof(TCHAR));
			pGlobal=(PTSTR)GlobalLock(hGlobal);
			lstrcpy(pGlobal,pText);
			GlobalUnlock(hGlobal);
			
			OpenClipboard(hwnd);
			EmptyClipboard();
			SetClipboardData(CF_TEXT,hGlobal);
			CloseClipboard();
			
			if(LOWORD(wParam)==IDM_EDIT_COPY)
				return 0;
			
		case IDM_EDIT_CLEAR:
			if(pText)
			{
				free(pText);
				pText=NULL;
			}
			InvalidateRect(hwnd,NULL,TRUE);
			return 0;
			
		case IDM_EDIT_RESET:
			if(pText)
			{
				free(pText);
				pText=NULL;
			}
			
			pText=(PTSTR)malloc((lstrlen(szDefaultText)+1)*sizeof(TCHAR));
			lstrcpy(pText,szDefaultText);
			InvalidateRect(hwnd,NULL,TRUE);
			return 0;
		}
		break;
		
		case WM_PAINT:
			hdc=BeginPaint(hwnd,&ps);
			GetClientRect(hwnd,&rect);
			
			if(pText!=NULL)
				DrawText(hdc,pText,-1,&rect,DT_EXPANDTABS|DT_WORDBREAK);
			
			EndPaint(hwnd,&ps);
			return 0;
			
		case WM_DESTROY:
			if(pText)
				free(pText);
			
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}

