#include "StdAfx.h"
#include "AIO.h"
#include "DB.h"
#include "SkillAcquireDB.h"
#include "PlayerAction.h"

CAIOSystem g_AIOSystem;

CAIOSystem::CAIOSystem() : m_enabled(0), m_canMoveOutsidePeacezone(FALSE)
{
}

CAIOSystem::~CAIOSystem()
{
}

void CAIOSystem::Init()
{
	LoadData();
	g_Log.Add(CLog::Blue, "[%s] Enabled [%d] - loaded [%d] AIO profile(s).", __FUNCTION__, m_enabled, m_data.size());
}

void CAIOSystem::LoadData()
{
	wstringstream file(ReadFileW(L"..\\Script\\AIOData.txt"));
	if(file)
	{
		wstring line;
		while(getline(file, line))
		{
			if(line.find(L"//") == 0)
				continue;

			if(line.find(L"global_setting_begin") == 0)
			{
				//global_setting_begin	enabled=1 can_use_outside_peacezone=0	global_setting_end
				m_enabled = Parser::ParseOptionInt(line, L"enabled", 0);
				m_canUseOutsidePeaceZone = Parser::ParseOptionInt(line, L"can_use_outside_peacezone", 0);
				m_canMoveOutsidePeacezone = Parser::ParseOptionInt(line, L"can_move_outside_peacezone", 1);
				wstring loc = Parser::ParseOptionString(line, L"peacezone_location", L"0/0/0");
				if(loc.size() > 4)
				{
					loc = Utils::ReplaceString(loc, _T("/"), _T(" "), true);
					wstringstream sstr;
					sstr << loc;
					sstr >> m_peaceZoneLoc.x >> m_peaceZoneLoc.y >> m_peaceZoneLoc.z;
				}
			}

			if(line.find(L"setting_begin") == 0)
			{
				//setting_begin enabled=1 id=1  min_level=1 price_id=57  price_amount=10000  nickname_color=15453831 expire_time=2592000  reward_id=5706  setting_end
				AIOData data;
				data.enabled = Parser::ParseOptionInt(line, L"enabled", 0);
				data.expireTime = Parser::ParseOptionInt(line, L"expire_time", -1);
				data.id = Parser::ParseOptionInt(line, L"id");
				data.minLevel = Parser::ParseOptionInt(line, L"min_level");
				data.nicknameColor = Parser::ParseOptionInt(line, L"nickname_color");
				data.priceId = Parser::ParseOptionInt(line, L"price_id");
				data.priceAmount = Parser::ParseOptionInt(line, L"price_amount");
				data.rewardId = Parser::ParseOptionInt(line, L"reward_id");
				if(data.id > 0)
				{
					m_data.insert(pair<UINT, AIOData>(data.id, data));
				}
			}else if(line.find(L"begin") == 0 )
			{
				//begin	id=1  skill_id=1204	skill_level=2 abnormal_time=3600	party_buff=0	end
				AIOSkill skill;
				UINT id = Parser::ParseOptionInt(line, L"id");
				skill.skill.skill_id = Parser::ParseOptionInt(line, L"skill_id");
				skill.skill.level = Parser::ParseOptionInt(line, L"skill_level");
				skill.abnormalTime = Parser::ParseOptionInt(line, L"abnormal_time");
				skill.partyBuff = Parser::ParseOptionInt(line, L"party_buff");
				
				if(id > 0 && skill.skill.skill_id > 0 && skill.skill.level > 0)
				{
					map<UINT, AIOData>::iterator it = m_data.find(id);
					if(it != m_data.end())
					{
						it->second.skills.push_back(skill);
					}else
					{
						g_Log.Add(CLog::Error, "[%s] Invalid aio id[%d] for skill[%d][%d]!", __FUNCTION__, id, skill.skill.skill_id, skill.skill.level);
					}
				}
			}
		}
	}
}

bool CAIOSystem::CanUse(User *pUser, int skillId)
{
	bool canUse = true;
	if(m_enabled)
	{
		if(pUser->ValidUser())
		{
			if(pUser->pED->aioId != 0 && pUser->pSD->nSubClassIndex == 0)
			{
				map<UINT, AIOData>::iterator it = m_data.find(pUser->pED->aioId);
				if(it!=m_data.end())
				{
					vector<AIOSkill>& skills = it->second.skills;
					for(UINT n=0;n<skills.size();n++)
					{
						if(skills[n].skill.skill_id == skillId)
						{
							if(!m_canUseOutsidePeaceZone && !pUser->inPeaceZone)
							{
								canUse = false;
							}
							break;
						}
					}
				}else
				{
					g_Log.Add(CLog::Error, "[%s] Invalid aio type[%d] user[%S]!", __FUNCTION__, pUser->pED->aioId, pUser->pSD->wszName);
				}
			}
		}
	}
	return canUse;
}

