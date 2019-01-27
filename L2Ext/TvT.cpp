#include "stdafx.h"
#include "TvT.h"
#include "UserDb.h"
#include "Door.h"
#include "CNpc.h"
#include "PlayerAction.h"
#include "TerritoryData.h"

using namespace TvT;

CTvT g_TvT;

CTvT::CTvT() : m_lpEvent(0), m_enabled(FALSE)
{

}

CTvT::~CTvT()
{

}

void CTvT::Init()
{
	Lock();
	if(!m_lpEvent)
	{
		m_tvtInfo.clear();
		m_enabled = FALSE;
		LoadINI();
		if(m_enabled)
		{
			g_Log.Add(CLog::Blue, "[%S] Feature is enabled, loaded [%d] events", L"TvT", m_tvtInfo.size());
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%S] Cannot Init tvt at the moment - please wait till it ends!", L"TvT");
	}
	Unlock();
}

void CTvT::LoadINI()
{
	TCHAR g_TvTFile[260];
	GetCurrentDirectory(MAX_PATH,g_TvTFile);
	lstrcat(g_TvTFile, TEXT("\\TvT.ini"));
	const TCHAR* section = _T("TvTSystem");
	m_enabled = GetPrivateProfileInt(section, _T("Enabled"), FALSE, g_TvTFile);
	INT32 count = GetPrivateProfileInt(section, _T("TvTEventCount"), 0, g_TvTFile);
	if(count > 0)
	{
		for(int n=1;n<=count;n++)
		{
			tstringstream sectionEx;
			sectionEx << "TvT_" << n ;
			TvT::TvTInfo* pInfo = new TvT::TvTInfo();
			pInfo->enabled = GetPrivateProfileInt(sectionEx.str().c_str(), _T("Enabled"), FALSE, g_TvTFile);
			TCHAR sTemp[0x8000];
			memset(sTemp, 0, sizeof(sTemp));
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("Type"), 0, sTemp, 8192, g_TvTFile))
			{
				tstring type = sTemp;
				if(type == _T("LTS"))
				{
					pInfo->type = TvT::TvTLastTeamStanding;
				}else if(type == _T("DM"))
				{
					pInfo->type = TvT::TvTDeathMatch;
				}else
				{
					pInfo->type = TvT::TvTNone;
				}
			}
			UINT dayFlag = GetPrivateProfileInt(sectionEx.str().c_str(), _T("DayFlag"), 0, g_TvTFile);
			for(UINT m = 1; m < 8; m++)
			{
				if((dayFlag % 10) == 1)
				{
					pInfo->dayFlag[m-1] = true;
				}else
				{
					pInfo->dayFlag[m-1] = false;
				}
				dayFlag /= 10;
			}
			pInfo->startHour = GetPrivateProfileInt(sectionEx.str().c_str(), _T("StartHour"), 14, g_TvTFile);
			pInfo->startMinute = GetPrivateProfileInt(sectionEx.str().c_str(), _T("StartMinute"), 30, g_TvTFile);
			pInfo->registerTime = GetPrivateProfileInt(sectionEx.str().c_str(), _T("RegisterTime"), 300, g_TvTFile);
			pInfo->prepareTime = GetPrivateProfileInt(sectionEx.str().c_str(), _T("PrepareTime"), 50, g_TvTFile);
			pInfo->countDownTime = GetPrivateProfileInt(sectionEx.str().c_str(), _T("CountDownTime"), 30, g_TvTFile);
			pInfo->fightTime = GetPrivateProfileInt(sectionEx.str().c_str(), _T("FightTime"), 300, g_TvTFile);
			pInfo->finishTime = GetPrivateProfileInt(sectionEx.str().c_str(), _T("FinishTime"), 15, g_TvTFile);
			pInfo->respawnTime = GetPrivateProfileInt(sectionEx.str().c_str(), _T("RespawnTime"), 15, g_TvTFile);
			pInfo->dispelAbnormal = GetPrivateProfileInt(sectionEx.str().c_str(), _T("DispelAbnormal"), 0, g_TvTFile);
			pInfo->rewardId = GetPrivateProfileInt(sectionEx.str().c_str(), _T("RewardId"), 0, g_TvTFile);
			pInfo->rewardCount = GetPrivateProfileInt(sectionEx.str().c_str(), _T("RewardCount"), 0, g_TvTFile);

			pInfo->requiredItemId = GetPrivateProfileInt(sectionEx.str().c_str(), _T("RequiredItemId"), 0, g_TvTFile);
			pInfo->requiredItemCount = GetPrivateProfileInt(sectionEx.str().c_str(), _T("RequiredItemCount"), 0, g_TvTFile);

			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("BlueTeamLocation"), 0, sTemp, 8192, g_TvTFile))
			{
				tstring pos = sTemp;
				pos = Utils::ReplaceString(pos, _T("/"), _T(" "), true);
				tstringstream sstr;
				sstr << pos;
				sstr >> pInfo->bluePos.x >> pInfo->bluePos.y >> pInfo->bluePos.z;
			}else
			{
				pInfo->bluePos.x = 0;
				pInfo->bluePos.y = 0;
				pInfo->bluePos.z = 0;
			}
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("RedTeamLocation"), 0, sTemp, 8192, g_TvTFile))
			{
				tstring pos = sTemp;
				pos = Utils::ReplaceString(pos, _T("/"), _T(" "), true);
				tstringstream sstr;
				sstr << pos;
				sstr >> pInfo->redPos.x >> pInfo->redPos.y >> pInfo->redPos.z;
			}else
			{
				pInfo->redPos.x = 0;
				pInfo->redPos.y = 0;
				pInfo->redPos.z = 0;
			}
			pInfo->randomizeTeam = GetPrivateProfileInt(sectionEx.str().c_str(), _T("RandomizeTeam"), 0, g_TvTFile);
			pInfo->friendlyFire = GetPrivateProfileInt(sectionEx.str().c_str(), _T("FriendlyFire"), 0, g_TvTFile);
			pInfo->checkHwid = GetPrivateProfileInt(sectionEx.str().c_str(), _T("CheckHWID"), 0, g_TvTFile);
			pInfo->minPlayers = GetPrivateProfileInt(sectionEx.str().c_str(), _T("MinPlayers"), 0, g_TvTFile);
			pInfo->maxPlayers = GetPrivateProfileInt(sectionEx.str().c_str(), _T("MaxPlayers"), 0, g_TvTFile);
			pInfo->minLevel = GetPrivateProfileInt(sectionEx.str().c_str(), _T("MinLevel"), 0, g_TvTFile);
			pInfo->maxLevel = GetPrivateProfileInt(sectionEx.str().c_str(), _T("MaxLevel"), 80, g_TvTFile);
			pInfo->skillPchId = GetPrivateProfileInt(sectionEx.str().c_str(), _T("SkillPchId"), 0, g_TvTFile);
			pInfo->registerNpcClassId = GetPrivateProfileInt(sectionEx.str().c_str(), _T("RegisterNpcId"), 0, g_TvTFile);
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("RegisterNpcPos"), 0, sTemp, 8192, g_TvTFile))
			{
				tstring pos = sTemp;
				pos = Utils::ReplaceString(pos, _T("/"), _T(" "), true);
				tstringstream sstr;
				sstr << pos;
				sstr >> pInfo->registerNpcPos.x >> pInfo->registerNpcPos.y >> pInfo->registerNpcPos.z;
			}else
			{
				pInfo->kickOutPos.x = 0;
				pInfo->kickOutPos.y = 0;
				pInfo->kickOutPos.z = 0;
			}


			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("KickOutPos"), 0, sTemp, 8192, g_TvTFile))
			{
				tstring pos = sTemp;
				pos = Utils::ReplaceString(pos, _T("/"), _T(" "), true);
				tstringstream sstr;
				sstr << pos;
				sstr >> pInfo->kickOutPos.x >> pInfo->kickOutPos.y >> pInfo->kickOutPos.z;
			}else
			{
				pInfo->kickOutPos.x = 0;
				pInfo->kickOutPos.y = 0;
				pInfo->kickOutPos.z = 0;
			}

			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("FightZoneXRange"), 0, sTemp, 8192, g_TvTFile))
			{
				tstring pos = sTemp;
				tstringstream sstr;
				sstr << pos;
				sstr >> pInfo->fightZoneX[0] >> pInfo->fightZoneX[1];
			}else
			{
				pInfo->fightZoneX[0] = 0;
				pInfo->fightZoneX[1] = 0;
			}

			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("FightZoneYRange"), 0, sTemp, 8192, g_TvTFile))
			{
				tstring pos = sTemp;
				tstringstream sstr;
				sstr << pos;
				sstr >> pInfo->fightZoneY[0] >> pInfo->fightZoneY[1];
			}else
			{
				pInfo->fightZoneY[0] = 0;
				pInfo->fightZoneY[1] = 0;
			}

			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("DoorName"), 0, sTemp, 8192, g_TvTFile))
			{
				tstringstream sstr;
				sstr << sTemp;
				wstring doorName;
				while(sstr >> doorName)
				{
					pInfo->doorList.push_back(doorName);
				}
			}
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("RegistrationStartMessage1"), 0, sTemp, 8192, g_TvTFile))
			{
				pInfo->registrationStartMsg1 = sTemp;
			}
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("RegistrationStartMessage2"), 0, sTemp, 8192, g_TvTFile))
			{
				pInfo->registrationStartMsg2 = sTemp;
			}
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("RegistrationRemindMessage"), 0, sTemp, 8192, g_TvTFile))
			{
				pInfo->registrationRemindMsg = sTemp;
			}
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("RegistrationEndMessage"), 0, sTemp, 8192, g_TvTFile))
			{
				pInfo->registrationEndMsg = sTemp;
			}
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("RegistratrionNontEnoughParticipants"), 0, sTemp, 8192, g_TvTFile))
			{
				pInfo->registrationNotEnoughParticipantsMsg = sTemp;
			}
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("PreparationStartMessage"), 0, sTemp, 8192, g_TvTFile))
			{
				pInfo->preparationStartMsg = sTemp;
			}
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("PreparationRemindMessage"), 0, sTemp, 8192, g_TvTFile))
			{
				pInfo->preparationRemindMsg = sTemp;
			}
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("FightStartRemindMessage"), 0, sTemp, 8192, g_TvTFile))
			{
				pInfo->fightStartRemindMsg = sTemp;
			}
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("FightEndRemindMessage"), 0, sTemp, 8192, g_TvTFile))
			{
				pInfo->fightEndRemindMsg = sTemp;
			}
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("DrawMessage"), 0, sTemp, 8192, g_TvTFile))
			{
				pInfo->endMsg[0] = sTemp;
			}
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("BlueWinMessage"), 0, sTemp, 8192, g_TvTFile))
			{
				pInfo->endMsg[1] = sTemp;
			}
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("RedWinMessage"), 0, sTemp, 8192, g_TvTFile))
			{
				pInfo->endMsg[2] = sTemp;
			}if(GetPrivateProfileString(sectionEx.str().c_str(), _T("TeleportBackRemindMessage"), 0, sTemp, 8192, g_TvTFile))
			{
				pInfo->teleportBackRemindMsg = sTemp;
			}
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("EndMessage"), 0, sTemp, 8192, g_TvTFile))
			{
				pInfo->endEventMsg = sTemp;
			}
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("FightStartMessage"), 0, sTemp, 8192, g_TvTFile))
			{
				pInfo->fightStartMessage = sTemp;
			}

			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("BlockedSkillList"), 0, sTemp, 0x8000, g_TvTFile))
			{
				tstringstream sstr;
				sstr << sTemp;
				INT32 skillId = 0;
				while(sstr >> skillId)
				{
					pInfo->blockedSkill.push_back(skillId);
				}
			}
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("BlockedItemList"), 0, sTemp, 0x8000, g_TvTFile))
			{
				tstringstream sstr;
				sstr << sTemp;
				INT32 itemId = 0;
				while(sstr >> itemId)
				{
					pInfo->blockedItem.push_back(itemId);
				}
			}

			pInfo->defaultMageBuffs.clear();
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("DefaultMageBuffList"), 0, sTemp, 0x8000, g_TvTFile))
			{
				tstringstream sstr;
				sstr << sTemp;
				UINT skillId = 0;
				while(sstr >> skillId)
				{
					pInfo->defaultMageBuffs.push_back(skillId);
				}
			}
			
			pInfo->defaultFighterBuffs.clear();
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("DefaultFighterBuffList"), 0, sTemp, 0x8000, g_TvTFile))
			{
				tstringstream sstr;
				sstr << sTemp;
				UINT skillId = 0;
				while(sstr >> skillId)
				{
					pInfo->defaultFighterBuffs.push_back(skillId);
				}
			}
			memset(pInfo->blockedCrystalType, 0, sizeof(pInfo->blockedCrystalType));
			if(GetPrivateProfileString(sectionEx.str().c_str(), _T("BlockedCrystalTypeList"), 0, sTemp, 0x8000, g_TvTFile))
			{
				tstringstream sstr;
				sstr << sTemp;
				UINT crystalType = 0;
				while(sstr >> crystalType)
				{
					if(crystalType < 10)
					{
						pInfo->blockedCrystalType[crystalType] = true;
					}else
					{
						g_Log.Add(CLog::Error, "[%s] CrystalType overflow[%d]!", __FUNCTION__, crystalType);
					}
				}
			}

			pInfo->blockAugmentation = GetPrivateProfileInt(sectionEx.str().c_str(), _T("BlockAugmentation"), 0, g_TvTFile);
			pInfo->blockAttribute = GetPrivateProfileInt(sectionEx.str().c_str(), _T("BlockAttribute"), 0, g_TvTFile);
			pInfo->maxArmorEnchant = GetPrivateProfileInt(sectionEx.str().c_str(), _T("MaxArmorEnchant"), 0, g_TvTFile);
			pInfo->maxWeaponEnchant = GetPrivateProfileInt(sectionEx.str().c_str(), _T("MaxWeaponEnchant"), 0, g_TvTFile);

			m_tvtInfo.push_back(pInfo);
		}
	}
}

