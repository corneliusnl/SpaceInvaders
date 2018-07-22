#include <string.h>
#include <stdlib.h>

typedef struct
{
	HINSTANCE 	hInstance;
	HINSTANCE 	hPrevInst;
	int			cmdShow;
} WINAPP;

LRESULT CALLBACK _export WndProc(HWND, UINT, WPARAM, LPARAM);

