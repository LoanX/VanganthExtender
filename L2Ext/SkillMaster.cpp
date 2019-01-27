#include "stdafx.h"
#include "SkillMaster.h"
#include "License.h"


using namespace License;

CSkillMaster g_SkillMaster;

#pragma optimize("", off)

void CSkillMaster::Init()
{
	VIRTUALIZER_TIGER_BLACK_START;
	m_Enabled = false;
	if(CLicense::GetInstance().CanUse(LicenseSkillMaster))
	{
		m_Enabled = false;
		m_SkillCount = 0;
		m_Lists.clear();
		LoadData();
		if(m_Enabled)
		{
			g_Log.Add(CLog::Blue, "[%s] Feature is enabled, loaded [%d] list(s) and [%d] skill(s).", __FUNCTION__, m_Lists.size(), m_SkillCount);
		}
	}
	VIRTUALIZER_TIGER_BLACK_END;
}


#pragma optimize("", on)

/*
setting_begin enabled=1	setting_end
skill_begin	skill_id=370	level=1	max_level=3	required_level=70	price_id=7575	price_amount=1	skill_end
skill_begin	skill_id=370	level=2	max_level=3	required_level=70	price_id=7575	price_amount=1	skill_end
skill_begin	skill_id=370	level=3	max_level=3	required_level=70	price_id=7575	price_amount=1	skill_end
*/

void CSkillMaster::LoadData()
{
	guard;

	LPBYTE lpFile = 0;
	UINT size = ReadFileBinary(L"..\\Script\\SkillMaster.txt", lpFile);
	if(lpFile)
	{
		if(size > 2)
		{
			wstringstream file((PWCHAR)&lpFile[2]);
			wstring line;
			UINT currentList = 0;
			MasterSkillList* pList = 0;
			while(getline(file, line))
			{
				if(line.find(L"\\") == 0 || line.find(L"//") == 0)
				{
					continue;
				}
				if(line.find(L"setting_begin") == 0)
				{
					//parse settings
					//setting_begin enabled=1	setting_end
					m_Enabled = Parser::ParseOptionInt(line, L"enabled");
					
				}else if(line.find(L"skill_begin") == 0)
				{
					if(pList)
					{
						//skill_begin	skill_id=370	level=1	max_level=3	required_level=70	price_id=7575	price_amount=1	skill_end
						MasterSkillInfo msi;

						msi.skillId = Parser::ParseOptionInt(line, L"skill_id");
						msi.level = Parser::ParseOptionInt(line, L"level");
						msi.maxLevel = Parser::ParseOptionInt(line, L"max_level");
						msi.requiredLevel = Parser::ParseOptionInt(line, L"required_level");
						msi.price.itemId = Parser::ParseOptionInt(line, L"price_id");
						msi.price.amount = Parser::ParseOptionInt(line, L"price_amount");
						msi.priceSP = Parser::ParseOptionInt(line, L"price_sp");
						pList->m_Skills[msi.skillId].push_back(msi);
						SkillPchId pch(msi.skillId, msi.level);
						pList->m_PchId.push_back(pch);
						m_SkillCount++;
					}
				}else if(line.find(L"list_") == 0)
				{
					//list_X_begin / list_X_end
					line = Parser::Replace(line, L'_', L' ');
					wstringstream paramStream(line);
					wstring temp;
					if(paramStream >> temp >> currentList >> temp)
					{
						if(currentList > 0)
						{
							if(temp == L"begin")
							{
								pList = new MasterSkillList();
								pList->listId = currentList;
							}else if(temp == L"end")
							{
								if(pList)
								{
									m_Lists.push_back(pList);
								}
							}
						}
					}
				}
			}
		}
		delete [] lpFile;
	}

	unguard;
}

MasterSkillList* CSkillMaster::GetSkillList(UINT listId)
{
	guard;
	MasterSkillList* pList = 0;
	for(UINT n=0;n<m_Lists.size();n++)
	{
		if(m_Lists[n]->listId == listId)
		{
			pList = m_Lists[n];
			break;
		}
	}

	unguard;
	return pList;
}

bool CSkillMaster::IsAvailable(int skillId, int level, UINT listId)
{
	SkillPchId pch(skillId, level);
	if(MasterSkillList* pList = GetSkillList(listId))
	{
		for(UINT n=0;n<pList->m_PchId.size();n++)
		{
			if(pList->m_PchId[n] == pch)
			{
				return true;
			}
		}
	}
	return false;
}

INT32 CSkillMaster::GetLowerLevel(int skillId, int baseLevel, UINT listId)
{
	guard;
	INT32 level = 0;

	if(MasterSkillList* pList = GetSkillList(listId))
	{
		for(UINT n=0;n<pList->m_PchId.size();n++)
		{
			SkillPchId& pch = pList->m_PchId[n];
			if(pch.skillId == skillId)
			{
				if(pch.level < baseLevel && level < pch.level)
				{
					level = pch.level;
				}
			}
		}
	}

	unguard;
	return level;
}

