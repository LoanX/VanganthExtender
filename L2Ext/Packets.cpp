#include "StdAfx.h"
#include "Packets.h"

User* g_CharInfoUser[16];

void CPacketFix::Initialize()
{
	for(UINT n=0;n<16;n++)
	{
		g_CharInfoUser[n] = 0;
	}
	g_HookManager.WriteCall( 0x58B9F3, LoadCharSelectedPacket, 0);
	g_HookManager.WriteJump( 0x574B48, LoadCharacterDiss, 0);
	g_HookManager.WriteCall( 0x574C31, CharSelectInfoFix, 0);
	//Sell bug fix by TestDude
	g_HookManager.WriteMemoryBYTE(0x87A4EA, 0x44);
	//Buy item bug by me
	g_HookManager.WriteMemoryBYTE(0x87D80D, 0x44);
	//BuyPreviewPacket fix by me ;P
	g_HookManager.WriteMemoryBYTE(0x87C545, 0x44);

	g_HookManager.WriteCall( 0x8AA5A8, WarehouseDepositListSend, 0);
	g_HookManager.WriteCall( 0x8AAFBA, WarehouseDepositListSend, 0); //Ex
	g_HookManager.WriteCall( 0x54EF19, WarehouseWithdrawList, 0);
	g_HookManager.WriteCall( 0x54EFD9, WarehouseWithdrawList, 0);
	g_HookManager.WriteCall( 0x563F44, WarehouseWithdrawListEx, 0);//Ex
	g_HookManager.WriteCall( 0x564006, WarehouseWithdrawListEx, 0);//Ex
	g_HookManager.WriteCall( 0x57B996, WarehouseWithdrawListEx, 0);//Reply
	//Warehouse Deposit List
	g_HookManager.WriteCall( 0x8AA10B, WarehouseDepositListFix, 0);
	g_HookManager.WriteCall( 0x8AA202, WarehouseDepositListFix, 0);
	g_HookManager.WriteCall( 0x8AAD0E, WarehouseDepositListExFix, 0);//ex
	g_HookManager.WriteCall( 0x8AADD8, WarehouseDepositListExFix, 0); //ex
	g_HookManager.WriteCall( 0x821409, PackageSendableList, 0);

	g_HookManager.WriteCall( 0x74781E, SendSummonStatusFix, 0);
	g_HookManager.WriteCall( 0x75125E, SendSummonInfo, 0);
	g_HookManager.WriteCall( 0x6C27E1, SendNpcInfo, 0);
	g_HookManager.WriteCall( 0x6C2219, SendNpcInfo, 0);
	g_HookManager.WriteCall( 0x8590EC, SendNpcInfo, 0);
	g_HookManager.WriteCall( 0x859C37, BCSendNpcInfo, 0);

	g_HookManager.WriteCall( 0x7922C9, CPacketFix::AssembleShortCutInfo, 0);
	g_HookManager.WriteCall( 0x792303, CPacketFix::AssembleShortCutInfo, 0);
	g_HookManager.WriteCall( 0x79233D, CPacketFix::AssembleShortCutInfo, 0);
	g_HookManager.WriteCall( 0x7924D7, CPacketFix::AssembleShortCutInfo, 0);
	g_HookManager.WriteCall( 0x792529, CPacketFix::AssembleShortCutInfo, 0);

	g_HookManager.WriteCall( 0x792000, CPacketFix::AddShortCut, 0);
	g_HookManager.WriteCall( 0x791FD1, CPacketFix::AddShortCut, 0);
	g_HookManager.WriteCall( 0x791F9D, CPacketFix::AddShortCut, 0);
	g_HookManager.WriteCall( 0x791F6B, CPacketFix::AddShortCut, 0);
	g_HookManager.WriteCall( 0x791F39, CPacketFix::AddShortCut, 0);
	
	//Multisell
	g_HookManager.WriteCall( 0x6919A2, MultisellIngredientsFix, 0);
	g_HookManager.WriteCall( 0x6918C5, MultisellIngredientsFix, 0);
	g_HookManager.WriteCall( 0x691CD8, MultisellProductsFix, 0);
	g_HookManager.WriteCall( 0x691DC2, MultisellFix, 0);
	g_HookManager.WriteCall( 0x8286CE, SkillListFix, 0);
	g_HookManager.WriteCall( 0x82882B, SkillListFix, 0);
	g_HookManager.WriteCall( 0x82891E, SkillListFix, 0);

	g_HookManager.WriteCall( 0x858E12, CharInfoFix, 0);
	g_HookManager.WriteCall( 0x85990B, CharInfoBCFix, 0);
	g_HookManager.WriteMemoryDWORD(0xA878E8, (DWORD)CharInfoSetUser);

	g_HookManager.WriteCall( 0x85768E, UserInfoFix, 0);
	g_HookManager.WriteCall( 0x856BEF, GMUserInfoFix, 0);

	//Asm
	//User	
	g_HookManager.WriteJump( 0x81AE31, ItemListHook1, 0);
	g_HookManager.WriteJump( 0x81AD74, ItemListHook2, 0);
	g_HookManager.WriteJump( 0x81AEBA, ItemListHook3, 0);
//CPET
	//g_HookManager.WriteJump( 0x74FB9A, ItemListHook4, 0);
	//g_HookManager.WriteJump( 0x74FC52, ItemListHook5, 0);
	//g_HookManager.WriteJump( 0x74FCD2, ItemListHook6, 0);
//User	
	g_HookManager.WriteJump( 0x81A45C, InventoryUpdateHook1, 0);
	g_HookManager.WriteJump( 0x81A610, InventoryUpdateHook2, 0);
//CPET
	//g_HookManager.WriteJump( 0x74F246, InventoryUpdateHook3, 0); //??
	//g_HookManager.WriteJump( 0x74F3F4, InventoryUpdateHook4, 0); //??


	//MyTargetSelected
	g_HookManager.WriteCall(0x4EEEAF, MyTargetSelected, 0);

	//DoorInfo
	g_HookManager.WriteCall(0x7FEF32, SendDoorInfo, 0);
	g_HookManager.WriteCall(0x776A63, SendDoorInfo, 0);
}

