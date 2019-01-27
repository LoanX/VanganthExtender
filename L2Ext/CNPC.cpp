#include "StdAfx.h"
#include "CNpc.h"


void CNPC::TeleportToLocation(int x, int y, int z, int param0)
{
	typedef void(*f)(CNPC*, int, int, int, int);
	f(0x6C1060L)(this, x, y, z, param0);
}

void CNPC::TimerExpired(int id)
{
	typedef void(__fastcall *__TE)(CNPC*, int id);
	__TE _TE = (__TE)0x006C5E60;
	_TE(this, id);
}

void CNPC::KillNPC()
{
	if(this)
	{
		npcKilledByGM = 1;
		if(pContributeData)
		{
			pContributeData->Clear();
		}
		Die(0);
	}
}



CNPC* CNPC::GetNPCByServerId(LPUINT pSID)
{
	typedef CNPC*(*f)(LPUINT);
	return f(0x7F05D0L)(pSID);
}