void CTvT::TimerExpired()
{
	guard;
	if(m_enabled)
	{
		Lock();
		if(m_lpEvent)
		{
			time_t currentTime = time(NULL);

			switch(m_lpEvent->m_state)
			{
			case TvT::StateNone:
				{
					m_lpEvent->Init();
				};
			case TvT::StateRegistration:
				{
					time_t diff = m_lpEvent->m_stateTime + m_lpEvent->m_lpInfo->registerTime;
					diff -= currentTime;
					if(diff > 0)
					{
						//broadcast message when it ends
						if(m_lpEvent->m_lpInfo->registrationRemindMsg.size() > 0)
						{
							if(diff == 60)
							{
								TCHAR remainSec[16];
								_itot_s(static_cast<int>(diff), remainSec, 16, 10);
								wstring announce = Utils::ReplaceString(m_lpEvent->m_lpInfo->registrationRemindMsg, _T("[param]"), remainSec);
								Utils::BroadcastToAllUser_Announce(announce.c_str());
							}else if(diff == 30)
							{
								TCHAR remainSec[16];
								_itot_s(static_cast<int>(diff), remainSec, 16, 10);
								wstring announce = Utils::ReplaceString(m_lpEvent->m_lpInfo->registrationRemindMsg, _T("[param]"), remainSec);
								Utils::BroadcastToAllUser_Announce(announce.c_str());
							}else if(diff == 10)
							{
								TCHAR remainSec[16];
								_itot_s(static_cast<int>(diff), remainSec, 16, 10);
								wstring announce = Utils::ReplaceString(m_lpEvent->m_lpInfo->registrationRemindMsg, _T("[param]"), remainSec);
								Utils::BroadcastToAllUser_Announce(announce.c_str());
							}
						}

					}else
					{
						m_lpEvent->m_stateTime = time(NULL);
						if(m_lpEvent->m_lpInfo->registrationEndMsg.size() > 0)
						{
							Utils::BroadcastToAllUser_Announce(m_lpEvent->m_lpInfo->registrationEndMsg.c_str());
						}

						//switch to next state if 
						if(m_lpEvent->m_lpInfo->minPlayers > 0 && m_lpEvent->m_lpInfo->minPlayers > m_lpEvent->m_users.size())
						{
							if(m_lpEvent->m_lpInfo->registrationNotEnoughParticipantsMsg.size() > 0)
							{
								Utils::BroadcastToAllUser_Announce(m_lpEvent->m_lpInfo->registrationNotEnoughParticipantsMsg.c_str());
							}
							m_lpEvent->m_state = TvT::StateDelete;
							break;
						}

						m_lpEvent->m_state = TvT::StatePreparation;
						if(m_lpEvent->m_lpInfo->preparationStartMsg.size() > 0)
						{
							Utils::BroadcastToAllUser_Announce(m_lpEvent->m_lpInfo->preparationStartMsg.c_str());
						}
						if(m_lpEvent->m_lpInfo->doorList.size() > 0)
						{
							//close doors
							for(list<wstring>::iterator Iter = m_lpEvent->m_lpInfo->doorList.begin(); Iter!= m_lpEvent->m_lpInfo->doorList.end();Iter++)
							{
								CDoor *pDoor = g_DoorDB.GetDoor(Iter->c_str());
								if(pDoor)
								{
									pDoor->Close();
								}
							}
						}

						if(m_lpEvent->m_lpInfo->kickOutPos.x != 0 || m_lpEvent->m_lpInfo->kickOutPos.y != 0)
						{
							g_UserDB.KickOutFromRange(m_lpEvent->m_lpInfo->kickOutPos, m_lpEvent->m_lpInfo->fightZoneX[0], m_lpEvent->m_lpInfo->fightZoneX[1], m_lpEvent->m_lpInfo->fightZoneY[0], m_lpEvent->m_lpInfo->fightZoneY[1] );
						}
					}
					break;
				}
			case TvT::StatePreparation:
				{
					time_t diff = m_lpEvent->m_stateTime + m_lpEvent->m_lpInfo->prepareTime;
					diff -= currentTime;
					if(diff > 0)
					{
						if(m_lpEvent->m_lpInfo->preparationRemindMsg.size() > 0)
						{
							if(diff == 30)
							{
								TCHAR remainSec[16];
								_itot_s(static_cast<int>(diff), remainSec, 16, 10);
								wstring announce = Utils::ReplaceString(m_lpEvent->m_lpInfo->preparationRemindMsg, _T("[param]"), remainSec);
								m_lpEvent->Broadcast("cddSS", 0x4A, 0, 18, L"", announce.c_str());
							}else if(diff == 10)
							{
								TCHAR remainSec[16];
								_itot_s(static_cast<int>(diff), remainSec, 16, 10);
								wstring announce = Utils::ReplaceString(m_lpEvent->m_lpInfo->preparationRemindMsg, _T("[param]"), remainSec);
								m_lpEvent->Broadcast("cddSS", 0x4A, 0, 18, L"", announce.c_str());
							}
							else if(diff == 5)
							{
								TCHAR remainSec[16];
								_itot_s(static_cast<int>(diff), remainSec, 16, 10);
								wstring announce = Utils::ReplaceString(m_lpEvent->m_lpInfo->preparationRemindMsg, _T("[param]"), remainSec);
								m_lpEvent->Broadcast("cddSS", 0x4A, 0, 18, L"", announce.c_str());
							}
						}
					}else
					{
						m_lpEvent->m_stateTime = time(NULL);
						m_lpEvent->m_state = TvT::StateCountDown;

						vector<UINT> toDelete;
						for(map<UINT, User*>::iterator Iter = m_lpEvent->m_users.begin(); Iter!=m_lpEvent->m_users.end();Iter++)
						{
							User *pUser = Iter->second;
							if(pUser->ValidUser())
							{
								if(!pUser->pSD->nAlive)
								{
									//2578	1	a,Only alive players can participe in TvT event.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
									pUser->SendSystemMessage(2578);
									toDelete.push_back(Iter->first);
									continue;
								}
								if(pUser->pSD->pExData->territoryData.territoryId[BLOCK_TVT_ZONE] != 0)
								{
									//2579	1	a,You cannot participe in TvT event while being in current location!.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
									pUser->SendSystemMessage(2579);
									toDelete.push_back(Iter->first);
									continue;
								}
								if(m_lpEvent->m_lpInfo->dispelAbnormal)
								{
									pUser->DispelAllByGM();
									if(CSummon *pSummon = pUser->GetSummonOrPet())
									{
										pSummon->DispelAllByGM();
									}
								}
								ValidateItems(pUser);
								pUser->pSD->teamType = pUser->pED->tvtUser.team;
								pUser->pED->tvtUser.status = TvT::UserPreparing;
								pUser->SetUserInfoChanged();
								pUser->SetCharInfoChanged();
							}else
							{
								toDelete.push_back(Iter->first);
							}
						}

						for(INT32 n=0;n<toDelete.size();n++)
						{
							map<UINT, User*>::iterator Iter = m_lpEvent->m_users.find(toDelete[n]);
							if(Iter!=m_lpEvent->m_users.end())
							{
								User *pUser = Iter->second;
								if(pUser->ValidUser())
								{
									pUser->pED->tvtUser.Clear();
									pUser->pSD->teamType = 0;
								}
								m_lpEvent->m_users.erase(Iter);
							}
						}

						//randomize team if needed
						if(m_lpEvent->m_lpInfo->randomizeTeam)
						{
							m_lpEvent->RandomizeTeams();
						}

						if(m_lpEvent->m_lpInfo->bluePos.x != 0 || m_lpEvent->m_lpInfo->bluePos.y != 0)
						{
							//port blue
							for(map<UINT, User*>::iterator Iter = m_lpEvent->m_users.begin(); Iter!=m_lpEvent->m_users.end();Iter++)
							{
								User *pUser = Iter->second;
								if(pUser->ValidUser() && pUser->pED->tvtUser.team == TvT::TeamBlue)
								{
									pUser->pED->tvtUser.orgPos.x = (INT32)pUser->pSD->Pos.x;
									pUser->pED->tvtUser.orgPos.y = (INT32)pUser->pSD->Pos.y;
									pUser->pED->tvtUser.orgPos.z = (INT32)pUser->pSD->Pos.z;
									pUser->pED->tvtUser.respawnPos.x = m_lpEvent->m_lpInfo->bluePos.x;
									pUser->pED->tvtUser.respawnPos.y = m_lpEvent->m_lpInfo->bluePos.y;
									pUser->pED->tvtUser.respawnPos.z = m_lpEvent->m_lpInfo->bluePos.z;
									pUser->StopMove();
									PlayerAction::OnTeleportToLocation(pUser, m_lpEvent->m_lpInfo->bluePos.x, m_lpEvent->m_lpInfo->bluePos.y, m_lpEvent->m_lpInfo->bluePos.z, false);
								}
							}
						}
						if(m_lpEvent->m_lpInfo->redPos.x != 0 || m_lpEvent->m_lpInfo->redPos.y != 0)
						{
							//port red
							for(map<UINT, User*>::iterator Iter = m_lpEvent->m_users.begin(); Iter!=m_lpEvent->m_users.end();Iter++)
							{
								User *pUser = Iter->second;
								if(pUser->ValidUser() && pUser->pED->tvtUser.team == TvT::TeamRed)
								{
									pUser->pED->tvtUser.orgPos.x = (INT32)pUser->pSD->Pos.x;
									pUser->pED->tvtUser.orgPos.y = (INT32)pUser->pSD->Pos.y;
									pUser->pED->tvtUser.orgPos.z = (INT32)pUser->pSD->Pos.z;
									pUser->pED->tvtUser.respawnPos.x = m_lpEvent->m_lpInfo->redPos.x;
									pUser->pED->tvtUser.respawnPos.y = m_lpEvent->m_lpInfo->redPos.y;
									pUser->pED->tvtUser.respawnPos.z = m_lpEvent->m_lpInfo->redPos.z;
									pUser->StopMove();
									PlayerAction::OnTeleportToLocation(pUser, m_lpEvent->m_lpInfo->redPos.x, m_lpEvent->m_lpInfo->redPos.y, m_lpEvent->m_lpInfo->redPos.z, false);
								}
							}
						}

					}
					break;
				}
			case TvT::StateCountDown:
				{
					time_t diff = m_lpEvent->m_stateTime + m_lpEvent->m_lpInfo->countDownTime;
					diff -= currentTime;
					if(diff > 0)
					{
						if(m_lpEvent->m_lpInfo->fightStartRemindMsg.size() > 0)
						{
							if(diff == 30)
							{
								TCHAR remainSec[16];
								_itot_s(static_cast<int>(diff), remainSec, 16, 10);
								wstring announce = Utils::ReplaceString(m_lpEvent->m_lpInfo->fightStartRemindMsg, _T("[param]"), remainSec);
								m_lpEvent->Broadcast("cddSS", 0x4A, 0, 18, L"", announce.c_str());
							}else if(diff == 10)
							{
								TCHAR remainSec[16];
								_itot_s(static_cast<int>(diff), remainSec, 16, 10);
								wstring announce = Utils::ReplaceString(m_lpEvent->m_lpInfo->fightStartRemindMsg, _T("[param]"), remainSec);
								m_lpEvent->Broadcast("cddSS", 0x4A, 0, 18, L"", announce.c_str());
							}
							else if(diff < 6)
							{
								TCHAR remainSec[16];
								_itot_s(static_cast<int>(diff), remainSec, 16, 10);
								wstring announce = Utils::ReplaceString(m_lpEvent->m_lpInfo->fightStartRemindMsg, _T("[param]"), remainSec);
								m_lpEvent->Broadcast("cddSS", 0x4A, 0, 18, L"", announce.c_str());
							}
						}
					}else
					{
						m_lpEvent->m_stateTime = time(NULL);
						m_lpEvent->m_state = TvT::StateFight;
						for(map<UINT, User*>::iterator Iter = m_lpEvent->m_users.begin(); Iter!=m_lpEvent->m_users.end();Iter++)
						{
							User *pUser = Iter->second;
							if(pUser->ValidUser() && pUser->pSD->nAlive)
							{
								pUser->pED->tvtUser.status = TvT::UserFighting;
								TvTInfo *pInfo = m_lpEvent->m_lpInfo;
								if(pInfo)
								{
									if(CCategoryDataDB::IsInCategory(pUser->pSD->nClass, CCategoryDataDB::MAGE_GROUP))
									{
										for(UINT n=0;n<pInfo->defaultMageBuffs.size();n++)
										{
											if(CSkillInfo *pSI = g_SkillDB.GetSkillInfo(pInfo->defaultMageBuffs[n].skillId, pInfo->defaultMageBuffs[n].level))
											{
												pSI->ActivateSkill(pUser, pUser);
											}
										}
									}else
									{
										for(UINT n=0;n<pInfo->defaultFighterBuffs.size();n++)
										{
											if(CSkillInfo *pSI = g_SkillDB.GetSkillInfo(pInfo->defaultFighterBuffs[n].skillId, pInfo->defaultFighterBuffs[n].level))
											{
												pSI->ActivateSkill(pUser, pUser);
											}
										}
									}
								}
							}
						}
						m_lpEvent->Broadcast("cddSS", 0x4A, 0, 18, L"", m_lpEvent->m_lpInfo->fightStartMessage.c_str());
					}
					break;
				}
			case TvT::StateFight:
				{
					time_t diff = m_lpEvent->m_stateTime + m_lpEvent->m_lpInfo->fightTime;
					diff -= currentTime;
					if(diff > 0)
					{
						if(m_lpEvent->m_lpInfo->fightEndRemindMsg.size() > 0)
						{
							if(diff == 30)
							{
								TCHAR remainSec[16];
								_itot_s(static_cast<int>(diff), remainSec, 16, 10);
								wstring announce = Utils::ReplaceString(m_lpEvent->m_lpInfo->fightEndRemindMsg, _T("[param]"), remainSec);
								m_lpEvent->Broadcast("cddSS", 0x4A, 0, 18, L"", announce.c_str());
							}else if(diff == 10)
							{
								TCHAR remainSec[16];
								_itot_s(static_cast<int>(diff), remainSec, 16, 10);
								wstring announce = Utils::ReplaceString(m_lpEvent->m_lpInfo->fightEndRemindMsg, _T("[param]"), remainSec);
								m_lpEvent->Broadcast("cddSS", 0x4A, 0, 18, L"", announce.c_str());
							}
							else if(diff < 6)
							{
								TCHAR remainSec[16];
								_itot_s(static_cast<int>(diff), remainSec, 16, 10);
								wstring announce = Utils::ReplaceString(m_lpEvent->m_lpInfo->fightEndRemindMsg, _T("[param]"), remainSec);
								m_lpEvent->Broadcast("cddSS", 0x4A, 0, 18, L"", announce.c_str());
							}
							if(m_lpEvent->ValidateWinner())
							{
								m_lpEvent->OnFinish();
							}
						}
					}else
					{
						m_lpEvent->ValidateWinner(true);
						m_lpEvent->OnFinish();
					}
					break;
				}
			case TvT::StateFinish:
				{
					time_t diff = m_lpEvent->m_stateTime + m_lpEvent->m_lpInfo->finishTime;
					diff -= currentTime;
					if(diff > 0)
					{
						if(m_lpEvent->m_lpInfo->teleportBackRemindMsg.size() > 0)
						{
							if(diff == 10)
							{
								TCHAR remainSec[16];
								_itot_s(static_cast<int>(diff), remainSec, 16, 10);
								wstring announce = Utils::ReplaceString(m_lpEvent->m_lpInfo->teleportBackRemindMsg, _T("[param]"), remainSec);
								m_lpEvent->Broadcast("cddSS", 0x4A, 0, 18, L"", announce.c_str());
							}else if(diff < 6)
							{
								TCHAR remainSec[16];
								_itot_s(static_cast<int>(diff), remainSec, 16, 10);
								wstring announce = Utils::ReplaceString(m_lpEvent->m_lpInfo->teleportBackRemindMsg, _T("[param]"), remainSec);
								m_lpEvent->Broadcast("cddSS", 0x4A, 0, 18, L"", announce.c_str());
							}
						}
					}else
					{
						m_lpEvent->m_stateTime = time(NULL);
						m_lpEvent->m_state = TvT::StateDelete;
					}
					break;
				}
			case TvT::StateDelete:
				{
					m_lpEvent->OnDelete();
					delete m_lpEvent;
					m_lpEvent = 0;
					break;
				};
			};
		}else
		{
			tm ti; time_t rawtime;
			time ( &rawtime );
			localtime_s(&ti, &rawtime );

			for(list<TvT::TvTInfo*>::iterator Iter = m_tvtInfo.begin();Iter!=m_tvtInfo.end();Iter++)
			{
				TvT::TvTInfo *pInfo = (*Iter);
				if(pInfo)
				{
					if(pInfo->enabled)
					{
						if(pInfo->dayFlag[ti.tm_wday])
						{
							if(pInfo->startHour == ti.tm_hour && pInfo->startMinute == ti.tm_min)
							{
								m_lpEvent = new CMatch(pInfo);
								break;
							}
						}
					}
				}
			}
		}
		Unlock();
	}
	unguard;
}

