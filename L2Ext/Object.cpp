#include "StdAfx.h"

bool CObject::IsValid(DWORD dwVTable)
{
	if(this)
	{
		try
		{
			if( (*(LPDWORD)this) == dwVTable ) 
			{
				return true;
			}
		}catch(...)
		{
			return false;
		}
	}
	return false;
};

CParty* CObject::SafeCastParty()
{
	if(IsValid(VT_PARTY))
	{
		return reinterpret_cast<CParty*>(this);
	}
	return 0;
}

User* CObject::SafeCastUser()
{
	if(IsValid(VT_USER))
	{
		return reinterpret_cast<User*>(this);
	}
	return 0;
}

CSummon* CObject::SafeCastSummon()
{
	if(IsValid(VT_CSUMMON))
	{
		return reinterpret_cast<CSummon*>(this);
	}
	return 0;
}

CCreature* CObject::SafeCastCreature()
{
	if(this)
	{
		DWORD dwVTable = (*(LPDWORD)this);
		switch(dwVTable)
		{
		case VT_CBOSS:
		case VT_CMERCHANT:
		case VT_CCREATURE:
		case VT_CNPC:
		case VT_CPET:
		case VT_CSUMMON:
		case VT_CZZOLDAGU:
		case VT_USER:
			return reinterpret_cast<CCreature*>(this);
		}
	}
	return 0;
}

CItem* CObject::SafeCastItem()
{
	if(this)
	{
        try
		{ 
			if(    (*(LPDWORD)this) == 0xA07CF8 
				|| (*(LPDWORD)this) == 0xA0B368 
				|| (*(LPDWORD)this) == 0xA0A448 
				|| (*(LPDWORD)this) == 0xA09558
				|| (*(LPDWORD)this) == 0xA09CC8
				|| (*(LPDWORD)this) == 0xA08DC8
				|| (*(LPDWORD)this) == 0xA0ABD8 )
			{
				return reinterpret_cast<CItem*>(this);
			}
		}
        catch(...)
		{ 
			/* Invalid pointer */
			/* Preventing Crash ;) */
		}
	}
	return 0;
}