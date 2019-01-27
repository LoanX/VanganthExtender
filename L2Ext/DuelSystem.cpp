#include "StdAfx.h"
#include "DuelSystem.h"

CDuelSystem g_DuelSystem;

CDuelSystem::CDuelSystem() : m_Enabled(FALSE), m_FightTime(300), m_CountdownTime(20)
{
}

CDuelSystem::~CDuelSystem()
{
}

/*

[Duel]
Enabled=1
;countdown time in seconds
CountdownTime=20
;fight time in seconds
FightTime=300
;Cancel all buffs/debuffs when duel starts
DispelAbnormals=1
*/

void CDuelSystem::LoadINI()
{
	guard;
	const TCHAR* section = _T("Duel");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), FALSE, g_ConfigFile);
	m_CountdownTime = GetPrivateProfileInt(section, _T("CountdownTime"), FALSE, g_ConfigFile);
	m_FightTime = GetPrivateProfileInt(section, _T("FightTime"), FALSE, g_ConfigFile);
	m_DispelAbnormals = GetPrivateProfileInt(section, _T("DispelAbnormals"), FALSE, g_ConfigFile);

	unguard;
}

void CDuelSystem::Initialize()
{
	LoadINI();
	if(m_Enabled)
	{
		g_Log.Add(CLog::Blue, "[%s] done.", __FUNCTION__);
	}
}
bool CDuelSystem::CanDuel(CUserSocket *pSocket, User *pTarget)
{
	if(!pTarget->ValidUser())
	{
		pSocket->SendSystemMessage(SystemMessage::THERE_IS_NO_OPPONENT_TO_RECEIVE_YOUR_CHALLENGE_FOR_A_DUEL_);
		return false;
	}
	if(pTarget->nObjectID == pSocket->GetUser()->nObjectID)
	{
		pSocket->SendSystemMessage(SystemMessage::THERE_IS_NO_OPPONENT_TO_RECEIVE_YOUR_CHALLENGE_FOR_A_DUEL_);
		return false;
	}
	if(pTarget->pSD->nStoreMode || pTarget->pSD->activateOfflineShopTime)
	{
		pSocket->SendSystemMessage_S(SystemMessage::$C1_CANNOT_DUEL_BECAUSE_$C1_IS_CURRENTLY_ENGAGED_IN_A_PRIVATE_STORE_OR_MANUFACTURE_, pTarget->pSD->wszName);
		return false;
	}
	if(!pTarget->pSD->nAlive || (pTarget->pSD->fHP < (pTarget->pSD->MaxHP/2)) || (pTarget->pSD->fMP < (pTarget->pSD->MaxMP/2)))
	{
		pSocket->SendSystemMessage_S(SystemMessage::$C1_CANNOT_DUEL_BECAUSE_$C1_S_HP_OR_MP_IS_BELOW_50PERCENT_, pTarget->pSD->wszName);
		return false;
	}
	if(pTarget->pSD->nInPeaceZone)
	{
		pSocket->SendSystemMessage_S(SystemMessage::$C1_CANNOT_MAKE_A_CHALLENGE_TO_A_DUEL_BECAUSE_$C1_IS_CURRENTLY_IN_A_DUELXPROHIBITED_AREA_XPEACEFUL_ZONE___SEVEN_SIGNS_ZONE___NEAR_WATER___RESTART_PROHIBITED_AREAX_, pTarget->pSD->wszName);
		return false;
	}
	if(pTarget->InCombat() && (pTarget->CombatTime() + 30) > (DWORD)time(NULL))
	{
		pSocket->SendSystemMessage_S(SystemMessage::$C1_CANNOT_DUEL_BECAUSE_$C1_IS_CURRENTLY_ENGAGED_IN_BATTLE_, pTarget->pSD->wszName);
		return false;
	}
	
	if(pTarget->pSD->bGuilty)
	{
		pSocket->SendSystemMessage_S(SystemMessage::$C1_CANNOT_DUEL_BECAUSE_$C1_IS_CURRENTLY_ENGAGED_IN_BATTLE_, pTarget->pSD->wszName);
		return false;
	}

	if(pTarget->pSD->DuelInfo.DuelID)
	{
		pSocket->SendSystemMessage_S(SystemMessage::$C1_CANNOT_DUEL_BECAUSE_$C1_IS_ALREADY_ENGAGED_IN_A_DUEL_, pTarget->pSD->wszName);
		return false;
	}
	if(pTarget->pSD->nKarma)
	{
		pSocket->SendSystemMessage_S(SystemMessage::$C1_CANNOT_DUEL_BECAUSE_$C1_IS_IN_A_CHAOTIC_STATE_, pTarget->pSD->wszName);
		return false;
	}
	if(pTarget->olympiadUser.GetState() != 0)
	{
		pSocket->SendSystemMessage_S(SystemMessage::$C1_CANNOT_DUEL_BECAUSE_$C1_IS_PARTICIPATING_IN_THE_OLYMPIAD_, pTarget->pSD->wszName);
		return false;
	}
	if(pTarget->pSD->bIsRiding)
	{
		pSocket->SendSystemMessage_S(SystemMessage::$C1_CANNOT_DUEL_BECAUSE_$C1_IS_CURRENTLY_RIDING_A_BOAT__STEED__OR_STRIDER_, pTarget->pSD->wszName);
		return false;
	}
	if(!pSocket->GetUser()->CheckDistance(pTarget, 2000))
	{
		pSocket->SendSystemMessage_S(SystemMessage::$C1_CANNOT_RECEIVE_A_DUEL_CHALLENGE_BECAUSE_$C1_IS_TOO_FAR_AWAY_, pTarget->pSD->wszName);
		return false;
	}
	return true;
}

