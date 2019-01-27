#pragma once

extern "C"
{
	void ChronoWeaponFix1();
	void ChronoWeaponFix2();
	void OldSpoilFix();
	void SpoilFix();
	void SpoilFixEx();
	int SpoilAmountWrapper(double min, double max, double baseChance, double dice);
	void CheckOfflineShopWhenExit();
	void CMultiPartyCommandChannelManager_WithdrawInternal1();
	void CursedWeaponKarmaFix1();
};

enum CLIENT_PACKET
{
	PROTOCOL_VERSION = 0x00,
	TRADE_REQUEST = 0x15,
	ANSWER_TRADE_REQUEST = 0x44,
	ICON_ACTION = 0x45,
	REQUEST_START_PLEDGE_WAR = 0x4D,
	REQUEST_STOP_PLEDGE_WAR = 0x4F,
	REQEST_ACQUIRE_SKILL_INFO = 0x6B,
	PRIVATE_STORE_SELL_LIST_SET = 0x74,
	PRIVATE_STORE_BUY_LIST_SET = 0x91,
	NET_PING = 0xA8,
	REQUEST_RECIPE_SHOP_SET_LIST = 0xB2
};
enum OUTGOING_PACKET_OP_CODE
{
	CRYPT_INIT = 0x00,
	MOVE_TO_LOCATION = 0x01,
	NPC_SAY = 0x02,
	CHAR_INFO_PCK = 0x03,
	USER_INFO_PCK = 0x04,
	ATTACK = 0x05,
	DIE = 0x06,
	REVIVE = 0x07,
	ATTACK_OUT_OF_RANGE = 0x08,
	ATTACK_IN_COOL_TIME = 0x09,
	ATTACK_DEAD_TARGET = 0x0A,
	SPAWN_ITEM = 0x0B,
	DROP_ITEM = 0x0C,
	GET_ITEM = 0x0D,
	STATUS_UPDATE = 0x0E,
	NPC_HTML_MESSAGE = 0x0F,
	SELL_LIST = 0x10,
	BUY_LIST = 0x11,
	DELETE_OBJECT = 0x12,
	CHARACTER_SELECTION_INFO = 0x13,
	LOGIN_FAIL = 0x14,
	CHARACTER_SELECTED = 0x15,
	NPC_INFO = 0x16,
	NEW_CHARACTER_SUCCESS = 0x17,
	NEW_CHARACTER_FAIL = 0x18,
	CHARACTER_CREATE_SUCCESS = 0x19,
	ON_CHARACTER_CREATE_FAIL = 0x1A,
	ITEM_LIST = 0x1B,
	SUN_RISE = 0x1C,
	SUN_SET = 0x1D,
	SKILL_LIST = 0x58,
	SYSTEM_MESSAGE = 0x64,
	LOGOUT_OK = 0x7E,
	SKILL_COOL_TIME = 0xC1,
	EX_PACKET = 0xFE
};
enum EX_PACKET_OUT
{
	DUEL_ASK = 0x4B,
	DUEL_READY = 0x4C,
	DUEL_START = 0x4D,
	DUEL_END = 0x4E
};

void BroadcastToNeighborExceptSelf(int objectID, int range, FVector* pPos, int buffLen, PCHAR buff);
BOOL AppendToFile(HANDLE hFile, const wchar_t* format, ...);

class L2Server
{
public:
	static void BroadcastToAllUser(size_t len, PCHAR buff);
	static void BroadcastToNeighborDist(INT32 ObjectID, FVector *vPos, int Distance, int nSize, PCHAR buff);
	static CObject *GetObjectByID(UINT nObjectID);
	static int Rnd(int n);
	static int GetNoblessIndexByDBID(int nDBID);
	static LPVOID New(size_t nSize);
	static void Delete(LPVOID pPointer);
	static bool RequestInvite(int nType, User *pAsker, User *pAsked, int nTimeout);
	static User *GetInviter(CObjectSP *pObjectSP, User *pUser);
	static void ResetInvite(User *pUser);
	static long int GetL2Time();
	static BYTE IsValidPointer(LPVOID pPointer);
	static void CallStack(int nType);
	static void TimerDispatchFix(PINT64 pPointer, int nVar);
	static void SetResidenceOwner(CPledge *pPledge, UINT residenceId);
	static void SetResidenceBattleTime(UINT residenceId, UINT startTime = 15);
};

void EnableNpcPosEvent(const WCHAR* wName);
void DisableNpcPosEvent(const WCHAR* wName);
void SpawnNpc(UINT npcClassId, vector<pair<wstring, wstring>> aiParams, int x, int y, int z, UINT heading);