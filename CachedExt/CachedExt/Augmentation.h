#pragma once

class CAugmentation
{
public:
	static void Init();
	static bool WareHouseDepositItem(CItem *pNewItem, CItem *pItem, int param);
	static int AssembleItemPacket(PCHAR Buff, int Len, const PCHAR format, int ItemDBID, int ItemType, int Amount, int Enchant, int Eroded, int Bless, int Ident, int Wished);
};