#pragma once

extern "C"
{
	void ItemListHook1();
	void ItemListHook2();
	void ItemListHook3();
	void ItemListHook4();
	void ItemListHook5();
	void ItemListHook6();
	int ItemListFix(PCHAR buf, int bufLen, CItem *pItem, WORD ItemType, DWORD ObjectID, DWORD ItemID, DWORD Count, WORD ItemType2, WORD CustomType1, WORD IsEquiped, DWORD BodyPart, WORD EnchantLevel, WORD CustomType2);
	void InventoryUpdateHook1();
	void InventoryUpdateHook2();
	void InventoryUpdateHook3();
	void InventoryUpdateHook4();
	void LoadCharacterDiss();
	int InventoryUpdateFix(PCHAR buf, int bufLen, CItem *pItem,	WORD UpdateType, WORD ItemType, DWORD ObjectID, DWORD ItemID, DWORD Count, WORD ItemType2, WORD CustomType1, WORD IsEquiped, DWORD BodyPart, WORD EnchantLevel, WORD CustomType2);
}

class CPacketFix
{
public:
	static void Initialize();
	static void WarehouseDepositListSend(CSocket *pSocket, const char *format, ...);
	static int WarehouseWithdrawList(PCHAR buff, int bufLen, const char *format, ...);
	static int WarehouseWithdrawListEx(PCHAR buff, int bufLen, const char *format, ...);
	static int WarehouseDepositListFix(PCHAR buff, int bufLen, const char *format, ...);
	static int WarehouseDepositListExFix(PCHAR buff, int bufLen, const char *format, ...);
	static void SendSummonStatusFix(CSocket *pSocket, const char *format, ...);
	static void SendSummonInfo(CSocket *pSocket, const char *format, ...);
	static void MyTargetSelected(CSocket *pSocket, const char* format, ...);
	static void SendNpcInfo(CSocket *pSocket, const char *format, ...);
	static void BCSendNpcInfo(INT64 dwAddr, int nObjID, int nRange, FVector *pLocation, const char *format, ...);
	static int AssembleShortCutInfo(PCHAR lpBuff, int len, PCSTR format, ...);
	static void AddShortCut(CUserSocket *pSocket, PCSTR format, ...);
	static int SkillListFix(PCHAR buf, int bufLen, const char *format, ...);
	static void CharInfoSetUser(User *pUser, CUserSocket *pGMSocket);
	static void CharInfoFix(CUserSocket *pSocket, const char *format, ...);
	static void CharInfoBCFix(INT64 dwAddr, int nObjID, int nRange, FVector *pLocation, const char *format, ...);
	static int MultisellIngredientsFix(PCHAR buff, int bufLen, const char *format, ...);
	static int MultisellProductsFix(PCHAR buff, int bufLen, const char *format, ...);
	static int MultisellFix(PCHAR buff, int bufLen, const char *format, ...);
	static void UserInfoFix(CUserSocket *pSocket, const char *format, ...);
	static void GMUserInfoFix(CUserSocket *pSocket, const char *format, ...);
	static void LoadCharSelectedPacket(CUserSocket *pSocket, const char *format, ...);
	static void CharSelectInfoFix(CUserSocket *pSocket, const char *szFormat, BYTE bOpCode, DWORD dwCharNum, DWORD dwPacketSize, PUCHAR pOldPacket);
	static int PackageSendableList(PCHAR buff, int nBuffLen, const char *format, ...);
	static void SendDoorInfo(CDoor* pDoor, CUserSocket* pSocket);
};
void StaticObjectA(CSocket *pSocket, const char *format, ...);
void StaticObjectB(LPVOID pClass, int nObjectID, int nRange, FVector vPos, int Distance, char *pFormat, ...);