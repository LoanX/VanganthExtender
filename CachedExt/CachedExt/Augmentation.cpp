#include "CachedExt.h"
#include "Augmentation.h"

void CAugmentation::Init()
{
	CachedExt::SetHook(0xE8, 0x4B85D5, (DWORD)CAugmentation::AssembleItemPacket, 0);	//deposit
	CachedExt::SetHook(0xE8, 0x4BDE46, (DWORD)CAugmentation::AssembleItemPacket, 0);	//withdraw
}

int CAugmentation::AssembleItemPacket(PCHAR Buff, int Len, const PCHAR format, int ItemDBID, int ItemType, int Amount, int Enchant, int Eroded, int Bless, int Ident, int Wished)
{
	UINT Augmentation = 0;
	UINT protectionTimeout = 0;
	DBConn *pDBConn = new DBConn;
	pDBConn->Bind(&Augmentation);
	pDBConn->Bind(&protectionTimeout);
	if(pDBConn->Execute(L"SELECT [augmentation], [protection_timeout] FROM [user_item] where [item_id] = %d", ItemDBID))
	{
		pDBConn->Fetch();
	};
	delete pDBConn;
	//g_Log.AddU(CLog::Blue, L"[LoadItems] AugmentationID[%d] ItemID[%d]", Augmentation, ItemDBID);
	return Assemble(Buff, Len, "dddddddddd", ItemDBID, ItemType, Amount, Enchant, Eroded, Bless, Ident, Wished, Augmentation, protectionTimeout);
}