MasterSkillInfo& CSkillMaster::GetSkillInfo(int skillId, int level, UINT listId)
{
	guard;

	static MasterSkillInfo m_NullInfo;
	if(MasterSkillList* pList = GetSkillList(listId))
	{
		map<INT32, vector<MasterSkillInfo>>::iterator it = pList->m_Skills.find(skillId);
		if(it!=pList->m_Skills.end())
		{
			vector<MasterSkillInfo>& skills = it->second;
			for(UINT n=0;n<skills.size();n++)
			{
				if(skills[n].level == level)
				{
					unguard;
					return skills[n];
				}
			}
		}
	}


	unguard;
	return m_NullInfo;
}

void CSkillMaster::ShowSkillList(User *pUser, UINT listId)
{
	guard;

	if(m_Enabled)
	{
		if(MasterSkillList* pList = GetSkillList(listId))
		{
			char buff[8190] = { 0 };
			int len = 0;
			int skillCount = 0;
			for(map<int, vector<MasterSkillInfo>>::iterator it = pList->m_Skills.begin(); it != pList->m_Skills.end(); it++)
			{
				INT32 acquiredLevel = pUser->GetAcquiredSkillLevel(it->first);
				vector<MasterSkillInfo>& skills = it->second;
				for(UINT n=0;n<skills.size();n++)
				{
					MasterSkillInfo& msi = skills[n];
					if(acquiredLevel < msi.level)
					{
						if(pUser->pSD->nLevel >= msi.requiredLevel)
						{
							//add skill to buffer
							len += Assemble(&buff[len], (sizeof(buff) - len), "ddddd", msi.skillId, msi.level, msi.maxLevel, msi.priceSP, msi.price.itemId);
							skillCount++;
							break;
						}
					}
				}
			}
			if(skillCount > 0)
			{
				pUser->pSocket->Send("cddb", 0x8A, m_AcquireSkillMode, skillCount, len, buff);
				pUser->pED->skillMasterFlag = listId;
			}else
			{
				pUser->pSocket->Send("cdd", 0x8A, m_AcquireSkillMode, 0);
			}
		}
	}

	unguard;
}

void CSkillMaster::AcquireSkill(User *pUser, int skillId, int level, UINT listId)
{
	guard;

	if(m_Enabled)
	{
		//check if skill exist in db
		if(IsAvailable(skillId, level, listId))
		{
			INT32 lowerLevel = GetLowerLevel(skillId, level, listId);
			if(lowerLevel == 0 || pUser->GetAcquiredSkillLevel(skillId) == lowerLevel)
			{
				//check requirements
				MasterSkillInfo& msi = GetSkillInfo(skillId, level, listId);
				if(msi.skillId == skillId)
				{
					if(msi.priceSP == 0 || pUser->pSD->nSp >= msi.priceSP)
					{
						if(msi.price.itemId == 0 || msi.price.amount == 0 || pUser->DeleteItemInInventory(msi.price.itemId, msi.price.amount))
						{
							//consume sp
							pUser->WriteLock(__FILEW__, __LINE__);
							pUser->pSD->nSp = pUser->pSD->nSp - msi.priceSP;
							pUser->WriteUnlock();
							pUser->SetUserInfoChanged();
							pUser->AcquireSkill(msi.skillId, msi.level);
							pUser->SendAcquireSkillToDB(msi.skillId, msi.level);
							pUser->ValidateParameters();

							//277	1	a,You have earned $s1.\0	4	79	9B	B0	FF	a,ItemSound3.sys_enchant_success\0	a,	0	0	0	0	0	a,	a,popup\0
							CSystemMessage msg(277);
							msg.AddSkill(msi.skillId, msi.level);
							pUser->SendSystemMessage(&msg);

							ShowSkillList(pUser, listId);
							
						}else
						{
							//276	1	a,You do not have the necessary materials or prerequisites to learn this skill.\0	4	79	9B	B0	FF	a,ItemSound3.sys_shortage\0	a,	0	0	0	0	0	a,	a,popup\0
							pUser->pSocket->SendSystemMessage(276);
						}
					}else
					{
						//278	1	a,You do not have enough SP to learn this skill.\0	4	79	9B	B0	FF	a,ItemSound3.sys_shortage\0	a,	0	0	0	0	0	a,	a,popup\0
						pUser->pSocket->SendSystemMessage(278);
					}
				}
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Hack - User[%S] tried to acquire invalid skill[%d][%d]", __FUNCTION__, pUser->pSD->wszName, skillId, level);
			}
		}else
		{
			g_Log.Add(CLog::Error, "[%s] User[%S] tried to use not available skill[%d][%d]!", __FUNCTION__, pUser->pSD->wszName, skillId, level);
		}
	}

	unguard;
}

void CSkillMaster::ShowSkillInfo(User *pUser, int skillId, int level, UINT listId)
{
	guard;

	if(m_Enabled)
	{
		MasterSkillInfo& msi = GetSkillInfo(skillId, level, listId);
		if(msi.skillId == skillId)
		{
			if(msi.price.itemId && msi.price.amount)
			{
				pUser->pSocket->Send("cddddddddd", 0x8B, msi.skillId, msi.level, msi.priceSP, m_AcquireSkillMode, 1, 1, msi.price.itemId, msi.price.amount, 0);
			}else
			{
				pUser->pSocket->Send("cddddddd", 0x8B, msi.skillId, msi.level, msi.priceSP, m_AcquireSkillMode, 1, 0, 0);
			}			
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Invalid skill[%d][%d] - User[%S]", __FUNCTION__, skillId, level, pUser->pSD->wszName);
		}
	}

	unguard;
}