void CTvT::Unregister(User *pUser)
{
	if(!m_enabled)
		return;

	if(pUser->ValidUser())
	{
		if(pUser->pED->tvtUser.status != TvT::UserNone)
		{
			Lock();
			if(m_lpEvent)
			{
				if(m_lpEvent->m_state == TvT::StateRegistration)
				{
					map<UINT, User*>::iterator Iter = m_lpEvent->m_users.find(pUser->nObjectID);
					if(Iter!= m_lpEvent->m_users.end())
					{
						m_lpEvent->m_users.erase(Iter);
						//2580	1	a,You've been unregistered from TvT event.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->pSocket->SendSystemMessage(2580);
						if(m_lpEvent->m_lpInfo->requiredItemId > 0)
						{
							pUser->AddItemToInventory(m_lpEvent->m_lpInfo->requiredItemId, m_lpEvent->m_lpInfo->requiredItemCount);
						}
					}
				}else
				{
					//2581	1	a,You cannot unregister from TvT event at the moment.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pUser->pSocket->SendSystemMessage(2581);
				}
			}
			Unlock();
			pUser->pED->tvtUser.Clear();
			pUser->pSD->teamType = 0;
		}
	}
}

void CTvT::Register(User *pUser, int team)
{
	CTL;

	if(!m_enabled)
		return;

	if(pUser->pED->tvtUser.status == TvT::UserNone)
	{
		if(pUser->pSD->nCursedOwner)
		{
			//2582	1	a,The Cursed Weapon owner cannot participe in TvT event.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->pSocket->SendSystemMessage(2582);
			return;
		}
		if(pUser->pSD->nAlive == 0)
		{
			//2578	1	a,Only alive players can participe in TvT event.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->SendSystemMessage(2578);
			return;
		}
		if(pUser->pSD->pExData->territoryData.territoryId[BLOCK_TVT_ZONE] != 0)
		{
			//2579	1	a,You cannot participe in TvT event while being in current location!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->pSocket->SendSystemMessage(2579);
			return;
		}
		Lock();
		if(m_lpEvent)
		{
			if(m_lpEvent->m_state == TvT::StateRegistration)
			{
				if(pUser->pSD->nLevel >= m_lpEvent->m_lpInfo->minLevel)
				{
					if(pUser->pSD->nLevel <= m_lpEvent->m_lpInfo->maxLevel)
					{
						if(m_lpEvent->m_users.size() < m_lpEvent->m_lpInfo->maxPlayers)
						{
							if(team == 0)
							{
								team = 1 + g_Random.RandInt(1);
							}
							if(m_lpEvent->m_lpInfo->checkHwid)
							{
								UINT ip = pUser->pSocket->addr.S_un.S_addr;
								for(map<UINT, User*>::iterator Iter = m_lpEvent->m_users.begin();Iter!=m_lpEvent->m_users.end();Iter++)
								{
									if(User *pOther = Iter->second->GetUser())
									{
										if(pOther->pSocket->addr.S_un.S_addr == ip && !memcmp(pOther->pSocket->pED->longHWID.hash, pUser->pSocket->pED->longHWID.hash, 32))
										{
											Unlock();
											//2583	1	a,You cannot register more than one character in TvT event.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
											pUser->SendSystemMessage(2583); 
											return;
										}
									}
								}
							}

							if(m_lpEvent->m_lpInfo->requiredItemId == 0 || pUser->inventory.HaveItemByClassID(m_lpEvent->m_lpInfo->requiredItemId, m_lpEvent->m_lpInfo->requiredItemCount))
							{	
								if(m_lpEvent->m_lpInfo->requiredItemId > 0)
								{
									pUser->DeleteItemInInventory(m_lpEvent->m_lpInfo->requiredItemId, m_lpEvent->m_lpInfo->requiredItemCount);
								}

								if(team == 1 || team == 2)
								{
									pUser->pED->tvtUser.status = TvT::UserRegistered;
									if(team == 1)
										pUser->pED->tvtUser.team = TvT::TeamBlue;
									else
										pUser->pED->tvtUser.team = TvT::TeamRed;
									m_lpEvent->m_users.insert(pair<UINT, User*>(pUser->nObjectID, pUser));
									pUser->pED->tvtUser.friendlyFire = m_lpEvent->m_lpInfo->friendlyFire;
									//2584	1	a,You've been registered in TvT event.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
									pUser->pSocket->SendSystemMessage(2584);
								}else
								{
									g_Log.Add(CLog::Error, "[%s] Invalid team[%d] - user[%S]", __FUNCTION__, team, pUser->pSD->wszName);
								}
							}else
							{
								if(m_lpEvent->m_lpInfo->requiredItemId > 0)
								{
									//2585	1	a,You don't have enough item(s).\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
									pUser->pSocket->SendSystemMessage(2585);
								}
							}
						}else
						{
							//2586	1	a,There are no empty slots in TvT event, please come back later..\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pUser->pSocket->SendSystemMessage(2586);
						}
					}else
					{
						//2587	1	a,Your level is too high to participe in TvT event.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->pSocket->SendSystemMessage(2587);
					}
				}else
				{
					//2588	1	a,Your level is too low to participe in TvT event.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pUser->pSocket->SendSystemMessage(2588);
				}
			}else
			{
				//2577	1	a,You cannot participe in TvT event at the moment.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2577);
			}
		}else
		{
			//2577	1	a,You cannot participe in TvT event at the moment.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->pSocket->SendSystemMessage(2577);
		}
		Unlock();
	}else
	{
		//2589	1	a,You already are participing in TvT event.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
		pUser->pSocket->SendSystemMessage(2589);
	}
}

