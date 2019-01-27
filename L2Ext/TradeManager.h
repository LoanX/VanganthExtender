#pragma once
#include "MemoryObject.h"

class CTrade : public MemoryObject
{
public:
	/* 18 */ LPVOID pUnkn18;
	/* 20 */ LPVOID pUnkn20;
	/* 28 */ UINT traderId[2];
	/* 30 */ vector<UINT> items[2]; 
	User* GetTrader(UINT index);	//Index can be 0 or 1
	void Lock();
	void Unlock();
	bool CanAdd(INT32 itemId, UINT userId);
};

typedef CSPointer<CTrade> CTradeSP;

class CTradeManager
{
	CTradeManager* pInstance;
public:
	CTradeManager(void);
	CTrade* GetTradeSP(CTradeSP* pTradeSP, UINT tradeServerId);
	User *GetTrader(UINT tradeServerId, UINT traderIndex);	//TraderIndex can be 0 and 1
	virtual ~CTradeManager(void);
};

extern CTradeManager g_TradeManager;