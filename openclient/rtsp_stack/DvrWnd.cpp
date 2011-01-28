#include <stdio.h>
#include <wtypes.h>
#include <string.h>
#include "HikData.h"

//extern 
HINSTANCE hookInst;


static LRESULT   CALLBACK WndProc(HWND   hWnd,   UINT   message,WPARAM   wParam,   LPARAM   lParam)  
{  
	char msg[256];
	switch (message)
	{
	case WM_DVR_MSG:
		sprintf(msg, "WM_DVR_MSG, lParam = %d\n", lParam);
		OutputDebugString("OnDvrMsg\n");
		break;

	}
	return 0;
} 

void Hik_ReleaseInstance(HikInstance* inst)
{
	DestroyWindow(inst->hWnd);
	delete inst;
}

HWND CreateWnd()  
{  
	WNDCLASSEX   wc;  
	HWND   tmp; 
	static BOOL reg = FALSE;
	
	wc.cbSize   =   sizeof(WNDCLASSEX);  
	wc.style   =   CS_HREDRAW   |   CS_VREDRAW;    
	wc.lpfnWndProc   =   (WNDPROC)WndProc;  
	wc.hInstance   =   hookInst;  
	wc.hCursor   =   LoadCursor(NULL,   IDC_ARROW);  
	wc.hbrBackground   =   (HBRUSH)COLOR_WINDOW;  
	wc.lpszClassName   =   "AnyWnd";    
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = "Menu";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!reg)
	{
		if (!RegisterClassEx(&wc))
		{
			DWORD err = GetLastError();
			return NULL;
		}
	}

	reg = TRUE;
	
	tmp   =   CreateWindowEx(NULL,"AnyWnd","",  
		WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,  
		CW_USEDEFAULT,CW_USEDEFAULT,  
		CW_USEDEFAULT,NULL,NULL,hookInst,NULL);  
	return tmp;
}

DWORD WINAPI MessageLoop(LPVOID pParam)
{
	MSG msg;
	HikInstance* pInst = (HikInstance*)pParam;
	pInst->hWnd = CreateWnd();
	pInst->state = HikInstance_Running;
	while (GetMessage(&msg, pInst->hWnd, NULL, NULL) && pInst->state != HikInstance_Quit)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Hik_ReleaseInstance(pInst);
	return 0;
}


void StopHikMessageLoop(HikInstance* inst)
{
	inst->state = HikInstance_Quit;
	::PostMessage(inst->hWnd, WM_QUIT, 0, 0);
}