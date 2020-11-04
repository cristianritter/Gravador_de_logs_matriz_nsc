#include <stdio.h>
#include <windows.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <Commctrl.h>
//#include <system.h>
using namespace std;

/*BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	char String[255];
	if (!hWnd)
		return TRUE;		// Not a window
	if (!::IsWindowVisible(hWnd))
		return TRUE;		// Not visible
	if (!SendMessage(hWnd, WM_GETTEXT, sizeof(String), (LPARAM)String))
		return TRUE;		// No window title
	printf("%s\n", String);
	return TRUE;
}
*/
HWND janela;
HWND subjanela;
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    char buffer[128];
    int written = GetWindowTextA(hwnd, buffer, 128);
    if (written && strstr(buffer,"Bloco") != NULL)
    {
        //  *(HWND*)lParam = hwnd;
        janela = hwnd;
        return FALSE;
    }
    return TRUE;
}


void DumpNotepadText()
{
    cout << "[DumpNotepadText]" << endl;

    cout << "- procurando LogViewer window" << endl;
    HWND hwndNotepad = janela;

    if(NULL != hwndNotepad)
    {
        cout << "- find Edit control window" << endl;
        HWND hwndSubEdit = FindWindowEx(janela, NULL, "EDIT", NULL);

//       	HWND hwndSUB = FindWindowEx(hwndSubEdit, NULL, "ThunderRT6FormDC", NULL);

        //     	HWND hwndEdit = FindWindowEx(hwndSUB, NULL, "MSFlexGridWndClass", NULL);

 //       cout << janela;
  //      cout << hwndSubEdit;
//cout << hwndSUB;
//cout << hwndEdit;
        if(NULL != hwndSubEdit)
        {
            cout << "- get text length" << endl;
            //    int textLen = (int)SendMessage(hwndEdit, WM_GETTEXTLENGTH, 0, 0);
            int textLen = (int)SendMessage(hwndSubEdit, WM_GETTEXTLENGTH, 0, 0);
            cout << "text length :" << textLen;
            if(0 < textLen)
            {
                cout << "- get text (up to 1024 chars, inc term null)" << endl;
                const int bufferSize = 1024;
                char textBuffer[bufferSize] = "";
                //	SendMessage(hwndEdit, WM_GETTEXT, (WPARAM)bufferSize, (LPARAM)textBuffer);
//                SendMessage(hwndEdit, WM_GETTEXT, (WPARAM)bufferSize, (LPARAM)textBuffer);
                SendMessage(hwndSubEdit, WM_GETTEXT, (WPARAM)bufferSize, (LPARAM)textBuffer);
                SendMessage(hwndSubEdit, WM_SETTEXT, (WPARAM)bufferSize, (LPARAM)"alterando o texto do notepad");
                SendMessage(janela, WM_SETTEXT, (WPARAM)bufferSize, (LPARAM)"Notepad do CRistian");

                cout << "[begin text]" << endl;
                cout << textBuffer     << endl;
                cout << "[end text]"   << endl;
            }
            else
            {
                cerr << "? No text there! :-(" << endl;
            }
        }
        else
        {
            cerr << "? Huh? Can't find Edit control!?!" << endl;
        }
    }
    else
    {
        cerr << " LogViewer esta fechado? :-(" << endl;
    }
}


int main()
{
    EnumWindows(EnumWindowsProc, NULL);
    DumpNotepadText();
    system("pause");
    return 0;
}

