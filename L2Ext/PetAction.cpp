#include "StdAfx.h"
#include "PetAction.h"
#include "CursedWeaponSystem.h"
#include "DB.h"

extern BOOL g_GetLootFromPet;
extern BOOL g_PetBlockGiveAllItemsToMaster;

void PetAction::Initialize()
{
	g_HookManager.WriteCall( 0x54C4DB, PetAction::PickUpItem, 0);
	g_HookManager.NopMemory(0x54C4E0, 12);
	g_HookManager.WriteCall( 0x54C5FE, PetAction::PickUpItem, 0);
	g_HookManager.NopMemory(0x54C603, 12);
	g_HookManager.WriteMemoryDWORD(0xA4B0F0, (DWORD)PetAction::TimerExpired);

	g_HookManager.WriteCall( 0x752E2D, PetAction::GetItemFromPet, 0);
	g_HookManager.WriteCall( 0x8729A3, PetAction::GetItemFromPet, 0);

	g_HookManager.WriteCall( 0x5727B3, PetAction::GiveItemToPet, 0);
	g_HookManager.WriteCall( 0x5727E6, PetAction::GiveItemToPet, 0);
	g_HookManager.WriteCall( 0x8726D9, PetAction::GiveItemToPet, 0);

	g_HookManager.WriteCall( 0x560A43, PetAction::LinkPet, 0);
	g_HookManager.WriteCall( 0x753674, PetAction::LinkPet, 0);
	g_HookManager.WriteCall( 0x753819, PetAction::LinkPet, 0);

	g_HookManager.WriteCall( 0x572778, PetAction::GiveAllItemToMaster, 0);
	g_HookManager.WriteCall( 0x816DEA, PetAction::GiveAllItemToMaster, 0);
	g_HookManager.WriteCall( 0x83503C, PetAction::GiveAllItemToMaster, 0);

	g_HookManager.WriteCall( 0x707A44, PetAction::DespawnPet, 0);
	g_HookManager.WriteCall( 0x7057C8, PetAction::DespawnPet, 0);
	g_HookManager.WriteCall( 0x8381C0, PetAction::DespawnPet, 0);
	g_HookManager.WriteCall( 0x850018, PetAction::DespawnPet, 0);

}

void PetAction::GiveAllItemToMaster(CPet *pPet)
{
	guard;

	if(g_PetBlockGiveAllItemsToMaster == FALSE)
	{
		if(User *pUser = pPet->GetMaster())
		{
			if(pUser->IsNowTrade())
			{
				pUser->TradeCancel();
			}
		}

		typedef void(*f)(CPet*);
		f(0x752980L)(pPet);
	}
	unguard;
}

bool PetAction::LinkPet(LPVOID lpInstance, User *pUser, CPet *pPet)
{
	guard;

	if(pUser->ValidUser())
	{
		if(pUser->IsNowTrade())
		{
			pUser->TradeCancel();
		}
		pUser->pSocket->pED->itemActionTimestamp = GetTickCount() + 3000;
	}
	typedef bool (*f)(LPVOID, User*, CPet*);
	bool ret = f(0x74C6B0L)(lpInstance, pUser, pPet);

	unguard;
	return ret;
}

void PetAction::DespawnPet(User *pUser, bool bByLeaveWorld)
{
	if(pUser->IsNowTrade())
		pUser->TradeCancel();

	pUser->pSocket->pED->itemActionTimestamp = GetTickCount() + 1000;

	pUser->DespawnPet(bByLeaveWorld);
}

void PetAction::GiveItemToPet(LPVOID lpInstance, User *pUser, CItem *pItem, int count)
{
	if(pUser->ValidUser())
	{
		if(pUser->IsNowTrade())
		{
			pUser->TradeCancel();
		}
		pUser->pSocket->pED->itemActionTimestamp = GetTickCount() + 1000;
	}
	typedef void(*f)(LPVOID, User*, CItem*, int);
	f(0x529E50L)(lpInstance, pUser, pItem, count);
}

void PetAction::GetItemFromPet(LPVOID lpInstance, User *pUser, CItem *pItem, int count)
{
	if(pUser->ValidUser())
	{
		if(pUser->IsNowTrade())
		{
			pUser->TradeCancel();
		}

		pUser->pSocket->pED->itemActionTimestamp = GetTickCount() + 1000;
	}
	typedef void(*f)(LPVOID, User*, CItem*, int);
	f(0x52A060L)(lpInstance, pUser, pItem, count);
}

void PetAction::PickUpItem(CInventory *pInventory, CItem *pItem, int nChangedType)
{
	pInventory->SetInventoryChanged(pItem, nChangedType);
	pInventory->WriteUnlock();

	CPet *pPet = pInventory->GetOwner()->CastPet();
	if(pPet->IsPet())
	{
		if(g_CursedWeaponSystem.IsCursedWeapon(pItem))
		{
			g_CursedWeaponSystem.OnPickUp(pPet, pItem);
		}else
		{
			if(g_GetLootFromPet)
			{
				if(User *pUser = pPet->GetMaster())
				{
					if(pUser->inventory.CheckAddable(pItem->pSID->nItemID, pItem->pSID->nItemAmount))
					{
						if(pUser->IsNowTrade())
						{
							pUser->TradeCancel();
						}

						GetItemFromPet(g_DB.GetOrgInstance(), pUser, pItem, pItem->pSID->nItemAmount);
					}else
					{
						pPet->DropItem(pItem, pItem->pSID->nItemAmount);
					}
				}
			}
		}
	}
}

void PetAction::TimerExpired(CPet *pPet, int id)
{
	typedef void (*f)(CPet*, int);
	f(0x752260)(pPet, id);
	
	if(pPet->IsPet())
	{
		User *pMaster = pPet->GetMaster();
		if(pMaster->ValidUser())
		{
			CItem *pCollar = pMaster->inventory.GetItemByDBID(pPet->pSD->nPetDBID);
			if(pCollar->IsValidItem())
			{
			}else
			{
				g_Logger.Add(L"[PetDupe] User[%s] haven't got a collar!", pMaster->pSD->wszName);
				g_Log.Add(CLog::Error, "[%s] Master[%S] haven't got a collar!- can be trying pet dupe!", __FUNCTION__, pMaster->pSD->wszName);
				pMaster->pSD->pExData->kickTick = 1;
			}
		}
	}
}