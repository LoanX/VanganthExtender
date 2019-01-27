#pragma once

class CCastle: public CResidence
{
public:
	CCastle *Constructor()
	{
		typedef CCastle* (*_f)(CCastle*);
		_f f = (_f) 0x004AC2F0;
		return f(this);
	}
	LPVOID Destructor(DWORD dData)
	{
		typedef LPVOID (*_f)(CCastle*, DWORD);
		_f f = (_f) 0x004AC680;
		return f(this, dData);
	}
	static bool IsAnyCastleDuringSiege()
	{
		for(int n=1;n<10;n++)
		{
			CCastle *pCastle = (CCastle*)g_ResidenceDB.GetResidence(n);
			if(pCastle)
			{
				if(pCastle->IsSiegeTime())
					return true;
			}
		}
		return false;
	}
	static bool IsCastleSiegeRegistered(CPledge *pPledge)
	{
		typedef bool (*f)(CPledge*);
		return f(0x49F740L)(pPledge);
	}
	static int GetBattleFieldCastleId(FVector* pPos)
	{
		typedef int (*f)(FVector*);
		return f(0x4A09A0L)(pPos);
	}
	void BroadcastSetSiegeTime()
	{
		typedef void(*f)(CCastle*);
		f(0x4A1160L)(this);
	}
};