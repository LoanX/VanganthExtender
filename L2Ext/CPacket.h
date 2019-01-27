#pragma once
#include <windows.h>

LPVOID L2_New(int nSize);
void L2_Delete(LPVOID pPointer);

class CPacket
{
	size_t pos;
	bool readonly;
	CHAR buff[8190];
	PUCHAR readBuff;
public:
	CPacket();
	CPacket(PUCHAR packet);
	void WriteC(BYTE cData);
	void WriteH(INT16 hData);
	void WriteD(INT32 dData);
	void WriteF(double fData);
	void WriteQ(INT64 iData);
	void WriteB(int nBuffSize, PCHAR Buff);
	void WriteB(int nBuffSize, const unsigned char* Buff);
	void WriteS(PWCHAR wText);
	void WriteS(const wchar_t* wText);
	size_t GetSize();
	PCHAR GetBuff();
	BYTE ReadC();
	INT16 ReadH();
	INT32 ReadD();
	INT64 ReadQ();
	double ReadF();
	PWCHAR ReadS();
	char *ReadB(int nLen);
	virtual ~CPacket();
};