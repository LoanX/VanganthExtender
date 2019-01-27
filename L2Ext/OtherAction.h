#pragma once

class CDBSocket;

class COtherAction
{
public:
	static void Initialize();
	static void CSkillAction2OnTick(CSkillAction *pAction);
	static void CSkillAction2AddTimer(CSkillAction *pAction, int Tick);
	static HWND CreateWindowExWHook(DWORD dwExStyle,LPCWSTR lpClassName,LPCWSTR lpWindowName,DWORD dwStyle,int X,int Y,int nWidth,int nHeight,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,LPVOID lpParam);
	static bool TimeAttackBoard_GiveReward(LPVOID pInstance, User *pUser, int nRoomNo, LPVOID pArray);
	static bool CGeo_InSiegeZone(LPVOID pGeo, int x, int y, int z, int zero);
	static double CBattleCamp_AttackedBy(CBattleCamp* pBattleCamp, CCreature* pEnemy, double Damage);
	static bool CPartyMatchingRequest_JoinRoom(LPVOID pInstance, CUserSP* pUserSP, int roomId, int locationFilter);
	static void CPartyRoomInfo_SetMaster(CPartyRoomInfo *pInfo, CUserSP* pUserSP);
	static void CCastle_OnSiegeEnd(CCastle *pCastle);
	static void AmpedDeleteItem(LPVOID pInstance, CItem *pItem);
	static void CParty_GetAllMemberWhoCanGetExpSp(LPVOID lpContainer, User** lpUser);
	static void CContributeData_Distribute(CContributeData *pData, CCreature *pCreature);
	static void CSkillAction2OnEnd(CSkillAction* pAction, bool finishedWell);
	static void CDBSocket_SendV(CDBSocket *pSocket, const char* format, va_list va);
	static void ObjectDB_OnRegisterObject(PVOID pInstance, CObject *pObj);
	static void OnAuthSocketInitialize(PVOID pSocket);
	static void OnShutdownStart(PVOID pInstance, UINT timestamp, INT32 id);
};

extern "C"
{
	void OnLoadItemHook();
	CItem* OnLoadItem(const unsigned char* packet, const char *format, int *pItemDBID, int *pItemType, int *pAmount, int *pEnchant, int *pEroded, int *pBless, int *pIdent, int *pWished);
	void OnSaveItemHook();
	int OnSaveItem(PCHAR packet, int nSize, CItem *pItem, int nItemDBID, int nCharDBID, int nItemType, int nAmount, int nEnchant, int nEroded, int nBless, int nIdent, int nWished, int nWarehouse);
}