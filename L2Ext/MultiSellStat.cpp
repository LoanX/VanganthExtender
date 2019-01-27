#include "stdafx.h"
#include "MultiSellStat.h"
#include "MultiSell.h"
#include "DB.h"

CMultiSellStat g_MultiSellStat;
MultiSellInfo g_MultiSellInfo[32];

CMultiSellStat::CMultiSellStat() : m_enabled(FALSE)
{
}

CMultiSellStat::~CMultiSellStat()
{

}

void CMultiSellStat::Init()
{
	m_enabled = FALSE;
	memset(g_MultiSellInfo, 0, sizeof(g_MultiSellInfo));

	LoadINI();
	if(m_enabled)
	{
		g_Log.Add(CLog::Blue, "MultiSellStat feature is enabled");
		g_HookManager.WriteCall(0x84A400, CMultiSellStat::OnCreateUpdateItems, 0 );
	}
}

void CMultiSellStat::LoadINI()
{
	const TCHAR* sectionName = _T("MultiSellStat");
	m_enabled = GetPrivateProfileInt(sectionName, _T("Enabled"), 0, g_ConfigFile);
}

void CMultiSellStat::SetInfo(UINT listId, UINT entryId, UINT count)
{
	guard;
	if(m_enabled)
	{
		UINT index = GetThreadIndex();
		if(index < 32)
		{
			g_MultiSellInfo[index].count = count;
			g_MultiSellInfo[index].entryId = entryId;
			g_MultiSellInfo[index].listId = listId;
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Invalid thread index[%d]!", __FUNCTION__, index);
		}
	}
	unguard;
}

void CMultiSellStat::OnCreateUpdateItems(LPVOID lpInstance, User *pUser, int n, int buffSize, PCHAR lpBuff, LPVOID lpAtomicJob)
{
	guard;

	typedef void(*f)(LPVOID, User*, int, int, PCHAR, LPVOID);
	f(0x538BB0L)(lpInstance, pUser, n, buffSize, lpBuff, lpAtomicJob);

	if(g_MultiSellStat.IsEnabled())
	{
		UINT index = GetThreadIndex();
		if(index < 32)
		{
			INT32 priceId[5];
			INT32 priceCount[5];
			INT32 rewardId[5];
			INT32 rewardCount[5];
			memset(priceId, 0, sizeof(priceId));
			memset(priceCount, 0, sizeof(priceCount));
			memset(rewardId, 0, sizeof(rewardId));
			memset(rewardCount, 0, sizeof(rewardCount));

			if(g_MultiSellDB.GetPriceAndReward(g_MultiSellInfo[index].listId, g_MultiSellInfo[index].entryId, g_MultiSellInfo[index].count, priceId, priceCount, rewardId, rewardCount))
			{
				g_DB.RequestSaveMultiSellLog(pUser->nDBID, pUser->pSocket->accountId, pUser->pSD->wszName, pUser->pSocket->accountName, priceId[0], priceCount[0], priceId[1], priceCount[1], priceId[2], priceCount[2], priceId[3], priceCount[3], priceId[4], priceCount[4],
					rewardId[0], rewardCount[0], rewardId[1], rewardCount[1], rewardId[2], rewardCount[2], rewardId[3], rewardCount[3], rewardId[4], rewardCount[4], time(0));
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Failed!", __FUNCTION__);
			}
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Invalid thread index[%d]!", __FUNCTION__, index);
		}
	}

	unguard;
}