void CDuelSystem::UpdateDuelUserInfo(User *pUser)
{
	guard;

	if(pUser->ValidUser())
	{
		if(pUser->pSD->DuelInfo.DuelID > 0)
		{
			CHAR buff[8190];
			int len = Assemble(buff, 8190, "chSddddddddd", 0xFE, 0x4F, pUser->pSD->wszName, pUser->nObjectID, pUser->pSD->nClass, pUser->pSD->nLevel,
				(int)pUser->pSD->fHP, (int)pUser->pSD->MaxHP,
				(int)pUser->pSD->fMP, (int)pUser->pSD->MaxMP,
				(int)pUser->pSD->fCP, (int)pUser->pSD->MaxCP );

			vector<User*> vTeam;
			m_Lock.Enter();
			DuelMap::iterator Iter = m_Duels.find(pUser->pSD->DuelInfo.DuelID);
			if(Iter!=m_Duels.end())
			{
				if(pUser->pSD->DuelInfo.DuelTeam == 1)
				{
					Iter->second->GetTeamMembers(vTeam, 2);
				}else if(pUser->pSD->DuelInfo.DuelTeam == 2)
				{
					Iter->second->GetTeamMembers(vTeam, 1);
				}
			}
			m_Lock.Leave();
			for(UINT n=0; n < vTeam.size(); n++)
			{
				if(User *pMember = vTeam[n]->SafeCastUser())
				{
					pMember->pSocket->Send("b", len, buff);
				}
			}
		}
	}
	unguard;
}

void CDuelSystem::Damaged(CCreature *pCreature, CCreature *pEnemy)
{
	guard;
	if(m_Enabled)
	{
		if(pEnemy->ValidCreature() && pCreature->ValidCreature())
		{
			int nDuelID1 = 0;
			int nDuelID2 = 0;
			if(pCreature->IsSummon())
			{
				User *pMaster = pCreature->CastSummon()->GetMaster();
				if(pMaster->ValidUser())
				{
					nDuelID1 = pMaster->pSD->DuelInfo.DuelID;
					if(nDuelID1 && !pMaster->pSD->DuelInfo.Fighting)
					{
						Interrupt(nDuelID1);
						unguard;
						return;
					}
				}
			}else if(pCreature->ValidUser())
			{
				nDuelID1 = pCreature->pSD->DuelInfo.DuelID;
				if(nDuelID1 && !pCreature->pSD->DuelInfo.Fighting)
				{
					Interrupt(nDuelID1);
					unguard;
					return;
				}
			}
			if(pEnemy->IsSummon())
			{
				User *pMaster = pEnemy->CastSummon()->GetMaster();
				if(pMaster->ValidUser())
				{
					nDuelID2 = pMaster->pSD->DuelInfo.DuelID;
					if(nDuelID2 && !pMaster->pSD->DuelInfo.Fighting)
					{
						Interrupt(nDuelID2);
						unguard;
						return;
					}
				}
			}else if(pEnemy->ValidUser())
			{
				nDuelID2 = pEnemy->pSD->DuelInfo.DuelID;
				if(nDuelID2 && !pEnemy->pSD->DuelInfo.Fighting)
				{
					Interrupt(nDuelID2);
					unguard;
					return;
				}
			}
			if(nDuelID1 != nDuelID2)
			{
				if(nDuelID1)
					Interrupt(nDuelID1);
				if(nDuelID2)
					Interrupt(nDuelID2);
			}
		}
	}
	unguard;
}

