
#if !defined(__CCHINESECODELIB_H_)
#define __CCHINESECODELIB_H_

class CChineseCodeLib  
{
public:
	static void UTF_8ToGB2312(string& pOut,char *pText, int pLen);
	static void GB2312ToUTF_8(string& pOut,char *pText, int pLen);
	// Unicode 转换成UTF-8
	static void UnicodeToUTF_8(char* pOut,WCHAR* pText);
	// GB2312 转换成　Unicode
	static void Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer);
	// 把Unicode 转换成 GB2312
	static void UnicodeToGB2312(char* pOut,unsigned short uData);
	// 把UTF-8转换成Unicode
	static void UTF_8ToUnicode(WCHAR* pOut,char* pText);

	CChineseCodeLib();
	virtual ~CChineseCodeLib();
};

extern CChineseCodeLib gChineseCodeLib;

#endif
