#include "stdafx.h"
#include "MultiPartyCommandChannel.h"
#include "PlayerAction.h"
#include "Geo.h"

void CMultiPartyCommandChannel::TeleportMPCCToLocation(UINT requestorId, INT32 x, INT32 y, INT32 z)
{
	guard;

	for(UINT n=0;n<parties.size();n++)
	{
		if(CParty *pParty = CObject::GetObjectBySID(parties[n])->SafeCastParty())
		{
			UserVector members;
			pParty->GetAllMembersEx(members);
			for(UINT m=0;m<members.size();m++)
			{
				if(User *pUser = members[m]->SafeCastUser())
				{
					if(pUser->pSD->nAlive)
					{
						pUser->StopMove();
						INT32 rand = g_Random.RandInt(10);
						rand -= 5;
						INT32 _x = x + rand;
						rand = g_Random.RandInt(10);
						rand -= 5;
						INT32 _y = y + rand;
						PlayerAction::OnTeleportToLocation(pUser, _x, _y, CGeo::GetInstance()->GetZLoc(_x, _y, z), false);
					}else
					{
						g_Log.Add(CLog::Error, "[%s] Cannot teleport dead player[%S]", __FUNCTION__, pUser->pSD->wszName);
					}
				}else
				{
					g_Log.Add(CLog::Error, "[%s] Invalid User[%p]!", __FUNCTION__, members[m]);
				}
			}
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Invalid Party[%d]!", __FUNCTION__, parties[n]);
		}
	}

	unguard;
}

void CMultiPartyCommandChannel::TeleportMPCCToLocationByIndex(UINT requestorId, UINT index, INT32 x, INT32 y, INT32 z)
{
	guard;

	if(index < parties.size())
	{
		if(CParty *pParty = CObject::GetObjectBySID(parties[index])->SafeCastParty())
		{
			UserVector members;
			pParty->GetAllMembersEx(members);
			for(UINT m=0;m<members.size();m++)
			{
				if(User *pUser = members[m]->SafeCastUser())
				{
					if(pUser->pSD->nAlive)
					{
						pUser->StopMove();
						INT32 rand = g_Random.RandInt(10);
						rand -= 5;
						INT32 _x = x + rand;
						rand = g_Random.RandInt(10);
						rand -= 5;
						INT32 _y = y + rand;
						PlayerAction::OnTeleportToLocation(pUser, _x, _y, CGeo::GetInstance()->GetZLoc(_x, _y, z), false);
					}else
					{
						g_Log.Add(CLog::Error, "[%s] Cannot teleport dead player[%S]", __FUNCTION__, pUser->pSD->wszName);
					}
				}else
				{
					g_Log.Add(CLog::Error, "[%s] Invalid User[%p]!", __FUNCTION__, members[m]);
				}
			}
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Invalid Party[%d]!", __FUNCTION__, parties[index]);
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Invalid party index[%d] max parties[%d]", __FUNCTION__, index, parties.size());
	}

	unguard;
}