void CPacketFix::CharInfoSetUser(User *pUser, CUserSocket *pGMSocket)
{
	UINT index = GetThreadIndex();
	if(index < 16)
	{
		g_CharInfoUser[index] = pUser;
	}

	typedef void(*f)(User*, CUserSocket*);
	f(0x857AC0L)(pUser, pGMSocket);

	if(index < 16)
	{
		g_CharInfoUser[index] = 0;
	}

}

int CPacketFix::AssembleShortCutInfo(PCHAR lpBuff, int len, PCSTR format, ...)
{
	va_list vl;
	va_start(vl, format);
	INT32 Type = va_arg(vl, INT32);	
	INT32 slot  = va_arg(vl, INT32);
	INT32 id    = va_arg(vl, INT32);
	INT32 param = 0, level = 0;
	INT32 ret = 0;
	switch(Type)
	{
		case ShortCutSkill:
			{
				level = va_arg(vl, INT32);
				param = va_arg(vl, INT32);
				ret = Assemble(lpBuff, len, "ddddcd", Type, slot, id, level,/* skill disabled */ 0, param );
				break;
			}
		case ShortCutItem:
			{
				INT32 param = va_arg(vl, INT32);
				Augmentation augmentation;
				int reuseGroup = -1;
				int reuse = 0;

				CItem *pItem = (CItem*)CObject::GetObjectBySID(id);
				if(pItem->IsValidItem())
				{
					augmentation.Full = pItem->nAugmentationID;
					CItemDataEx *pEx = g_ItemDBEx.GetItemDataEx(pItem->pSID->nItemID);
					if(pEx)
					{
						reuseGroup = pEx->GetReuseGroup();
					}
				}

				ret = Assemble(lpBuff, len, "dddddddhh", Type, slot, id, param, reuseGroup, reuse, 0, augmentation.Part.effectA, augmentation.Part.effectB);
				break;
			}
		case ShortCutAction:
		case ShortCutMacro:
		case ShortCutRecipe:
		default:
			INT32 param = va_arg(vl, INT32);
			ret = Assemble(lpBuff, len, "dddd", Type, slot, id, param);
			break;
	}

	return ret;
}

void CPacketFix::AddShortCut(CUserSocket *pSocket, PCSTR format, ...)
{
	va_list vl;
	va_start(vl, format);
	BYTE opCode = va_arg(vl, BYTE);
	INT32 type = va_arg(vl, INT32);
	INT32 slot = va_arg(vl, INT32);
	INT32 id = va_arg(vl, INT32);
	INT32 param = 0, level = 0;
	switch(type)
	{
		case ShortCutSkill:
				level = va_arg(vl, INT32);
				param = va_arg(vl, INT32);
				pSocket->Send("cddddcd", opCode, type, slot, id, level, /* c5 */ 0, param);
				break;
		case ShortCutItem:	
		case ShortCutAction:
		case ShortCutMacro:
		case ShortCutRecipe:
		default:
			INT32 param = va_arg(vl, INT32);
			pSocket->Send("cdddd", opCode, type, slot, id, param);
			break;

	}
	
	va_end(vl);
}