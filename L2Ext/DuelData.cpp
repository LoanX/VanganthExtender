#include "StdAfx.h"
#include "DuelData.h"

void CDuel::BroadcastToAll(PCHAR Buff, int Size)
{
	Lock();
	list<CDuelUser>::iterator Iter;
	for(Iter = lTeam1.begin();Iter != lTeam1.end();Iter++)
	{
		Iter->Send(Buff, Size);
	}
	for(Iter = lTeam2.begin();Iter != lTeam2.end();Iter++)
	{
		Iter->Send(Buff, Size);
	}
	Unlock();
}

void CDuel::BroadcastToTeam(int Team, PCHAR Buff, int Size)
{
	Lock();
	switch(Team)
	{
	case 1:
		{
			list<CDuelUser>::iterator Iter;
			for(Iter = lTeam1.begin();Iter != lTeam1.end();Iter++)
			{
				Iter->Send(Buff, Size);
			}
			break;
		}
	case 2:
		{
			list<CDuelUser>::iterator Iter;
			for(Iter = lTeam2.begin();Iter != lTeam2.end();Iter++)
			{
				Iter->Send(Buff, Size);
			}
			break;
		}
	}
	Unlock();
}

void CDuel::Start(User *pUser1, User *pUser2, bool Party, int nDuelID)
{
	Lock();
	if(Party)
	{
		Mode = PARTY;
		//Adding party members
		CParty *pParty = pUser1->GetParty();
		if(pParty)
		{
			UserVector vMembers;
			pParty->GetAllMembersEx(vMembers);
			UserVector::iterator Iter = vMembers.begin();
			for(;Iter!=vMembers.end();Iter++)
			{
				User *pUser = *Iter;
				if(pUser->ValidUser())
				{
					CDuelUser DuelUser(pUser, 1);
					pUser->pSD->DuelInfo.Set(pUser2->nObjectID, nDuelID, 1);
					lTeam1.push_back(DuelUser);
				}
			}
		}else
			g_Log.Add(CLog::Error, "[%s] Party for user[%S]- 1 - Cannot be found.", __FUNCTION__, pUser1->pSD->wszName);
		pParty = pUser2->GetParty();
		if(pParty)
		{
			UserVector vMembers;
			pParty->GetAllMembersEx(vMembers);
			UserVector::iterator Iter = vMembers.begin();
			for(;Iter!=vMembers.end();Iter++)
			{
				User *pUser = *Iter;
				if(pUser->ValidUser())
				{
					CDuelUser DuelUser(pUser, 2);
					pUser->pSD->DuelInfo.Set(pUser1->nObjectID, nDuelID, 2);
					lTeam2.push_back(DuelUser);
				}
			}
		}else
			g_Log.Add(CLog::Error, "[%s] Party for user[%S]- 2 - Cannot be found.", __FUNCTION__, pUser2->pSD->wszName);
	}
	else
	{
		Mode = SOLO;
		
		CDuelUser DuelUser1(pUser1, 1);
		pUser1->pSD->DuelInfo.Set(pUser2->nObjectID, nDuelID, 1);
		lTeam1.push_back(DuelUser1);
		CDuelUser DuelUser2(pUser2, 2);
		pUser2->pSD->DuelInfo.Set(pUser1->nObjectID, nDuelID, 2);
		lTeam2.push_back(DuelUser2);
	}
	wsLeader1 = pUser1->pSD->wszName;
	wsLeader2 = pUser2->pSD->wszName;
	StartTime = time(NULL);
	Unlock();
}

void CDuel::BroadcastUpdateStatus()
{
	Lock();
	try
	{
		list<CDuelUser>::iterator Iter;
		for(Iter = lTeam1.begin();Iter != lTeam1.end();Iter++)
		{
			if(User *pUser = Iter->GetUser())
			{
				CHAR buff[8190];
				int len = Assemble(buff, 8190, "chSddddddddd", 0xFE, 0x4F, pUser->pSD->wszName, pUser->nObjectID, pUser->pSD->nClass, pUser->pSD->nLevel,
					(int)pUser->pSD->fHP, (int)pUser->pSD->MaxHP,
					(int)pUser->pSD->fMP, (int)pUser->pSD->MaxMP,
					(int)pUser->pSD->fCP, (int)pUser->pSD->MaxCP );
				
				if(len > 0)
				{
					list<CDuelUser>::iterator Iter2;
					for(Iter2 = lTeam2.begin();Iter2 != lTeam2.end();Iter2++)
					{
						Iter2->Send(buff, len);
					}
				}
			}
		}
		for(Iter = lTeam2.begin();Iter != lTeam2.end();Iter++)
		{
			if(User *pUser = Iter->GetUser())
			{
				CHAR buff[8190];
				int len = Assemble(buff, 8190, "chSddddddddd", 0xFE, 0x4F, pUser->pSD->wszName, pUser->nObjectID, pUser->pSD->nClass, pUser->pSD->nLevel,
					(int)pUser->pSD->fHP, (int)pUser->pSD->MaxHP,
					(int)pUser->pSD->fMP, (int)pUser->pSD->MaxMP,
					(int)pUser->pSD->fCP, (int)pUser->pSD->MaxCP );
				if(len > 0)
				{
					list<CDuelUser>::iterator Iter2;
					for(Iter2 = lTeam1.begin();Iter2 != lTeam1.end();Iter2++)
					{
						Iter2->Send(buff, len);
					}
				}
			}
		}
	}catch(...)
	{
		EXCEPTION_LOG;
	}
	Unlock();
}

