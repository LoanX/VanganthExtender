#include "StdAfx.h"
#include "Packets.h"

void CPacketFix::SendSummonStatusFix(CSocket *pSocket, const char *format, ...)
{
	CPacket Packet;
	va_list va;
	va_start(va, format);
/*c*/	Packet.WriteC(va_arg(va, BYTE)); //header
/*d*/	int nSummonType = va_arg(va, DWORD);
		Packet.WriteD(nSummonType); // SummonType
/*d*/	Packet.WriteD(va_arg(va, DWORD)); //ObjectID
/*d*/	Packet.WriteD(va_arg(va, DWORD)); //x
/*d*/	Packet.WriteD(va_arg(va, DWORD)); //y
/*d*/	Packet.WriteD(va_arg(va, DWORD)); //z
/*S*/	Packet.WriteS(va_arg(va, PWCHAR)); //caption
/*d*/	Packet.WriteD(va_arg(va, DWORD)); // current feed
/*d*/	Packet.WriteD(va_arg(va, DWORD)); // max feed
/*d*/	Packet.WriteD(va_arg(va, DWORD)); //current hp
/*d*/	Packet.WriteD(va_arg(va, DWORD)); //max hp
/*d*/	Packet.WriteD(va_arg(va, DWORD)); //current mp
/*d*/	Packet.WriteD(va_arg(va, DWORD)); //max mp
/*d*/	int nLevel = va_arg(va, DWORD);
		Packet.WriteD(nLevel); //level
/*d*/	INT64 nExp = va_arg(va, DWORD);
		Packet.WriteQ(nExp); //exp
/*d*/	INT64 nLowestExp = va_arg(va, DWORD);
		Packet.WriteQ(nLowestExp); //lowest exp
/*d*/	INT64 nHighestExp = va_arg(va, DWORD);
		Packet.WriteQ(nHighestExp); //highest exp
	va_end(va);
	pSocket->Send("b", Packet.GetSize(), Packet.GetBuff());
};
void CPacketFix::SendSummonInfo(CSocket *pSocket, const char *format, ...)
{
	
	
	CPacket Packet;
	va_list va;
	va_start(va, format);
/*c*/	Packet.WriteC(va_arg(va, BYTE));
/*d*/	int nSummonType = va_arg(va, DWORD);
		Packet.WriteD(nSummonType);
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*f*/	Packet.WriteF(va_arg(va, double));
/*f*/	Packet.WriteF(va_arg(va, double));
/*f*/	Packet.WriteF(va_arg(va, double));
/*f*/	Packet.WriteF(va_arg(va, double));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*c*/	Packet.WriteC(va_arg(va, BYTE));
/*c*/	Packet.WriteC(va_arg(va, BYTE));
/*c*/	Packet.WriteC(va_arg(va, BYTE));
/*c*/	Packet.WriteC(va_arg(va, BYTE));
/*c*/	Packet.WriteC(va_arg(va, BYTE));
/*S*/	Packet.WriteS(va_arg(va, PWCHAR));
/*S*/	Packet.WriteS(va_arg(va, PWCHAR));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	int nLevel = va_arg(va, DWORD);
		Packet.WriteD(nLevel);
/*d*/	INT64 nExp = va_arg(va, DWORD);
		Packet.WriteQ(nExp); //exp
/*d*/	INT64 nLowestExp = va_arg(va, DWORD);
		Packet.WriteQ(nLowestExp); //lowest exp
/*d*/	INT64 nHighestExp = va_arg(va, DWORD);
		Packet.WriteQ(nHighestExp); //highest exp
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*c*/	Packet.WriteC(va_arg(va, BYTE));
/*c*/	Packet.WriteC(va_arg(va, BYTE));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
/*d*/	Packet.WriteD(va_arg(va, DWORD));
	va_end(va);
	pSocket->Send("b", Packet.GetSize(), Packet.GetBuff());
};