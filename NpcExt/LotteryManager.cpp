#include "stdafx.h"
#include "LotteryManager.h"

CLotteryManager::CLotteryManager() : m_Enabled(0), m_PriceId(0), m_RewardId(0), m_RewardMultipler(0.0)
{
}

/*
[LotteryManager]
Enabled=1
PriceItemId=57
RewardItemId=57
;to divide reward 1000x use 0.001 as a multipler
RewardMultimpler=1.0
*/

CLotteryManager& CLotteryManager::Get()
{
	static CLotteryManager m_Instance;
	return m_Instance;
}

void CLotteryManager::Init()
{
	TCHAR file[260] = { 0 };
	GetCurrentDirectory(MAX_PATH,file);
	lstrcat(file, _T("\\l2npc.ini"));
	const TCHAR* section = _T("LotteryManager");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, file) ? true : false;
	m_PriceId = GetPrivateProfileInt(section, _T("PriceItemId"), 57, file);
	m_RewardId = GetPrivateProfileInt(section, _T("RewardItemId"), 57, file);
	m_RewardMultipler = GetPrivateProfileDouble(section, _T("RewardMultimpler"), 1.0, file);
	if(m_Enabled)
	{
		static bool hooked = false;
		if(!hooked)
		{
			hooked = true;
			WriteCall(0x50C4B0L, CLotteryManager::OnSEAddAtomicItemProcess_BuyTicket);
			WriteCall(0x50E33FL, CLotteryManager::OnCalculateReward);
			WriteCall(0x50E38CL, CLotteryManager::OnSEAddAtomicItemProcess_GiveReward);
			WriteCall(0x4B4DB7L, CLotteryManager::OnGetAccumulatedReward);
			WriteCall(0x50C6BDL, CLotteryManager::OnGetAccumulatedReward);
		}
		g_Log.Add(LogBlue, "[%s] PriceId[%d] RewardId[%d] RewardMultipler[%f]", __FUNCTION__, m_PriceId, m_RewardId, m_RewardMultipler);
	}
}

int CLotteryManager::OnGetAccumulatedReward(PVOID instancePtr)
{
	typedef int(*f)(PVOID);
	if(CLotteryManager::Get().Enabled())
	{
		int r = f(0x4B3730L)(instancePtr);
		r = static_cast<int>( double(r) * CLotteryManager::Get().RewardMultipler() );
		if(r == 0)
			r = 1;
		return r;
	}

	return f(0x4B3730L)(instancePtr);
}

void CLotteryManager::OnSEAddAtomicItemProcess_GiveReward(int userIndex, int itemId, int amount, int enchant, int blessed)
{
	if(CLotteryManager::Get().Enabled())
	{
		typedef void(*f)(int, int, int, int, int);
		f(0x5C3090L)(userIndex, CLotteryManager::Get().RewardId(), amount, enchant, blessed);
	}else
	{
		typedef void(*f)(int, int, int, int, int);
		f(0x5C3090L)(userIndex, itemId, amount, enchant, blessed);
	}
}

int CLotteryManager::OnCalculateReward(PVOID instancePtr, int roundNumber, int flagNumber, PUINT statePtr)
{
	typedef int (*f)(PVOID, int, int, PUINT);
	if(CLotteryManager::Get().Enabled())
	{
		int amount = f(0x4B4710L)(instancePtr, roundNumber, flagNumber, statePtr);
		g_Log.Add(LogBlue, "[%s] Calculate base amount[%d] multiplied[%d]", __FUNCTION__, amount, static_cast<int>( double(amount) * CLotteryManager::Get().RewardMultipler() ));
		amount = static_cast<int>( double(amount) * CLotteryManager::Get().RewardMultipler() );
		if(amount == 0)
			amount = 1;

		return amount;
	}
	return f(0x4B4710L)(instancePtr, roundNumber, flagNumber, statePtr);
}

void CLotteryManager::OnSEAddAtomicItemProcess_BuyTicket(int userIndex, int itemId, int amount, int enchant, int blessed)
{
	if(CLotteryManager::Get().Enabled())
	{
		typedef void(*f)(int, int, int, int, int);
		f(0x5C3090L)(userIndex, CLotteryManager::Get().PriceId(), amount, enchant, blessed);
	}else
	{
		typedef void(*f)(int, int, int, int, int);
		f(0x5C3090L)(userIndex, itemId, amount, enchant, blessed);
	}
}