bool CDuelSystem::Die(User *pUser)
{
	if(m_Enabled)
	{
		int nDuelID = pUser->pSD->DuelInfo.DuelID;
		bool bReturn = false;
		if(nDuelID)
		{
			m_Lock.Enter();
			DuelMap::iterator Iter = m_Duels.find(nDuelID);
			if(Iter!=m_Duels.end())
			{
				bReturn = Iter->second->Die(pUser);
				if(Iter->second->State == CDuel::END)
				{
					delete Iter->second;
					m_Duels.erase(Iter);
				}
			}
			m_Lock.Leave();
		}
		
		return bReturn;
	}

	return false;
}
int CDuelSystem::GenerateDuelID()
{
	int nDuelID = 1;
	for(nDuelID=1; nDuelID < (m_Duels.size() + 10); nDuelID++) //Basicly +2 would be enough :P
	{
		if(m_Duels.find(nDuelID) == m_Duels.end())
		{
			return nDuelID;
		}
	}
	return nDuelID;
}

void CDuelSystem::Interrupt(int nDuelID)
{
	guard;
	m_Lock.Enter();
	DuelMap::iterator Iter = m_Duels.find(nDuelID);
	if(Iter!=m_Duels.end())
	{
		Iter->second->Interrupt();
		delete Iter->second;
		m_Duels.erase(Iter);
	}
	m_Lock.Leave();
	unguard;
}