void CAIOSystem::BecomeAIO(User *pUser, UINT id, bool byGM)
{
	CTL;
	if(m_enabled)
	{
		if(pUser->ValidUser())
		{
			if(pUser->pED->aioId == 0)
			{
				if(pUser->pSD->nSubClassIndex == 0)
				{
					map<UINT, AIOData>::iterator it = m_data.find(id);
					if(it != m_data.end())
					{
						AIOData& aio = it->second;
						if(aio.enabled)
						{
							if(pUser->IsNowTrade())
							{
								pUser->TradeCancel();
							}

							if(aio.minLevel <= pUser->pSD->nLevel || byGM)
							{
								if(aio.priceId > 0 && aio.priceAmount > 0 && !byGM)
								{
									if(!pUser->DeleteItemInInventory(aio.priceId, aio.priceAmount))
									{
										//2525	1	a,You don't have enough item(s) to acquire AIO status!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
										pUser->pSocket->SendSystemMessage(2525);
										return;
									}
								}

								if(aio.rewardId > 0)
								{
									pUser->AddItemToInventory(aio.rewardId, 1);
								}

								UINT timestamp = time(0);
								if(aio.expireTime == -1)
								{
									timestamp = -1;
								}else
								{
									timestamp += aio.expireTime;
								}
								pUser->pED->aioId = aio.id;
								pUser->pED->aioTimestamp = timestamp;
								pUser->pED->aioNicknameColor = aio.nicknameColor;
								g_DB.RequestSaveAIO(pUser->nDBID, aio.id, timestamp);
								AcquireSkills(pUser);
								//2526	1	a,Congratulations! You have acquired AIO status!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
								pUser->pSocket->SendSystemMessage(2526);
								pUser->SocialAction(SocialActionLevelUp);
								g_Logger.Add(L"User[%s] acquired AIO[%d] status.", pUser->pSD->wszName, aio.id);
							}else
							{
								//2527	1	a,Your level is too low to acquire AIO status!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
								pUser->pSocket->SendSystemMessage(2527);
							}
						}else
						{
							//2528	1	a,This AIO option is disabled!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pUser->pSocket->SendSystemMessage(2528);
						}
					}else
					{
						g_Log.Add(CLog::Error, "[%s] Invalid aio id[%d] user[%S]", __FUNCTION__, id, pUser->pSD->wszName);
					}
				}else
				{
					//2529	1	a,AIO status can be acquired only in main class!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pUser->pSocket->SendSystemMessage(2529);
				}
			}else
			{
				//2530	1	a,You already have AIO status!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2530);
			}
		}
	}
}

void CAIOSystem::RemoveAIO(User *pUser)
{
	guard;
	if(m_enabled)
	{
		if(pUser->pED->aioId != 0)
		{
			if(pUser->pSD->nSubClassIndex == 0)
			{
				pUser->pED->aioTimestamp = time(0);
				//2531	1	a,Your AIO status will be removed shortly.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2531);
			}else
			{
				//2532	1	a,Please switch to your main class.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2532);
			}
		}else
		{
			//2533	1	a,You have to be AIO character in order to use this option!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->pSocket->SendSystemMessage(2533);
		}
	}
	unguard;
}

DWORD CAIOSystem::GetNicknameColor(UINT id)
{
	guard;
	UINT nicknameColor = 0;
	map<UINT, AIOData>::iterator it = m_data.find(id);
	if(it!=m_data.end())
	{
		nicknameColor = it->second.nicknameColor;
	}
	unguard;
	return nicknameColor;
}

void CAIOSystem::AcquireSkills(User *pUser)
{
	guard;
	if(m_enabled)
	{
		if(pUser->ValidUser())
		{
			bool gotNew = false;
			map<UINT, AIOData>::iterator it = m_data.find(pUser->pED->aioId);
			if(it!=m_data.end())
			{
				if(it->second.enabled)
				{
					vector<AIOSkill>& skills = it->second.skills;
					for(UINT n=0;n<skills.size();n++)
					{
						if(pUser->GetAcquiredSkillLevel(skills[n].skill.skill_id) < skills[n].skill.level)
						{
							pUser->AcquireSkill(skills[n].skill.skill_id, skills[n].skill.level);
							pUser->SendAcquireSkillToDB(skills[n].skill.skill_id, skills[n].skill.level);
							gotNew = true;
						}
					}
				}
			}

			if(gotNew)
			{
				pUser->ValidateSkillList();
				pUser->ValidateParameters();
			}
		}
	}
	unguard;
}

void CAIOSystem::DeleteSkills(User *pUser)
{
	guard;
	g_SkillAcquireDB.ValidateSkills(pUser);
	unguard;
}

