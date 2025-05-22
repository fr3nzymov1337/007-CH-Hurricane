#include "client.h"

#define WINDOW_CLASS_NAME "CHAT_INPUT_WINDOWS_CLASS"

void TrapezoidalConsole(int con_x, int con_y, int con_w, int con_h);

ChatInput gChatInput;

HWND hEdit;
WNDPROC pfnOrigProc;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps; 
	HDC hdc;

	switch(msg)
	{
	case WM_CREATE: 
		{
			return(0);
		} 
		break;

	case WM_PAINT: 
		{
			hdc = BeginPaint(hwnd,&ps);  
			EndPaint(hwnd,&ps);
			return(0);
		} break;

		case WM_DESTROY: 
		{
			return(0);
		} 
		break;

		default:break;
    } 

	return (DefWindowProc(hwnd, msg, wparam, lparam));
} 

LRESULT CALLBACK EditWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
		case VK_RETURN: 
			{
				if (msg==WM_KEYDOWN)
				{
					gChatInput.Active = false;
					char Text[255];

					gChatInput.GetInputText(Text, 255);
					gChatInput.EnterInput(Text);
					gChatInput.CloseInput();
				}
				return(0);
			} 
			break;

		case VK_ESCAPE: 
			{
				if (msg==WM_KEYDOWN)
				{
					gChatInput.Active = false;
					char Text[255];

					gChatInput.EnterInput(Text);
					gChatInput.CloseInput();
				}
				return(0);
			} 
			break;

		default:break;
    } 

	return (pfnOrigProc(hwnd, msg, wparam, lparam));

} 

void ChatInput::OpenInput()
{
    hEdit = CreateWindowEx(WS_EX_CLIENTEDGE|WS_EX_NOPARENTNOTIFY, "EDIT", NULL, WS_CHILDWINDOW | ES_AUTOHSCROLL| WS_VISIBLE | WS_TABSTOP, 0, 0, 0, 0, mainhwnd, 0, NULL, 0);

	HDC dc = GetDC(hEdit);
	SetBkMode(dc,TRANSPARENT);
	SendMessage(hEdit,EM_LIMITTEXT,72,0L);

	pfnOrigProc = (WNDPROC) GetWindowLong(hEdit, GWL_WNDPROC);

	SetWindowLong(hEdit, GWL_WNDPROC, (LONG)(WNDPROC)EditWindowProc);
}

void ChatInput::CloseInput()
{
	SetFocus(mainhwnd);
}

bool firsttime = true;

void ChatInput::Init()
{
	mode = 1;

	width = 200;
	height = 18;

	mainhwnd = GetFocus();

	if (firsttime)
	{
		OpenInput();

		firsttime = false;
	}

	SetFocus(hEdit);
}

void DrawChat()
{
	int x = 150;
	int y = 500;
	int w = gChatInput.width;
	int h = gChatInput.height;
	if( !gChatInput.Active ) return;
	
	gChatInput.SetEditFocus();
	
	{
		TrapezoidalConsole(x, y, w, h);
	}

	char Text[255];
	if (gChatInput.GetInputText(Text, 255))
	{
		sprintf(Text, "%s%s", Text, "_");
		
		int length, height;

		g_Engine.pfnDrawConsoleStringLen(Text, &length, &height);

		if (length + 10 > gChatInput.width)
		{
			gChatInput.width = length + 10;
		}

		g_Engine.pfnDrawSetTextColor(1.0f,1.0f,1.0f);
		g_Engine.pfnDrawConsoleString(x+5,y,Text);
	}
	else
	{
		sprintf(Text, "%s", "_");
		g_Engine.pfnDrawSetTextColor(1.0f,1.0f,1.0f);
		g_Engine.pfnDrawConsoleString(x+5,y,Text);
	}
}

int ChatInput::KeyEvent(int keynum)
{
	return 0;
}

void ChatInput::SetEditFocus()
{
	SetFocus(hEdit);
}

void ChatInput::EnterInput(char *Text)
{
	SetWindowText(hEdit, "");

	char SayText[255];
	if (mode == 1)
	{
		sprintf(SayText, "say \"%s\"", Text);
	}
	else if (mode == 2)
	{
		sprintf(SayText, "say_team \"%s\"", Text);
	}

	g_Engine.pfnClientCmd(SayText);
}

int ChatInput::GetInputText(char *Text,int size)
{
	char EditText[255];

	int len = GetWindowText(hEdit, EditText, 255);

	if (len)
	{
		string uText;

		gChineseCodeLib.GB2312ToUTF_8(uText, EditText, strlen(EditText));
	

		if (uText.length())
		{		
			sprintf(Text, "%s", uText.c_str());
			return uText.length();
		}
	}

	return 0;
}