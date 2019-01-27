#pragma once


class PlayerAction
{
public:
	static void Initialize();
	static bool OnEnterWorld(User *pUser);
	static void OnLeaveWorld(User *pUser);
	static bool OnDie(User *pUser, CCreature *pEnemy);
	static void OnPickUpItem(CInventory *pInventory, CItem *pItem, int nChangedType);
	static void OnAddItemToInventory(CInventory *pInventory, CItem *pItem, int nType);
	static bool UseItem(User *pUser, CItem *pItem, int nForce);
	static bool UnequipItem(User *pUser, int nSlotType);
	static bool RegisterInOlympiad(COlympiadUser *pOU);
	static void OnChangeSubjobEnd(User *pUser, bool bCreate);
	static bool OnDropItem(User *pUser, CItem *pItem, int nAmount, FVector *pPos);
	static int OnGetRelationTo(User *pUser, User *pAttacker);
	static void OnOlympiadEscaping(User *pUser);
	static bool OnEnteringOlympiadStadion(COlympiadField *pOlympiadField);
	static CParty *OnUpdateHP(User *pUser);
	static void OnSendETCBuffStatusInfo(CSocket *pSocket,const char *format, ...);
	static bool OnOlympiadStartMatch(COlympiadField *pField);
	static bool OnTeleportToLocation(User *pUser, int nX, int nY, int nZ, int FadeScreen);
	static bool OnEquipItem(User *pUser, CItem *pItem, int nForce);
	static int OnDieLooseExp(User *pUser, int nLostExp);
	static void OnDieDropItem(User *pUser, bool bOnBattlefield);
	static void IncreaseKarma(User *pUser, bool bWithPk);
	static void OnOlympiadFinishMatch(COlympiadField *pOF);
	static void OnTimerExpired(User *pUser, int nTimerID);
	static void SaveCharacterInfo(INT64 nCDB, const char *format, ...);
	static void LoadItemsPacket(LPVOID pCDB, User *pUser);
	static void ObserverStart(User *pUser, int x, int y, int z, int yaw, int pitch, int period);
	static void ObserverEnd(User *pUser);
	static void OnChangeSubjobBegin(User *pUser);
	static void OnSendXPnSP(CUserSocket *pSocket, const char* format, BYTE opCode, DWORD MsgID, DWORD NumOfVars, DWORD Type1, int Exp, DWORD Type2, int SP);
	static bool OnClassChange(User *pUser, int nClass);
	static bool OnHear(User* pUser, User* pTalker, PWCHAR wTalkerName, const WCHAR* wMessage, int nType);
	static bool OnDeleteItemInInventoryBeforeCommit(User *pUser, int itemId, int count);
	static bool DeleteShipTicket(User *pUser, INT32 ticketId, INT32 count);
	static bool COlympiadEntry_RemoveEntry(LPVOID pInstance, UINT userId);
	static bool COlympiadEntry_AddEntry(LPVOID pInstance, UINT userId, bool bClassFree);
	static void TradeCancelled(CTrade *pTrade, User *pTrader);
	static void SummonPet(User* pUser, CItem *pPetCollar);
	static void SendAbnormalStatusInfo(User *pUser);
	static void ShowHTML(User *pUser, const WCHAR* wName, const WCHAR* wHtml, UINT itemClassId);
	static void ShowCommunityBoard(CUserSocket *pSocket, const char* format, BYTE opCode, BYTE param1, const WCHAR* wBypass2, const WCHAR* wBypass3, const WCHAR* wBypass4, const WCHAR* wBypass5, const WCHAR* wBypass6, const WCHAR* wBypass7, const WCHAR* wBypass8, const WCHAR* wBypass9, const WCHAR* wHtml);
	static void NpcHtmlSender(CUserSocket *pSocket, const char* format, BYTE opCode, UINT talkTargetId, const WCHAR* wHtml);
	static void ShowTutorialPage(CUserSocket *pSOcket, const char* format, BYTE opCode, const WCHAR* wHtml);
	static void OnRestartSendSaveCharacterInfo(LPVOID lpInstance, User *pUser, UINT param1, bool param2);
	static void OnTakeWithdrawPenalty(User *pUser);
	static void DBIncreaseUserDuelCount(LPVOID pInstance, UINT charId);
	static void CUserSocket_TimerExpired(CUserSocket *pSocket, UINT id);
	static void OnAction(User *pUser, CObject *pTarget, bool force, bool notMove);
	static void OnBroadcastCombatModeStart(User *pUser);
	static void OnDBRequestRenewSubjob(PVOID pInstance, User *pUser, UINT subjobId, UINT newClass, bool byBuilder, int npcIndex, int userIndex);
	static bool OnValidateCpRegen(User *pUser);
	static void OnSendSkillAcquireList(User *pUser, UINT type);
	static void OnRecipeMakeItem(PVOID instancePtr, User *pUser, UINT recipeId, UINT result, UINT ingrCount, UINT buffLen, PCHAR buffer);
};