#include "client.h"

CChineseCodeLib gChineseCodeLib;

CChineseCodeLib::CChineseCodeLib()
{

}

CChineseCodeLib::~CChineseCodeLib()
{

}

void CChineseCodeLib::UTF_8ToUnicode(WCHAR* pOut,char *pText)
{
	char* uchar = (char *)pOut;
	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
	return;
}

void CChineseCodeLib::UnicodeToGB2312(char* pOut,unsigned short uData)
{
	WideCharToMultiByte(CP_ACP,NULL,(LPCWSTR)&uData,1,pOut,sizeof(WCHAR),NULL,NULL);
	return;
}

void CChineseCodeLib::Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer)
{
	::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
	return;
}

void CChineseCodeLib::UnicodeToUTF_8(char* pOut,WCHAR* pText)
{
	char* pchar = (char *)pText;
	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
	pOut[2] = (0x80 | (pchar[0] & 0x3F));
	return;
}

void CChineseCodeLib::GB2312ToUTF_8(string& pOut,char *pText, int pLen)
{
	char buf[4];
	char* rst = new char[pLen + pLen*2];
	memset(buf,0,4);
	memset(rst,0,pLen + pLen*2);
	int i = 0;
	int j = 0;	
	while(i < pLen)
	{
		if( *(pText + i) >= 0)
		{
			rst[j++] = pText[i++];
		}
		else
		{
			WCHAR pbuffer;
			Gb2312ToUnicode(&pbuffer,pText+i);
			UnicodeToUTF_8(buf,&pbuffer);
			unsigned short int tmp = 0;
			tmp = rst[j] = buf[0];
			tmp = rst[j+1] = buf[1];
			tmp = rst[j+2] = buf[2];
			j += 3;
			i += 2;
		}
	}
	rst[j] = '\0';
	pOut = rst;		
	delete []rst;	
	return;
}

void CChineseCodeLib::UTF_8ToGB2312(string &pOut, char *pText, int pLen)
{
	 char * newBuf = new char[pLen];
	 char Ctemp[4];
	 memset(Ctemp,0,4);

	 int i =0;
	 int j = 0;
	 
	 while(i < pLen)
	 {
	 	if(pText[i] > 0)
		{
			newBuf[j++] = pText[i++];			
		}
		else                 
		{
			WCHAR Wtemp;
			UTF_8ToUnicode(&Wtemp,pText + i);
			UnicodeToGB2312(Ctemp,Wtemp);
			newBuf[j] = Ctemp[0];
			newBuf[j + 1] = Ctemp[1];
			i += 3;    
			j += 2;   
		}
	 }
	 newBuf[j] = '\0';
	 pOut = newBuf;
	 delete []newBuf;
	 return; 
}