void CTvT::OnDie(User *pUser, CCreature *pKiller)
{
	if(m_enabled)
	{
		if(pUser->ValidUser())
		{
			if(pUser->pED->tvtUser.status == TvT::UserFighting)
			{
				Lock();
				if(m_lpEvent->m_lpInfo->type == TvT::TvTLastTeamStanding)
				{
					pUser->pED->tvtUser.status = TvT::UserFinishing;
					if(m_lpEvent->ValidateWinner())
					{
						m_lpEvent->OnFinish();
					}
				}else if(m_lpEvent->m_lpInfo->type == TvT::TvTDeathMatch)
				{
					if(User *pUK = pKiller->GetUser())
					{
						if(pUK->pED->tvtUser.status == TvT::UserFighting)
						{
							if(pUser->pED->tvtUser.team == TvT::TeamBlue && pUK->pED->tvtUser.team == TvT::TeamRed)
							{
								m_lpEvent->m_killCountRed++;
							}else if(pUser->pED->tvtUser.team == TvT::TeamRed && pUK->pED->tvtUser.team == TvT::TeamBlue)
							{
								m_lpEvent->m_killCountBlue++;
							}
						}
					}
					pUser->pED->tvtUser.respawnTick = m_lpEvent->m_lpInfo->respawnTime;
				}
				Unlock();
			}
		}
	}
}

