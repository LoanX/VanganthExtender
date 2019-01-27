#include "StdAfx.h"
#include "DB.h"


bool CShadowItem::IsShadowItem(int nItemID)
{
	if(CItemDataEx *pData = g_ItemDBEx.GetItemDataEx(nItemID))
	{
		if(pData->GetShadowTime() > 0)
			return true;
	}
	return false;
}

void CShadowItem::EnterWorld(User *pUser)
{
	CreatureSharedData *pSD = pUser->pSD;
	if(pSD)
	{
		try
		{
			for(int n=0;n<18;n++)
			{
				int nIndex = pSD->nEquipment[n];
				if(nIndex > 0)
				{
					CItem *pItem = pUser->GetInventory()->GetItemByIndex(nIndex);
					if(pItem->IsValidItem() && IsShadowItem(pItem->pSID->nItemID))
					{
						int nTimerID = CCreatureTimerHandler::USER_SHADOW_ITEM_START - nIndex;
						TimerExpired(pUser, nTimerID);
					}
				}
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
		}
	}
}

void CShadowItem::EquipItem(User *pUser, CItem *pItem)
{
	if(pUser->ValidUser() && pItem->IsValidItem())
	{
		if(IsShadowItem(pItem->pSID->nItemID))
		{
			int nTimerID = CCreatureTimerHandler::USER_SHADOW_ITEM_START - pItem->pSID->nItemIndex;
			//g_Log.Add(CLog::Blue, "[%s] TimerID[%d] ItemIndex[%d]", __FUNCTION__, nTimerID, pItem->pSID->nItemIndex);
			TimerExpired(pUser, nTimerID);
		}
	}
}

void CShadowItem::TimerExpired(User *pUser, int nTimerID)
{
	if(nTimerID < CCreatureTimerHandler::USER_SHADOW_ITEM_START && pUser->ValidUser())
	{
		try
		{
			int nItemIndex = CCreatureTimerHandler::USER_SHADOW_ITEM_START - nTimerID;
			int nEquipedSlot = pUser->GetEquipedItemSlot(nItemIndex);
			if(nEquipedSlot != CItem::SlotType_None)
			{
				CItem *pItem = pUser->inventory.GetItemByIndex(nItemIndex);
				if(pItem->IsValidItem())
				{
					if(IsShadowItem(pItem->pSID->nItemID))
					{
						pItem->nManaLeft--;
						if(pItem->nManaLeft == (10*60))
						{
							CSystemMessage msg(SystemMessage::$S1_S_REMAINING_MANA_IS_NOW_10_);
							msg.AddItem(pItem->pSID->nItemID);
							pUser->SendSystemMessage(&msg);
						}else if(pItem->nManaLeft == (5*60))
						{
							CSystemMessage msg(SystemMessage::$S1_S_REMAINING_MANA_IS_NOW_5_);
							msg.AddItem(pItem->pSID->nItemID);
							pUser->SendSystemMessage(&msg);
						}else if(pItem->nManaLeft == (1*60))
						{
							CSystemMessage msg(SystemMessage::$S1_S_REMAINING_MANA_IS_NOW_1__IT_WILL_DISAPPEAR_SOON_);
							msg.AddItem(pItem->pSID->nItemID);
							pUser->SendSystemMessage(&msg);
						}else if(pItem->nManaLeft <= 0)
						{
							CSystemMessage msg(SystemMessage::$S1_S_REMAINING_MANA_IS_NOW_0__AND_THE_ITEM_HAS_DISAPPEARED_);
							msg.AddItem(pItem->pSID->nItemID);
							pUser->SendSystemMessage(&msg);
							if(pUser->IsNowTrade())
								pUser->TradeCancel();
							pUser->UnEquipItem(nEquipedSlot);
							g_DB.RequestDestroyItem(pItem, 1, pUser);
							return;
						}
						if(pItem->nManaLeft && (pItem->nManaLeft % 60) == 0 )
							pUser->GetInventory()->SetInventoryChanged(pItem, CInventory::UPDATE);
						//pUser->AddTimer(1200, nTimerID);
						pUser->pSD->pExData->TimerHandler.AddTimer(nTimerID, 1);
					}
				}
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
		}
	}
}