void CDuelSystem::LeaveWorld(User *pUser)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	if(pUser->ValidUser())
	{
		int nDuelID = pUser->pSD->DuelInfo.DuelID;
		if(nDuelID)
		{
			m_Lock.Leave();
			DuelMap::iterator Iter = m_Duels.find(nDuelID);
			if(Iter!=m_Duels.end())
			{
				Iter->second->Withdraw(pUser);
				delete Iter->second;
				m_Duels.erase(Iter);
			}
			m_Lock.Leave();
		}
	}
}
bool CDuelSystem::RequestAnswerDuel(CUserSocket *pSocket, const unsigned char* packet)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	int nParty, nUnkn, nAnswer;
	Disassemble(packet, "ddd", &nParty, &nUnkn, &nAnswer);
	User *pUser = pSocket->GetUser();
	if(pUser->ValidUser())
	{
		if(nAnswer)
		{
			//Start Duel
			switch(nParty)
			{
			case 1:
				{
					//Check Party Members
					if(pUser->pSD->DuelInfo.OpponentSID && !pUser->pSD->DuelInfo.DuelID)
					{
						User *pOpponent = User::GetUserBySID(&pUser->pSD->DuelInfo.OpponentSID);
						if(pOpponent->ValidUser())
						{
							CParty *pParty1 = pUser->GetParty();
							CParty *pParty2 = pOpponent->GetParty();
							if(pParty1 && pParty2 && (pParty1!=pParty2))
							{
								UserVector vUsers;
								pParty1->GetAllMembersEx(vUsers);
								UserVector::iterator Iter = vUsers.begin();
								for(;Iter!=vUsers.end();Iter++)
								{
									if(!CanDuel(pOpponent->GetSocket(), *Iter))
									{
										pUser->pSD->DuelInfo.Clear();
										pOpponent->pSD->DuelInfo.Clear();
										pSocket->SendSystemMessage(L"Your party doesn't meet the requiements.");
										return false;
									}
								}
								vUsers.clear();
								pParty2->GetAllMembersEx(vUsers);
								Iter = vUsers.begin();
								for(;Iter!=vUsers.end();Iter++)
								{
									if(!CanDuel(pSocket, *Iter))
									{
										pUser->pSD->DuelInfo.Clear();
										pOpponent->pSD->DuelInfo.Clear();
										pOpponent->GetSocket()->SendSystemMessage(L"Your party doesn't meet the requiements.");
										return false;
									}
								}
								StartDuel(pOpponent, pUser, true);
							}else
							{
								pUser->pSD->DuelInfo.Clear();
								pOpponent->pSD->DuelInfo.Clear();
							}
						}else
						{
							pUser->pSD->DuelInfo.Clear();
							pSocket->SendSystemMessage(SystemMessage::THERE_IS_NO_OPPONENT_TO_RECEIVE_YOUR_CHALLENGE_FOR_A_DUEL_);
						}
					}
					break;
				}
			default:
				{
					if(pUser->pSD->DuelInfo.OpponentSID && !pUser->pSD->DuelInfo.DuelID)
					{
						User *pOpponent = User::GetUserBySID(&pUser->pSD->DuelInfo.OpponentSID);
						if(CanDuel(pSocket, pOpponent))
						{
							if(CanDuel(pOpponent->GetSocket(), pUser))
							{
								//Start Duel
								StartDuel(pOpponent, pUser, false);
							}else
							{
								pUser->pSD->DuelInfo.Clear();
								pOpponent->pSD->DuelInfo.Clear();
							}
						}else
						{
							pUser->pSD->DuelInfo.Clear();
							if(pOpponent->ValidUser())
								pOpponent->pSD->DuelInfo.Clear();
						}
					}
					break;
				}
			}
		}else
		{
			//Cancel Duel
			if(pUser->pSD->DuelInfo.OpponentSID)
			{
				User *pOpponent = User::GetUserBySID(&pUser->pSD->DuelInfo.OpponentSID);
				if(pOpponent->ValidUser())
				{
					pOpponent->pSD->DuelInfo.Clear();
					if(nParty)
						pOpponent->GetSocket()->SendSystemMessage_S(SystemMessage::$C1_HAS_DECLINED_YOUR_CHALLENGE_TO_A_PARTY_DUEL_, pUser->pSD->wszName);
					else
						pOpponent->GetSocket()->SendSystemMessage_S(SystemMessage::$C1_HAS_DECLINED_YOUR_CHALLENGE_TO_A_DUEL_, pUser->pSD->wszName);
				}
			}
			pUser->pSD->DuelInfo.Clear();
		}
	}
	return false;
}