void CTvT::OnExit(User *pUser)
{
	if(m_enabled)
	{
		if(pUser->ValidUser())
		{
			if(pUser->pED->tvtUser.status != TvT::UserNone)
			{
				Lock();
				if(m_lpEvent)
				{
					map<UINT, User*>::iterator Iter = m_lpEvent->m_users.find(pUser->nObjectID);
					if(Iter!=m_lpEvent->m_users.end())
					{
						m_lpEvent->m_users.erase(Iter);
					}
					if(m_lpEvent->m_lpInfo->type == TvT::TvTLastTeamStanding)
					{
						if(m_lpEvent->ValidateWinner())
						{
							m_lpEvent->OnFinish();
						}
					}
				}
				Unlock();
				pUser->pED->tvtUser.Clear();
				pUser->pSD->teamType = 0;
			}
		}
	}
}

void CTvT::OnNpcSpawn(CNPC *pNpc)
{
	if(m_enabled)
	{
		if(pNpc)
		{
			Lock();
			if(m_lpEvent)
			{
				if(m_lpEvent->m_lpInfo->registerNpcClassId)
				{
					if(pNpc->pSD->nNpcClassID == m_lpEvent->m_lpInfo->registerNpcClassId)
					{
						if(m_lpEvent->m_managerServerId == 0)
						{
							m_lpEvent->m_managerServerId = pNpc->nObjectID;
						}
					}
				}
			}
			Unlock();
		}
	}
}

