#include "StdAfx.h"
#include "TradeManager.h"

CTradeManager g_TradeManager;

User* CTrade::GetTrader(UINT index)
{
	if(index < 2)
	{
		CUserSP userSp;
		typedef User* (*f)(INT64, CUserSP*, int ObjectID);
		f(0x479090)(0x0E4471A0, &userSp, traderId[index]);
		if(User *pUser = userSp.get()->SafeCastUser())
		{
			return pUser;
		}
		else
		{
			g_Log.Add(CLog::Error, "[%s] Invalid user[%d]", __FUNCTION__, index);
		}
	}

	return 0;
}

void CTrade::Lock()
{
	guard;
	if(this)
	{
		LPCIOCriticalSection lpLock = (LPCIOCriticalSection)((UINT64)this + 0x4080);
		lpLock->Lock(__FILEW__, __LINE__);
	}
	unguard;
}

void CTrade::Unlock()
{
	guard;
	if(this)
	{
		LPCIOCriticalSection lpLock = (LPCIOCriticalSection)((UINT64)this + 0x4080);
		lpLock->Unlock();
	}
	unguard;
}

bool CTrade::CanAdd(INT32 itemId, UINT userId)
{
	guard;
	bool ret = true;

	UINT index = 2;
	if(userId == traderId[0])
	{
		index = 1;
	}else if(userId == traderId[1])
	{
		index = 0;
	}
	if(index < 2)
	{
		Lock();
		vector<UINT>& vItems = items[index];
		for(UINT n=0;n<vItems.size();n++)
		{
			if(CSharedItemData *pSID = CSharedItemData::FromIndex(vItems[n]))
			{
				if(pSID->nItemID == itemId)
				{
					ret = false;
					break;
				}
			}
		}
		Unlock();
	}
	unguard;
	return ret;
}

CTradeManager::CTradeManager(void)
{
	pInstance = (CTradeManager*)0x0E447140;
}

CTradeManager::~CTradeManager(void)
{
}

CTrade* CTradeManager::GetTradeSP(CTradeSP *pTradeSP, UINT tradeServerId)
{
	typedef CTrade* (*f)(CTradeManager*, CTradeSP*, int);
	return f(0x7F36C0)(pInstance, pTradeSP, tradeServerId);
}

User* CTradeManager::GetTrader(UINT tradeServerId, UINT traderIndex)
{
	if(traderIndex < 2 && tradeServerId)
	{
		CTradeSP tradeSp;
		GetTradeSP(&tradeSp, tradeServerId);
		if(CTrade *pTrade = tradeSp.get())
		{
			return pTrade->GetTrader(traderIndex);
		}else
			g_Log.Add(CLog::Error, "[%s] Cannot find TradeSP[%d]", __FUNCTION__, tradeServerId);
	}
	return 0;
}