#include "StdAfx.h"
#include "Packets.h"

void CPacketFix::SendNpcInfo(CSocket *pSocket, const char *format, ...)
{
	CPacket Packet;
	va_list tag;
	va_start(tag, format);
/*c*/ Packet.WriteC(va_arg(tag, BYTE)); 
		UINT objectId = va_arg(tag, DWORD);
/*d*/ Packet.WriteD(objectId);
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*f*/ Packet.WriteF(va_arg(tag, double));
/*f*/ Packet.WriteF(va_arg(tag, double));
/*f*/ Packet.WriteF(va_arg(tag, double));
/*f*/ Packet.WriteF(va_arg(tag, double));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*c*/ Packet.WriteC(va_arg(tag, BYTE)); 
/*c*/ Packet.WriteC(va_arg(tag, BYTE)); 
/*c*/ Packet.WriteC(va_arg(tag, BYTE)); 
/*c*/ Packet.WriteC(va_arg(tag, BYTE)); 
/*c*/ Packet.WriteC(va_arg(tag, BYTE)); 
/*S*/ Packet.WriteS(va_arg(tag, PWCHAR));
/*S*/ Packet.WriteS(va_arg(tag, PWCHAR));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
		DWORD AbnormalVisualEffect = va_arg(tag, DWORD);
		if(CNPC *pNpc = CNPC::GetNPCByServerId(&objectId))
		{
			if(pNpc->IsSummon() || pNpc->IsPet())
			{
				User *pMaster = pNpc->CastSummon()->GetMaster();
				if(pMaster->ValidUser())
				{
					if(pMaster->pED->spawnProtection == 2)
					{
						AbnormalVisualEffect |= 0x200000;
					}
				}
			}
		}
/*d*/ Packet.WriteD(AbnormalVisualEffect);
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*c*/ Packet.WriteC(va_arg(tag, BYTE)); 
/*c*/ Packet.WriteC(va_arg(tag, BYTE)); 
/*f*/ Packet.WriteF(va_arg(tag, double)); 
/*f*/ Packet.WriteF(va_arg(tag, double)); 
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
	va_end(tag);
//Intelrude
	Packet.WriteD(0); //IsFlying

	pSocket->Send("b", Packet.GetSize(), Packet.GetBuff());
}

void CPacketFix::BCSendNpcInfo(INT64 dwAddr, int nObjID, int nRange, FVector *pLocation, const char *format, ...)
{
	CPacket Packet;
	va_list tag;
	va_start(tag, format);
/*c*/ Packet.WriteC(va_arg(tag, BYTE)); 
		UINT objectId = va_arg(tag, DWORD);
/*d*/ Packet.WriteD(objectId);
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*f*/ Packet.WriteF(va_arg(tag, double));
/*f*/ Packet.WriteF(va_arg(tag, double));
/*f*/ Packet.WriteF(va_arg(tag, double));
/*f*/ Packet.WriteF(va_arg(tag, double));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*c*/ Packet.WriteC(va_arg(tag, BYTE)); 
/*c*/ Packet.WriteC(va_arg(tag, BYTE)); 
/*c*/ Packet.WriteC(va_arg(tag, BYTE)); 
/*c*/ Packet.WriteC(va_arg(tag, BYTE)); 
/*c*/ Packet.WriteC(va_arg(tag, BYTE)); 
/*S*/ Packet.WriteS(va_arg(tag, PWCHAR));
/*S*/ Packet.WriteS(va_arg(tag, PWCHAR));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
	DWORD AbnormalVisualEffect = va_arg(tag, DWORD);
		if(CNPC *pNpc = CNPC::GetNPCByServerId(&objectId))
		{
			if(pNpc->IsSummon() || pNpc->IsPet())
			{
				User *pMaster = pNpc->CastSummon()->GetMaster();
				if(pMaster->ValidUser())
				{
					if(pMaster->pED->spawnProtection == 2)
					{
						AbnormalVisualEffect |= 0x200000;
					}
				}
			}
		}
/*d*/ Packet.WriteD(AbnormalVisualEffect);
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
/*c*/ Packet.WriteC(va_arg(tag, BYTE)); 
/*c*/ Packet.WriteC(va_arg(tag, BYTE)); 
/*f*/ Packet.WriteF(va_arg(tag, double)); 
/*f*/ Packet.WriteF(va_arg(tag, double)); 
/*d*/ Packet.WriteD(va_arg(tag, DWORD));
	va_end(tag);
//Intelrude
	Packet.WriteD(0); //IsFlying

	BroadcastToNeighborExceptSelf(nObjID, nRange, pLocation, (int)Packet.GetSize(), Packet.GetBuff());
}