void CTvT::TimerExpired(User *pUser)
{
	guard;
	if(m_enabled && pUser->pED->tvtUser.status != TvT::UserNone)
	{
		if((pUser->pED->tvtUser.status == TvT::UserPreparing || pUser->pED->tvtUser.status == TvT::UserFinishing) && pUser->pSD->nKarma == 0)
		{
			if(m_lpEvent->m_lpInfo->fightZoneX[0] <= (INT32)pUser->pSD->Pos.x && m_lpEvent->m_lpInfo->fightZoneX[1] >= (INT32)pUser->pSD->Pos.x
				&& m_lpEvent->m_lpInfo->fightZoneY[0] <= (INT32)pUser->pSD->Pos.y && m_lpEvent->m_lpInfo->fightZoneY[1] >= (INT32)pUser->pSD->Pos.y)
			{
				pUser->inPeaceZone = true;
			}
		}
		if(pUser->pED->tvtUser.status == TvT::UserFighting)
		{
			//check loc
			if(m_lpEvent->m_lpInfo->fightZoneX[0] > (INT32)pUser->pSD->Pos.x || m_lpEvent->m_lpInfo->fightZoneX[1] < (INT32)pUser->pSD->Pos.x
				|| m_lpEvent->m_lpInfo->fightZoneY[0] > (INT32)pUser->pSD->Pos.y || m_lpEvent->m_lpInfo->fightZoneY[1] < (INT32)pUser->pSD->Pos.y)
			{
				if(pUser->pED->tvtUser.resetTimer == 0)
				{
					//2590	1	a,You're outside combat zone, you'll be expelled from TvT event in $s1 seconds.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					CSystemMessage msg(2590);
					msg.AddNumber(15);
					pUser->SendSystemMessage(&msg);
					pUser->pED->tvtUser.resetTimer = 15;
				}

				pUser->pED->tvtUser.resetTimer--;
				if(pUser->pED->tvtUser.resetTimer == 0)
				{
					//2591	1	a,You're outside combat zone, you've been expelled from TvT event.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pUser->pSocket->SendSystemMessage(2591);
					OnExit(pUser);
					unguard;
					return;
				}
			}else if(pUser->pED->tvtUser.resetTimer != 0)
			{
				pUser->pED->tvtUser.resetTimer = 0;
			}

			if(pUser->pED->tvtUser.respawnTick > 0)
			{
				if(!pUser->pSD->nAlive)
				{
					pUser->pED->tvtUser.respawnTick--;
					if(pUser->pED->tvtUser.respawnTick != 0)
					{
						CSystemMessage msg(2452);	//You'll be respawned in $s1 second(s)!
						msg.AddNumber(pUser->pED->tvtUser.respawnTick);
						pUser->pSocket->SendSystemMessage(&msg);
					}else
					{
						pUser->ResurrectedBy(pUser, 100);
						
						pUser->pSD->fHP = pUser->pSD->MaxHP;
						pUser->pSD->fCP = pUser->pSD->MaxCP;
						pUser->pSD->fMP = pUser->pSD->MaxMP;

						TvTInfo *pInfo = m_lpEvent->m_lpInfo;
						if(pInfo)
						{
							if(CCategoryDataDB::IsInCategory(pUser->pSD->nClass, CCategoryDataDB::MAGE_GROUP))
							{
								for(UINT n=0;n<pInfo->defaultMageBuffs.size();n++)
								{
									if(CSkillInfo *pSI = g_SkillDB.GetSkillInfo(pInfo->defaultMageBuffs[n].skillId, pInfo->defaultMageBuffs[n].level))
									{
										pSI->ActivateSkill(pUser, pUser);
									}
								}
							}else
							{
								for(UINT n=0;n<pInfo->defaultFighterBuffs.size();n++)
								{
									if(CSkillInfo *pSI = g_SkillDB.GetSkillInfo(pInfo->defaultFighterBuffs[n].skillId, pInfo->defaultFighterBuffs[n].level))
									{
										pSI->ActivateSkill(pUser, pUser);
									}
								}
							}
						}


						if(pUser->pED->tvtUser.respawnPos.x != 0 || pUser->pED->tvtUser.respawnPos.y != 0)
						{
							pUser->StopMove();
							PlayerAction::OnTeleportToLocation(pUser, pUser->pED->tvtUser.respawnPos.x, pUser->pED->tvtUser.respawnPos.y, pUser->pED->tvtUser.respawnPos.z, false);
						}
					}
				}else
				{
					pUser->pED->tvtUser.respawnTick = 0;
				}
			}
		}
	}
	unguard;
}