void CDuel::StartFight()
{
	BroadcastUpdateStatus();

	CHAR Msg[128];
	int nMsg = Assemble(Msg, 128, "cdd", SYSTEM_MESSAGE, SystemMessage::LET_THE_DUEL_BEGIN_, 0);
	CHAR Ready[128];
	int nReady = Assemble(Ready, 128, "chd", EX_PACKET, DUEL_READY, (DWORD)Mode);
	CHAR Start[128];
	int nStart = Assemble(Start, 128, "chd", EX_PACKET, DUEL_START, (DWORD)Mode);
	Lock();
	list<CDuelUser>::iterator Iter;
	for(Iter = lTeam1.begin();Iter != lTeam1.end();Iter++)
	{

		Iter->StartFight();
		Iter->Send(Msg, nMsg);
		Iter->Send(Ready, nReady);
		Iter->Send(Start, nStart);
		
	}
	for(Iter = lTeam2.begin();Iter != lTeam2.end();Iter++)
	{
		Iter->StartFight();
		Iter->Send(Msg, nMsg);
		Iter->Send(Ready, nReady);
		Iter->Send(Start, nStart);
	}
	Unlock();

	BroadcastUpdateStatus();
}

void CDuel::End()
{
	guard;
	if(Winner != DRAW && Winner != NONE)
	{
		if(Mode == SOLO)
		{
			CSystemMessage msg(SystemMessage::$C1_HAS_WON_THE_DUEL_);
			if(Winner == TEAM_1)
			{
				msg.AddText((PWCHAR)wsLeader1.c_str());
			}else
			{
				msg.AddText((PWCHAR)wsLeader2.c_str());
			}
			BroadcastToAll(msg.GetBuff(), (int)msg.GetSize());
		}else
		{
			CSystemMessage msg(SystemMessage::$C1_S_PARTY_HAS_WON_THE_DUEL_);
			if(Winner == TEAM_1)
			{
				msg.AddText((PWCHAR)wsLeader1.c_str());
			}else
			{
				msg.AddText((PWCHAR)wsLeader2.c_str());
			}
			BroadcastToAll(msg.GetBuff(), (int)msg.GetSize());
		}
	}
	Lock();
	list<CDuelUser>::iterator Iter;
	for(Iter=lTeam1.begin();Iter!=lTeam1.end();Iter++)
	{
		Iter->DoEndJob();
		if(Winner == TEAM_1)
		{
			Iter->DoSocialAction(SocialActionVictory);
		}else if (Winner == TEAM_2)
		{
			Iter->DoSocialAction(SocialActionBow);
		}
	}
	for(Iter=lTeam2.begin();Iter!=lTeam2.end();Iter++)
	{
		Iter->DoEndJob();
		if(Winner == TEAM_2)
		{
			Iter->DoSocialAction(SocialActionVictory);
		}else if (Winner == TEAM_1)
		{
			Iter->DoSocialAction(SocialActionBow);
		}
	}
	State = END;
	Unlock();
	unguard;
}

void CDuel::SetWinner(WINNER Win)
{
	Lock();
	Winner = Win;
	Unlock();
}
bool CDuel::IsAnyUnblocked(int nTeam)
{
	Lock();
	switch(nTeam)
	{
	case 1:
		{
			list<CDuelUser>::iterator Iter;
			for(Iter=lTeam1.begin();Iter!=lTeam1.end();Iter++)
			{
				if(!Iter->IsBlocked())
				{
					Unlock();
					return true;
					break;
				}
			}
			break;
		}
	case 2:
		{
			list<CDuelUser>::iterator Iter;
			for(Iter=lTeam2.begin();Iter!=lTeam2.end();Iter++)
			{
				if(!Iter->IsBlocked())
				{
					Unlock();
					return true;
					break;
				}
			}
			break;
		}
	}
	Unlock();
	return false;
}

void CDuel::Interrupt()
{
	guard;
	SetWinner(DRAW);
	End();
	unguard;
}

void CDuel::BlockUser(User *pUser, int nTeam)
{
	Lock();
	switch(nTeam)
	{
	case 1:
		{
			list<CDuelUser>::iterator Iter;
			for(Iter=lTeam1.begin();Iter!=lTeam1.end();Iter++)
			{
				if(Iter->userServerId == pUser->nObjectID)
				{
					Iter->Block();
					break;
				}
			}
			break;
		}
	case 2:
		{
			list<CDuelUser>::iterator Iter;
			for(Iter=lTeam2.begin();Iter!=lTeam2.end();Iter++)
			{
				if(Iter->userServerId == pUser->nObjectID)
				{
					Iter->Block();
					break;
				}
			}
			break;
		}
	}
	Unlock();
}

