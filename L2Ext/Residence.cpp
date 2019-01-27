#include "StdAfx.h"

bool CResidence::IsCastle()
{
	if(this)
	{
		try
		{
			if( (*(LPDWORD)this) == VT_CCASTLE )
				return true;
		}catch(...)
		{
		}
	}
	return false;
}

bool CResidence::IsAgit()
{
	if(this)
	{
		try
		{
			switch((*(LPDWORD)this))
			{
			case VT_CAGIT:
			case VT_CAUCTIONAGIT:
			case VT_CNPCBATTLEAGIT:
			case VT_CSIEGEAGIT:
			case VT_CTEAMBATTLEAGIT:
				{
					return true;
					break;
				}
			default:
				{
					return false;
					break;
				}
			}
		}catch(...)
		{
		}
	}
	return false;
}

bool CResidence::IsSiegeTime()
{
	if(this)
	{
		try
		{
			if(pRD)
			{
				__time32_t SiegeTime = pRD->SiegeDate;
				__time32_t CurrentTime = _time32(NULL);
				if( CurrentTime > SiegeTime )
				{
					if( (SiegeTime + siegePeriod + lostTime) > CurrentTime )
					{
						return true;
					}
				}
			}
		}catch(...)
		{
			return false;
		}
	}
	return false;
}

RESIDENCE CResidence::CastType(int Residence)
{
	switch(Residence)
	{
	case GLUDIO:
		return GLUDIO;
		break;
	case DION:
		return DION;
		break;
	case GIRAN:
		return GIRAN;
		break;
	case OREN:
		return OREN;
		break;
	case ADEN:
		return ADEN;
	case INNADRIL:
		return INNADRIL;
		break;
	case GODDARD:
		return GODDARD;
		break;
	case RUNE:
		return RUNE;
		break;
	case SCHUTTGARD:
		return SCHUTTGARD;
	default:
		if(Residence > 9)
			return CLAN_HALL;
		else
			return NOT_RESIDENCE;
		break;
	};
}

CPledge* CResidence::GetPledge()
{
	if(this)
	{
		try
		{
			return *(CPledge**)((UINT64)this+0x14618);
		}
		catch(...)
		{
			return NULL;
		}
	}
	return NULL;
};

bool CResidence::IsOffendingPledge(int id)
{
	typedef bool (*f)(CResidence*, int);
	return f(0x7767D0L)(this, id);
}

bool CResidence::IsDefendingPledge(int id)
{
	typedef bool (*f)(CResidence*, int);
	return f(0x776670L)(this, id);
}

void CResidence::SetPledge(CPledge *pPledge)
{
	typedef void (*f)(CResidence*, CPledge*);
	f(0x776FB0L)(this, pPledge);
}

void CResidence::ChangeResidenceState(UINT state)
{
	typedef void(*f)(CResidence*, UINT);
	f(0x776CD0L)(this, state);
}