bool CTvT::CanUse(CItem *pItem)
{
	
	if(m_enabled && m_lpEvent)
	{
		if(m_lpEvent->m_state == TvT::StateCountDown || m_lpEvent->m_state == TvT::StateFight)
		{
			for(UINT n=0;n< m_lpEvent->m_lpInfo->blockedItem.size();n++)
			{
				if(m_lpEvent->m_lpInfo->blockedItem[n] == pItem->pSID->nItemID)
					return false;
			}

			if(pItem->pII->nCrystalType < 10)
			{
				if(m_lpEvent->m_lpInfo->blockedCrystalType[pItem->pII->nCrystalType])
					return false;
			}

			if(m_lpEvent->m_lpInfo->blockAugmentation && pItem->nAugmentationID)
				return false;

			if(pItem->pSID->nItemType == 0 && m_lpEvent->m_lpInfo->maxWeaponEnchant > 0)
			{
				if(pItem->pII->weaponType == WeaponDual)
				{
					if( pItem->pSID->nEnchantLevel > (m_lpEvent->m_lpInfo->maxWeaponEnchant + 1) )
					{
						return false;
					}
				}else
				{
					if(pItem->pSID->nEnchantLevel > m_lpEvent->m_lpInfo->maxWeaponEnchant)
					{
						return false;
					}
				}
			}else if(m_lpEvent->m_lpInfo->maxArmorEnchant > 0)
			{
				if(pItem->pSID->nSlotType == SlotOnePiece)
				{
					if( pItem->pSID->nEnchantLevel > (m_lpEvent->m_lpInfo->maxWeaponEnchant + 1) )
					{
						return false;
					}
				}else
				{
					if( pItem->pSID->nEnchantLevel > m_lpEvent->m_lpInfo->maxWeaponEnchant )
					{
						return false;
					}
				}
			}
		}
	}

	return true;
}

void CTvT::ValidateItems(User *pUser)
{
	
	if(pUser->ValidUser())
	{
		for(int n=0;n<18;n++)
		{
			INT32 index = pUser->pSD->nEquipment[n];
			if(index > 0)
			{
				CItem *pItem = pUser->inventory.GetItemByIndex(index);
				if(pItem)
				{
					if(!CanUse(pItem))
					{
						pUser->UnEquipItem(n);
						continue;
					}
				}
			}
		}
	}
}

bool CTvT::IsSkillBlocked(INT32 skillId)
{
	
	if(m_enabled && m_lpEvent)
	{
		if(m_lpEvent->m_state == TvT::StateCountDown || m_lpEvent->m_state == TvT::StateFight)
		{
			for(UINT n=0;n<m_lpEvent->m_lpInfo->blockedSkill.size();n++)
			{
				if(m_lpEvent->m_lpInfo->blockedSkill[n] == skillId)
					return true;
			}
		}
	}
	return false;
}