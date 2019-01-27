#include "StdAfx.h"
#include "DB.h"

void CFreightSystem::Initialize()
{
	g_HookManager.WriteCall( 0x00817690, SendPackage, 0);
	g_HookManager.WriteCall( 0x00897AD5, CWareHouseWithdraw_SetNpcData, 0);
	g_HookManager.WriteCall( 0x008A9248, LoadWareHouseItems, 0);
}

void CFreightSystem::SendPackage(INT64 nCached, User *pUser, int nTargetDBID, int nAmount, int nBuffSize, PUCHAR Buff, int nNpcClassID)
{
	//g_Log.Add(CLog::Blue, "[%s] pUser[%S] Amount[%d] Target[%d] NpcClassID[%d]", __FUNCTION__, pUser->pSD->wszName, nAmount, nTargetDBID, nNpcClassID);
	nNpcClassID = 0x666;
	g_DB.RequestPackageSendToTarget(pUser, nTargetDBID, nAmount, nBuffSize, Buff, nNpcClassID);
}

void CFreightSystem::CWareHouseWithdraw_SetNpcData(CWareHouseWithdraw *pWH, CNPC *pNPC)
{
	if(pWH)
	{
		pWH->SetNpcData(pNPC);
		pWH->nNpcClassID = 0x666;
		pWH->nNpcClassID1 = 0x666;
	}
}

void CFreightSystem::LoadWareHouseItems(INT64 nCached, int nDBID, User *pUser, int nWhID, CWareHouseWithdraw *pWH, int nNpcClassID, int nMemoryID)
{
	nNpcClassID = 0x666;
	g_DB.RequestLoadWareHouseItems(nDBID, pUser, nWhID, pWH, nNpcClassID, nMemoryID);
}