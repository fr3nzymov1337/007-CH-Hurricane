#include "client.h"

glReadPixels_t glReadPixels_s;

BOOL ScreenFirst = TRUE;
bool bAntiSSTemp = true;
bool DrawVisuals;
PBYTE BufferScreen;
int temp;
DWORD dwSize, time_scr;

void __stdcall m_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
{
	if (ScreenFirst)
	{
		ScreenFirst = FALSE;
		dwSize = ( width * height ) * 3;
		BufferScreen = (PBYTE)malloc(dwSize);
		glReadPixels_s(x, y, width, height, format, type, pixels);
		__try
		{
			memcpy(BufferScreen, pixels, dwSize);
		}
		__except (EXCEPTION_EXECUTE_HANDLER){};
		DrawVisuals = true;
		return;
	}
	__try
	{
		memcpy(pixels, BufferScreen, dwSize);
	}
	__except (EXCEPTION_EXECUTE_HANDLER){};
}

void Snapshot()
{
	if (bAntiSSTemp)
	{
		time_scr = GetTickCount();
		temp = 0;
		
		bAntiSSTemp = false;
	}

	if (GetTickCount() - time_scr > cvar.misc_snapshot_time*1000)
	{
		DrawVisuals = false;
		temp++;
		
		if (temp > 10)
		{
			bAntiSSTemp = true;
			ScreenFirst = TRUE;
			DWORD sz = ( g_Screen.iWidth * g_Screen.iHeight ) * 3;
			free( (PBYTE)BufferScreen );
			PBYTE buf = (PBYTE)malloc( sz );
			glReadPixels( 0 , 0 , g_Screen.iWidth , g_Screen.iHeight , GL_RGB , GL_UNSIGNED_BYTE , buf );
			free( (PBYTE)buf );
		}
	}

	static bool FirstFrame = true;

	if (FirstFrame)
	{
		DWORD sz = g_Screen.iWidth * g_Screen.iHeight * 3;
		PBYTE buf = (PBYTE)malloc( sz );
		glReadPixels( 0 , 0 , g_Screen.iWidth , g_Screen.iHeight , GL_RGB , GL_UNSIGNED_BYTE , buf );
		free( (PBYTE)buf );

		FirstFrame = false;
	}
}
