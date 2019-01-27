#pragma once


class CFreightSystem
{
public:
	static void Initialize();
	static void SendPackage(INT64 nCached, User *pUser, int nTargetDBID, int nAmount, int nBuffSize, PUCHAR Buff, int nNpcClassID);
	static void CWareHouseWithdraw_SetNpcData(CWareHouseWithdraw *pWH, CNPC *pNPC);
	static void LoadWareHouseItems(INT64 nCached, int nDBID, User *pUser, int nWhID, CWareHouseWithdraw *pWH, int nNpcClassID, int nMemoryID);
};