bool CDuelSystem::RequestAskDuel(CUserSocket *pSocket, const unsigned char* packet)
{
	User *pUser = pSocket->GetUser();
	if(pUser->ValidUser())
	{
		try
		{
			if(pUser->pSD->nInPeaceZone || pUser->pSD->nInBattleField)
			{
				pSocket->SendSystemMessage(SystemMessage::THIS_IS_NOT_A_SUITABLE_PLACE_TO_CHALLENGE_ANYONE_OR_PARTY_TO_A_DUEL_);
				return false;
			}
			if(pUser->pED->tvtUser.status != TvT::UserNone)
			{
				pSocket->SendSystemMessage(SystemMessage::YOU_ARE_UNABLE_TO_REQUEST_A_DUEL_AT_THIS_TIME_);
				return false;
			}

			if(pUser->pSD->activateOfflineShopTime || pUser->pSD->DuelInfo.DuelID || pUser->pSD->nKarma || (pUser->InCombat() && (pUser->CombatTime() + 30) > (DWORD)time(NULL)))
			{
				pSocket->SendSystemMessage(SystemMessage::YOU_ARE_UNABLE_TO_REQUEST_A_DUEL_AT_THIS_TIME_);
				return false;
			}
			if(pUser->pSD->DuelInfo.OpponentSID)
			{
				pSocket->SendSystemMessage(SystemMessage::WAITING_FOR_ANOTHER_REPLY_);
				return false;
			}
			WCHAR wszName[25]; int nParty;
			Disassemble(packet, "Sd", sizeof(wszName), wszName, &nParty);
			User *pTarget = g_UserDB.GetUserByName(wszName);
			if(pTarget->ValidUser())
			{
				if(CanDuel(pSocket, pTarget))
				{
					switch(nParty)
					{
					case 1:
						{
							//Party Duel
							CParty *pParty1 = pUser->GetParty();
							CParty *pParty2 = pTarget->GetParty();
							if(pParty1 && pParty2)
							{
								UserVector vUsers;
								pParty1->GetAllMembersEx(vUsers);
								for(UserVector::iterator Iter = vUsers.begin();Iter!=vUsers.end();Iter++)
								{
									if(!CanDuel(pSocket, *Iter))
									{
										return false;
									}
								}
								vUsers.clear();
								pParty2->GetAllMembersEx(vUsers);
								for(UserVector::iterator Iter = vUsers.begin();Iter!=vUsers.end();Iter++)
								{
									if(!CanDuel(pSocket, *Iter))
									{
										return false;
									}
								}
								pTarget->pSD->DuelInfo.Set(pUser->nObjectID, 0, 0);
								pUser->pSD->DuelInfo.Set(pTarget->nObjectID, 0, 0);
								CExRequestAskDuel pck(pUser->pSD->wszName, nParty);
								pTarget->GetSocket()->Send(&pck);
								pSocket->SendSystemMessage_S(SystemMessage::$C1_S_PARTY_HAS_BEEN_CHALLENGED_TO_A_DUEL_, pTarget->pSD->wszName);
							}else
							{
								pSocket->SendSystemMessage(SystemMessage::YOU_ARE_UNABLE_TO_REQUEST_A_DUEL_AT_THIS_TIME_);
								return false;
							}
							break;
						}
					default:
						{
							//Solo Duel
							pTarget->pSD->DuelInfo.Set(pUser->nObjectID, 0, 0);
							pUser->pSD->DuelInfo.Set(pTarget->nObjectID, 0, 0);
							pTarget->GetSocket()->Send("chSd", EX_PACKET, DUEL_ASK, pUser->pSD->wszName, 0);
							pSocket->SendSystemMessage_S(SystemMessage::$C1_HAS_BEEN_CHALLENGED_TO_A_DUEL_, pTarget->pSD->wszName);
							break;
						}
					}
				}
			}else
			{
				pSocket->SendSystemMessage(SystemMessage::THERE_IS_NO_OPPONENT_TO_RECEIVE_YOUR_CHALLENGE_FOR_A_DUEL_);
				return false;
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected!", __FUNCTION__);
		}
	}
	return false;
}

void CDuelSystem::StartDuel(User *pUser1, User *pUser2, bool Party)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	m_Lock.Enter();
	try
	{
		int nDuelID = GenerateDuelID();
		CDuel* pDuel = new CDuel(m_FightTime, m_CountdownTime);
		pDuel->Start(pUser1, pUser2, Party, nDuelID);
		m_Duels.insert(pair<int, CDuel*>(nDuelID, pDuel));
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception Detected!", __FUNCTION__);
	}
	m_Lock.Leave();
}

void CDuelSystem::TimerExpired()
{
	guard;
	if(m_Enabled)
	{
		m_Lock.Enter();
		vector<int> expiredDuels;
		DuelMap::iterator Iter = m_Duels.begin();
		for(;Iter!=m_Duels.end();Iter++)
		{
			Iter->second->TimerExpired();
			if(Iter->second->State == CDuel::END)
			{
				expiredDuels.push_back(Iter->first);
			}
		}
		for(UINT n=0;n<expiredDuels.size();n++)
		{
			DuelMap::iterator it = m_Duels.find(expiredDuels[n]);
			if(it != m_Duels.end())
			{
				delete it->second;
				m_Duels.erase(it);
			}
		}
		m_Lock.Leave();
	}
	unguard;
}
bool CDuelSystem::Withdraw(User *pUser)
{
	guard;

	if(pUser->ValidUser())
	{
		int nDuelID = pUser->pSD->DuelInfo.DuelID;
		if(nDuelID)
		{
			CDuel *pDuel = 0;
			m_Lock.Enter();
			DuelMap::iterator Iter = m_Duels.find(nDuelID);
			if(Iter!=m_Duels.end())
			{
				pDuel = Iter->second;
				m_Duels.erase(Iter);
			}
			m_Lock.Leave();
			pDuel->Withdraw(pUser);
			delete pDuel;
		}
	}
	unguard;
	return false;
}