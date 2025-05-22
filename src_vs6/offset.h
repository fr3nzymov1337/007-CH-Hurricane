#ifndef __OFFSET_H__
#define __OFFSET_H__

class AutoOffset
{
private:
	DWORD HwBase, HwSize, HwEnd;
	DWORD ClBase, ClSize, ClEnd;
	DWORD UiBase, UiSize, UiEnd;
	DWORD HlBase, HlSize, HlEnd;

	BOOL __comparemem(const UCHAR *buff1, const UCHAR *buff2, UINT size);
	ULONG __findmemoryclone(const ULONG start, const ULONG end, const ULONG clone, UINT size);
	ULONG __findreference(const ULONG start, const ULONG end, const ULONG address);

	DWORD FarProc(const DWORD Address, DWORD LB, DWORD HB);
	DWORD dwOldPageProtection;

public:
	BYTE HLType;

	bool GetRendererInfo();

	unsigned Absolute(DWORD Addr);
	void Error(const PCHAR Msg);
	DWORD GetModuleSize(const DWORD Address);
	DWORD MessagePtr(char Message[], BYTE size);
	DWORD dwSendPacketPointer, dwSendPacketBackup, dwSpeedPointer;

	bool CheckByte(DWORD Address, BYTE Value, int Offset);
	bool EnablePageWrite(DWORD addr, DWORD size);

	PVOID ClientFuncs();
	PVOID EngineFuncs();
	PVOID StudioFuncs();
	void *Sound();
	DWORD CL_Move(void);
	void GlobalTime();
	DWORD FindSpeed(void);

	PUserMsg FindUserMsgBase();
};

extern AutoOffset c_Offset;

#endif