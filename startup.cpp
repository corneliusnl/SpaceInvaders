// ##################################
// #<<<<<<< Startup Dialog >>>>>>>>>#
// ##################################
#include <Windows.h>

// ##################################
// #<<<<< Startup Dlg Proc >>>>>>>>>#
// ##################################
int _export FAR PASCAL StartupDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG:
        	return 0;

      case WM_DESTROY:
         return 0;
	}
	return 0;
};