#include "stdafx.h"
#include "ItemDelivery.h"
#include "DB.h"

CItemDelivery g_ItemDelivery;

CItemDelivery::CItemDelivery() : m_enabled(FALSE), m_checkInterval(60)
{
}

CItemDelivery::~CItemDelivery()
{
}

void CItemDelivery::Init()
{
	m_enabled = FALSE;
	m_checkInterval = 60;
	LoadINI();
	if(m_enabled)
	{
		g_Log.Add(CLog::Blue, "Item Delivery system is enabled.");
	}
}

void CItemDelivery::LoadINI()
{
	const TCHAR* sectionName = _T("ItemDelivery");
	m_enabled = GetPrivateProfileInt(sectionName, _T("Enabled"), 0, g_ConfigFile);
	m_checkInterval = GetPrivateProfileInt(sectionName, _T("CheckInterval"), 60, g_ConfigFile);
}

void CItemDelivery::OnTimerExpired(User *pUser)
{
	guard;
	if(m_enabled)
	{
		if(pUser->pED->itemDeliveryTimestamp < time(0))
		{
			pUser->pED->itemDeliveryTimestamp = time(0) + m_checkInterval;
			g_DB.RequestItemDeliveryAsk(pUser->nDBID);
		}
	}
	unguard;
}

void CItemDelivery::HandleRequest(User *pUser, UINT jobId, INT32 itemId, INT32 amount, INT32 enchant)
{
	guard;

	if(pUser->ValidUser())
	{
		if(pUser->inventory.CheckAddable(itemId, amount) && pUser->IsNowTrade() == false && pUser->pSD->nStoreMode == 0)
		{
			pUser->AddItemToInventory(itemId, amount, false, enchant);
			pUser->PlaySound(L"ItemSound.quest_finish", 1, pUser->nObjectID, pUser->pSD->Pos.x, pUser->pSD->Pos.y, pUser->pSD->Pos.z);
			pUser->SendSystemMessage(L"You have just received item(s) through Item Delivery System.");
			g_Logger.Add(L"Item delivery to user[%s] job[%d] item[%d][%d] enchant[%d]", pUser->pSD->wszName, jobId, itemId, amount, enchant);
			g_DB.RequestItemDeliveryDone(pUser->nDBID, jobId);
		}
	}

	unguard;
}