void CAIOSystem::TimerExpired(User *pUser)
{
	guard;
	if(m_enabled)
	{
		if(pUser->ValidUser())
		{
			if(pUser->pSD->nSubClassIndex == 0 && pUser->pED->aioId && pUser->pED->aioTimestamp != -1)
			{
				DWORD currentTime = time(NULL);
				if(pUser->pED->aioTimestamp < currentTime)
				{
					UINT id = pUser->pED->aioId;
					pUser->pED->aioId = 0;
					pUser->pED->aioTimestamp = 0;
					pUser->pED->aioNicknameColor = 0;
					pUser->SetUserInfoChanged();
					pUser->SetCharInfoChanged();
					g_DB.RequestSaveAIO(pUser->nDBID, 0, 0);
					//2534	1	a,Your AIO status just expired!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pUser->pSocket->SendSystemMessage(2534);
					DeleteSkills(pUser);
					map<UINT, AIOData>::iterator it = m_data.find(id);
					if(it!=m_data.end())
					{
						if(it->second.rewardId)
						{
							if(pUser->inventory.HaveItemByClassID(it->second.rewardId, 1))
							{
								pUser->DeleteItemInInventory(it->second.rewardId, 1);
							}
						}
					}
				}
				if(m_canMoveOutsidePeacezone == FALSE && pUser->pSD->nInPeaceZone == FALSE)
				{
					if(pUser->pSD->nActiveUser)
					{
						PlayerAction::OnTeleportToLocation(pUser, m_peaceZoneLoc.x, m_peaceZoneLoc.y, m_peaceZoneLoc.z, 0);
					}
				}
			}
		}
	}
	unguard;
}

void CAIOSystem::OnLoad(User *pUser, UINT id, UINT timestamp)
{
	guard;
	if(m_enabled)
	{
		if(pUser->ValidUser())
		{
			UINT currentTime = time(NULL);
			if(timestamp < currentTime)
			{
				pUser->pED->aioId = 0;
				pUser->pED->aioTimestamp = 0;
				pUser->pED->aioNicknameColor = 0;
				g_DB.RequestSaveAIO(pUser->nDBID, 0, 0);
				//2534	1	a,Your AIO status just expired!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2534);
				DeleteSkills(pUser);
				map<UINT, AIOData>::iterator it = m_data.find(id);
				if(it!=m_data.end())
				{
					if(it->second.rewardId)
					{
						if(pUser->inventory.HaveItemByClassID(it->second.rewardId, 1))
						{
							pUser->DeleteItemInInventory(it->second.rewardId, 1);
						}
					}
				}
			}else
			{
				pUser->pED->aioId = id;
				pUser->pED->aioTimestamp = timestamp;
				pUser->pED->aioNicknameColor = GetNicknameColor(id);
				pUser->SetUserInfoChanged();
				pUser->SetCharInfoChanged();
				if(timestamp != -1)
				{
					DWORD diff = timestamp - currentTime;
					DWORD days = diff / 86400;
					if(days)
						diff = diff % 86400;
					DWORD hours = diff / 3600;
					if(hours)
						diff = diff % 3600;
					DWORD minutes = diff / 60;
					if(minutes)
						diff = diff % 60;
					DWORD seconds = diff;
					
					//2535	1	a,Your AIO status will expire in $s1 day(s) $s2 hour(s) and $s3 minute(s).\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					CSystemMessage msg(2535);
					msg.AddNumber(days);
					msg.AddNumber(hours);
					msg.AddNumber(minutes);
					pUser->SendSystemMessage(&msg);
				}
				if(pUser->pSD->nSubClassIndex == 0)
				{
					DeleteSkills(pUser);
					AcquireSkills(pUser);
				}
			}
		}
	}
	unguard;
}

INT32 CAIOSystem::GetAbnormalTime(User *pUser, int skillId, int level)
{
	if(m_enabled)
	{
		if(pUser->ValidUser())
		{
			if(pUser->pED->aioId)
			{
				map<UINT, AIOData>::iterator it = m_data.find(pUser->pED->aioId);
				if(it!=m_data.end())
				{
					vector<AIOSkill>& skills = it->second.skills;
					for(UINT n=0;n<skills.size();n++)
					{
						if(skills[n].skill.skill_id == skillId && skills[n].skill.level == level)
						{
							return skills[n].abnormalTime;
						}
					}
				}
			}
		}
	}
	return 0;
}

bool CAIOSystem::IsPartyBuff(UINT id, int skillId, int level)
{
	guard;
	if(m_enabled)
	{
		map<UINT, AIOData>::iterator it = m_data.find(id);
		if(it!=m_data.end())
		{
			vector<AIOSkill>& skills = it->second.skills;
			for(UINT n=0;n<skills.size();n++)
			{
				if(skills[n].skill.skill_id == skillId && skills[n].skill.level == level)
				{
					unguard;
					return skills[n].partyBuff;
				}
			}
		}
	}
	unguard;
	return false;
}