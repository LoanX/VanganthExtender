#pragma once


class NpcAction
{
public:
	static void Initialize();
	static bool OnDie(CNPC *pNPC, CCreature *pEnemy);
	static void EnterWorld(CNPC* pNPC, bool bSetDefaultParam, int nHP, int nMP);
	static bool OnNpcDieAddEventToInventory(CNPC *pNpc, int itemId, int count, bool isPick, int param2, int param3, int param4);
	static bool OnNpcDieAddAdenaToInventory(CNPC *pNpc, int itemId, int count, bool isPick, int param2, int param3, int param4);
	static bool OnNpcDieAddItemToInventory(CNPC *pNpc, int itemId, int count, bool isPick, int param2, int param3, int param4);
	static void LeaveWorld(CNPC *pNPC);
	static bool OnNpcClassChange(CCreature *pCreature, UINT classId);
	static INT32 OnGetClassIdFromNameInNpcCreatePacket(UINT64 instance, const WCHAR* wNpcName);
	static void OnTimerExpired(CNPC *pNpc, INT32 id);
};