bool CDuel::Die(User *pUser)
{
	guard;
	if(pUser->pSD->DuelInfo.DuelID && pUser->pSD->DuelInfo.DuelTeam)
	{
		switch(Mode)
		{
		case SOLO:
			{
				if(pUser->pSD->DuelInfo.DuelTeam == 1)
				{
					SetWinner(TEAM_2);
				}else
				{
					SetWinner(TEAM_1);
				}
				End();
				unguard;
				return true;
				break;
			};
		case PARTY:
			{
				//Party
				if(pUser->pSD->DuelInfo.DuelTeam == 1)
				{
					BlockUser(pUser, 1);
					if(IsAnyUnblocked(1))
					{
						unguard;
						return true;
					}
					else
						SetWinner(TEAM_2);
				}else
				{
					BlockUser(pUser, 2);
					if(IsAnyUnblocked(2))
					{
						unguard;
						return true;
					}
					else
						SetWinner(TEAM_1);
				}
				End();
				unguard;
				return true;
				break;
			}
		default:
			{
				g_Log.Add(CLog::Error, "[%s] Invalid duel mode[%d]", __FUNCTION__, Mode);
				break;
			}
		}
	}
	unguard;
	return false;
}

void CDuel::GetTeamMembers(vector<User*>& vMembers, int team)
{
	guard;
	
	Lock();

	if(team == 1)
	{
		for(list<CDuelUser>::iterator it = lTeam1.begin(); it != lTeam1.end(); it++)
		{
			if(User *pUser = User::GetUserBySID(&it->userServerId))
			{
				vMembers.push_back(pUser);
			}
		}
	}else if(team == 2)
	{
		for(list<CDuelUser>::iterator it = lTeam2.begin(); it != lTeam2.end(); it++)
		{
			if(User *pUser = User::GetUserBySID(&it->userServerId))
			{
				vMembers.push_back(pUser);
			}
		}
	}

	Unlock();

	unguard;
}

void CDuel::Withdraw(User *pUser)
{
	guard;

	wstring wsWinner;
	wstring wsLoser;
	if(pUser->pSD->DuelInfo.DuelTeam == 1)
	{
		wsWinner = wsLeader2;
		wsLoser = wsLeader1;
		SetWinner(TEAM_2);
	}else
	{
		wsWinner = wsLeader1;
		wsLoser = wsLeader2;
		SetWinner(TEAM_1);
	}
	CHAR Msg[256];
	int nMsg = 0;
	if(Mode == SOLO)
	{
		nMsg = Assemble(Msg, 256, "cdddSdS", SYSTEM_MESSAGE, SystemMessage::SINCE_$C1_WITHDREW_FROM_THE_DUEL__$S2_HAS_WON_, 2, 0, pUser->pSD->wszName, 0, wsWinner.c_str());
	}else
	{
		nMsg = Assemble(Msg, 256, "cdddSdS", SYSTEM_MESSAGE, SystemMessage::SINCE_$C1_S_PARTY_WITHDREW_FROM_THE_DUEL__$S2_S_PARTY_HAS_WON_, 2, 0, wsLoser.c_str(), 0, wsWinner.c_str());
	}
	BroadcastToAll(Msg,nMsg);
	End();
	unguard;
}

void CDuel::TimerExpired()
{
	time_t CurrTime = time(NULL);
	switch(State)
	{
	case COUNTDOWN:
		{
			time_t TimeDiff = CurrTime - StartTime;
			if(TimeDiff >= m_CountdownTime)
			{
				//Start fight
				State = FIGHT;
				StartFight();
			}else
			{
				//Counting
				TimeDiff = m_CountdownTime - TimeDiff;
				CHAR Msg[128];
				int nMsg = Assemble(Msg, 128, "cdddd", SYSTEM_MESSAGE, SystemMessage::THE_DUEL_WILL_BEGIN_IN_$S1_SECONDXSX_, 1, 1, (DWORD)TimeDiff);
				BroadcastToAll(Msg, nMsg);
			}
			break;
		}
	case FIGHT:
		{
			time_t TimeDiff = CurrTime - StartTime - m_CountdownTime;
			if(TimeDiff > m_FightTime)
			{
				//Timeout
				CHAR Msg[256];
				int nMsg = Assemble(Msg, 256, "cdd", SYSTEM_MESSAGE, SystemMessage::THE_DUEL_HAS_ENDED_IN_A_TIE_, 0);
				SetWinner(DRAW);
				BroadcastToAll(Msg, nMsg);
				End();
			}
			break;
		}
	case END:
		{
			g_Log.Add(CLog::Error, "[%s] Invalid Duel State[%d]", __FUNCTION__, State);
			break;
		}
	};
}