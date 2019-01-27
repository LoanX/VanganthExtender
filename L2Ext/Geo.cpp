#include "StdAfx.h"
#include "Geo.h"
#include "DoorData.h"

CGeo* CGeo::GetInstance()
{
	//0x198E7B78 there is our geo
	static CGeo *pGeo = *(CGeo**)0x198E7B78;
	return pGeo;
}

bool CGeo::CanSee(FVector* Pos1, FVector* Pos2)
{
	typedef bool (*f)(CGeo*, FVector*, FVector*);
	if(this)
	{
		if(f(0x005CFAC0L)(this, Pos1, Pos2))
		{
			if(g_DoorData.IsEnabled())
			{
				if(!g_DoorData.CanSee(*Pos1, *Pos2))
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}else
	{
		g_Log.Add(CLog::Error, "[%s] CGeo NULL", __FUNCTION__);
		return true;
	}
};

int CGeo::GetZLoc(int x, int y, int z)
{
	typedef int (*f)(CGeo*, int, int, int);
	if(this)
	{
		return f(0x5CC860L)(this, x, y, z);
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Null instance!", __FUNCTION__);
	}
	return z;
}