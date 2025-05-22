//==============================================================================
// Chat.h
// credits to deltashark
//==============================================================================


//==============================================================================
#ifndef CHAT_H
#define CHAT_H
//==============================================================================

class ChatInput
{
private:
	HWND mainhwnd;
public:
	void Init();
	void OpenInput();
	void CloseInput();
	void EnterInput(char *Text);
	int GetInputText(char *Text,int size);
	int KeyEvent(int keynum);
	void SetEditFocus();	
	int Active, mode, width, height;
};
extern void DrawChat();
extern ChatInput gChatInput;
//==============================================================================
#endif
//==============================================================================