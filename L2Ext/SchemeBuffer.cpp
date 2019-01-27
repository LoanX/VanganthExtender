#include "stdafx.h"
#include "SchemeBuffer.h"

CSchemeBuffer g_SchemeBuffer;

CSchemeBuffer::CSchemeBuffer() : m_Enabled(FALSE), m_ReloadTime(0)
{
}

CSchemeBuffer::~CSchemeBuffer()
{

}

void CSchemeBuffer::Init()
{
	guard;
	m_Enabled = FALSE;
	Sleep(500);
	m_ReloadTime = 0;
	m_EmptyBuff.abnormalTime = 0;
	m_EmptyBuff.maxLevel = 0;
	m_EmptyBuff.minLevel = 0;
	m_EmptyBuff.price.itemId = 0;
	m_EmptyBuff.price.amount = 0;
	m_EmptyBuff.pSI = 0;
	m_EmptyBuff.skill.pchId = 0;
	m_Macro.clear();

	LoadData();
	if(m_Enabled)
	{
		g_Log.Add(CLog::Blue, "[%s] feature is enabled - loaded [%d][%d] buff(s).", __FUNCTION__, m_Buffs.size(), m_RemoteBuffs.size());
	}
	unguard;
}

void CSchemeBuffer::LoadData()
{
	guard;

	wstringstream file(ReadFileW(L"..\\Script\\SchemeBuffer.txt"));
	wstring line;
	BOOL enabled = FALSE;
	m_Buffs.clear();
	m_RemoteBuffs.clear();
	while(getline(file, line))
	{
		if(line.find(L"//") == 0)
			continue;

		if(line.find(L"setting_begin") == 0)
		{
			//setting_begin enabled=1	allow_pk=0	allow_guilty=0	allow_combat=0	allow_olympiad=0	low_level_msg=2454	high_level_msg=2455	not_enough_adena_msg=2456	add_to_list_msg=2408	add_to_summon_list_msg=2409	setting_end
			enabled = Parser::ParseOptionInt(line, L"enabled");
			m_AllowPK = Parser::ParseOptionInt(line, L"allow_pk");
			m_AllowGuilty = Parser::ParseOptionInt(line, L"allow_guilty");
			m_AllowCombat = Parser::ParseOptionInt(line, L"allow_combat");
			m_AllowOlympiad = Parser::ParseOptionInt(line, L"allow_olympiad");
			m_AllowCursedWeapon = Parser::ParseOptionInt(line, L"allow_cursedweapon");
			m_PeaceZoneOnly = Parser::ParseOptionInt(line, L"peacezone_only");
			m_BuffDelay = Parser::ParseOptionInt(line, L"buff_delay", 500);
			m_UseDelay = Parser::ParseOptionInt(line, L"use_delay", 500);
			m_LowLevelMsg = Parser::ParseOptionInt(line, L"low_level_msg", 2454);
			m_HighLevelMsg = Parser::ParseOptionInt(line, L"high_level_msg", 2455);
			m_NotEnoughAdenaMsg = Parser::ParseOptionInt(line, L"not_enough_adena_msg", 2456);
			m_AddToListMsg = Parser::ParseOptionInt(line, L"add_to_list_msg", 2408);
			m_AddToSummonListMsg = Parser::ParseOptionInt(line, L"add_to_summon_list_msg", 2409);
			m_UseListPrice.itemId = Parser::ParseOptionInt(line, L"use_list_price_id", 0);
			m_UseListPrice.amount = Parser::ParseOptionInt(line, L"use_list_price_amount", 0);
		}else if(line.find(L"buff_begin") == 0)
		{
			//buff_begin	pch_id=1111554	price=1000	min_level=0	max_level=80	abnormal_time=3600	remote_access=1	buff_end
			SchemeBuff buff;
			buff.skill.pchId = Parser::ParseOptionInt(line, L"pch_id");
			buff.price.itemId = Parser::ParseOptionInt(line, L"price_id");
			buff.price.amount = Parser::ParseOptionInt(line, L"price_amount");
			buff.minLevel = Parser::ParseOptionInt(line, L"min_level");
			buff.maxLevel = Parser::ParseOptionInt(line, L"max_level");
			buff.abnormalTime = Parser::ParseOptionInt(line, L"abnormal_time");
			buff.requiredItem.itemId = Parser::ParseOptionInt(line, L"required_item_id");
			buff.requiredItem.amount = Parser::ParseOptionInt(line, L"required_amount");
			int temp = Parser::ParseOptionInt(line, L"required_item_amount");
			if(temp != 0)
			{
				buff.requiredItem.amount = temp;
			}
			if(CSkillInfo *pSI = g_SkillDB.GetSkillInfo(buff.skill.skillId, buff.skill.level))
			{
				buff.pSI = pSI;
				m_Buffs.push_back(buff);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Invalid skill[%d]!", __FUNCTION__, buff.skill.pchId);
			}
		}else if(line.find(L"remote_buff_begin") == 0)
		{
			//remote_buff_begin	pch_id=1111554	price=1000	min_level=0	max_level=80	abnormal_time=3600	remote_access=1	remote_buff_end
			SchemeBuff buff;
			buff.skill.pchId = Parser::ParseOptionInt(line, L"pch_id");
			buff.price.itemId = Parser::ParseOptionInt(line, L"price_id");
			buff.price.amount = Parser::ParseOptionInt(line, L"price_amount");
			buff.minLevel = Parser::ParseOptionInt(line, L"min_level");
			buff.maxLevel = Parser::ParseOptionInt(line, L"max_level");
			buff.abnormalTime = Parser::ParseOptionInt(line, L"abnormal_time");
			buff.requiredItem.itemId = Parser::ParseOptionInt(line, L"required_item_id");
			buff.requiredItem.amount = Parser::ParseOptionInt(line, L"required_amount");
			if(CSkillInfo *pSI = g_SkillDB.GetSkillInfo(buff.skill.skillId, buff.skill.level))
			{
				buff.pSI = pSI;
				m_RemoteBuffs.push_back(buff);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Invalid remote skill[%d]!", __FUNCTION__, buff.skill.pchId);
			}
		}else if(line.find(L"macro_begin") == 0)
		{
			SchemeMacro macro;
			//macro_begin	name=mage	min_level=0	max_level=80	price_id=57	abnormal_time=3600	price_amount=1000	required_item_id=0	required_item_amount=0	buffs={91029507;68157443;78905346;69599234;69402627;70647814;85393410;71106563;67895298;88604673}	macro_end
			macro.abnormalTime = Parser::ParseOptionInt(line, L"abnormal_time");
			macro.price.itemId = Parser::ParseOptionInt(line, L"price_id");
			macro.price.amount = Parser::ParseOptionInt(line, L"price_amount");
			macro.requierdItem.itemId = Parser::ParseOptionInt(line, L"required_item_id");
			macro.requierdItem.amount = Parser::ParseOptionInt(line, L"required_item_amount");
			macro.minLevel = Parser::ParseOptionInt(line, L"min_level");
			macro.maxLevel = Parser::ParseOptionInt(line, L"max_level");

			wstring name = Parser::ParseOptionString(line, L"name");
			wstring buffs = Parser::ParseOptionString(line, L"buffs");
			buffs = Parser::Replace(buffs, L'{', L' ');
			buffs = Parser::Replace(buffs, L'}', L' ');
			buffs = Parser::Replace(buffs, L';', L' ');
			wstringstream buffStream(buffs);
			UINT pchId = 0;
			while(buffStream >> pchId)
			{
				SkillPchId pch(pchId);
				if(CSkillInfo *pSI = g_SkillDB.GetSkillInfo(pch.skillId, pch.level))
				{
					macro.buffs.push_back(pSI);
				}
			}
			if(name.size() > 0)
			{
				m_Macro[name] = macro;
			}
		}else if(line.find(L"macro_remote_begin") == 0)
		{
			SchemeMacro macro;
			//macro_begin	name=mage	min_level=0	max_level=80	price_id=57	abnormal_time=3600	price_amount=1000	required_item_id=0	required_item_amount=0	buffs={91029507;68157443;78905346;69599234;69402627;70647814;85393410;71106563;67895298;88604673}	macro_end
			macro.abnormalTime = Parser::ParseOptionInt(line, L"abnormal_time");
			macro.price.itemId = Parser::ParseOptionInt(line, L"price_id");
			macro.price.amount = Parser::ParseOptionInt(line, L"price_amount");
			macro.requierdItem.itemId = Parser::ParseOptionInt(line, L"required_item_id");
			macro.requierdItem.amount = Parser::ParseOptionInt(line, L"required_item_amount");
			macro.minLevel = Parser::ParseOptionInt(line, L"min_level");
			macro.maxLevel = Parser::ParseOptionInt(line, L"max_level");

			wstring name = Parser::ParseOptionString(line, L"name");
			wstring buffs = Parser::ParseOptionString(line, L"buffs");
			buffs = Parser::Replace(buffs, L'{', L' ');
			buffs = Parser::Replace(buffs, L'}', L' ');
			buffs = Parser::Replace(buffs, L';', L' ');
			wstringstream buffStream(buffs);
			UINT pchId = 0;
			while(buffStream >> pchId)
			{
				SkillPchId pch(pchId);
				if(CSkillInfo *pSI = g_SkillDB.GetSkillInfo(pch.skillId, pch.level))
				{
					macro.buffs.push_back(pSI);
				}
			}
			if(name.size() > 0)
			{
				m_MacroRemote[name] = macro;
			}
		}
	}

	m_Enabled = enabled;
	unguard;
}

void CSchemeBuffer::RequestReload()
{
	m_Enabled = FALSE;
	m_ReloadTime = time(0) + 2;
}

void CSchemeBuffer::TimerExpired()
{
	guard;
	if(m_ReloadTime > 0)
	{
		if(m_ReloadTime < time(0))
		{
			Init();
		}
	}
	unguard;
}

bool CSchemeBuffer::CanUse(User *pUser)
{
	guard;
	UINT currentTick = GetTickCount();
	if( (pUser->pED->schemeBufferUser.useTick < currentTick) || ( (pUser->pED->schemeBufferUser.useTick - m_UseDelay) > currentTick) )
	{
		pUser->pED->schemeBufferUser.useTick = currentTick + m_UseDelay;
		if(pUser->pED->schemeBufferUser.inBlockZone)
		{
			pUser->SendSystemMessage(L"You cannot perform this action here!");
			unguard;
			return false;
		}
		if(pUser->pSD->nActiveUser == 0)
		{
			pUser->SendSystemMessage(L"You cannot perform this action while teleporting!");
			unguard;
			return false;
		}
		if(!pUser->inPeaceZone && m_PeaceZoneOnly)
		{
			pUser->SendSystemMessage(L"You cannot perform this action outside a peace zone!");
			unguard;
			return false;
		}
		if(pUser->pSD->nAlive == 0)
		{
			pUser->SendSystemMessage(L"You cannot perform this action when you're dead!");
			unguard;
			return false;
		}
		if(pUser->pSD->nKarma > 0 && !m_AllowPK )
		{
			if(pUser->pSD->nCursedOwner == 0 || !m_AllowCursedWeapon)
			{
				pUser->SendSystemMessage(L"You cannot perform this action in PK mode!");
				unguard;
				return false;
			}
		}
		if(pUser->pSD->bGuilty && !m_AllowGuilty)
		{
			pUser->SendSystemMessage(L"You cannot perform this action in PvP mode!"); 
			unguard;
			return false;
		}
		if(pUser->IsInCombatMode() && !m_AllowCombat)
		{
			pUser->SendSystemMessage(L"You cannot perform this action in combat mode!"); 
			unguard;
			return false;
		}
		if(pUser->olympiadUser.IsInside() && !m_AllowOlympiad)
		{
			pUser->SendSystemMessage(L"You cannot perform this action during olympiad fight!");
			unguard;
			return false;
		}
		unguard;
		return true;
	}

	unguard;
	return false;
}

SchemeBuff& CSchemeBuffer::Get(UINT buffId, bool remote)
{
	guard;
	if(remote)
	{
		for(UINT n=0;n<m_RemoteBuffs.size();n++)
		{
			if(m_RemoteBuffs[n].skill.pchId == buffId)
			{
				unguard;
				return m_RemoteBuffs[n];
			}
		}
	}else
	{
		for(UINT n=0;n<m_Buffs.size();n++)
		{
			if(m_Buffs[n].skill.pchId == buffId)
			{
				unguard;
				return m_Buffs[n];
			}
		}
	}

	unguard;
	return m_EmptyBuff;
}

void CSchemeBuffer::UseMacro(User *pUser, wstring name, bool pet, bool remote)
{
	guard;
	if(m_Enabled)
	{
		if(CanUse(pUser))
		{
			//Find Macro
			map<wstring, SchemeMacro>::iterator it;

			if(remote)
			{
				it = m_MacroRemote.find(name);
			}else
			{
				it = m_Macro.find(name);
			}

			if( (remote && it != m_MacroRemote.end()) || (!remote && it != m_Macro.end()) )
			{
				SchemeMacro& macro = it->second;

				//check player levels
				if(macro.minLevel > pUser->pSD->nLevel)
				{
					pUser->SendSystemMessage(L"Your character has too low level to use this buff macro.");
					unguard;
					return;
				}
				if(macro.maxLevel < pUser->pSD->nLevel)
				{
					pUser->SendSystemMessage(L"Your character has too high level to use this buff macro.");
					unguard;
					return;
				}

				//check required item
				if(macro.requierdItem.itemId > 0 && macro.requierdItem.amount > 0)
				{
					if(!pUser->inventory.HaveItemByClassID(macro.requierdItem.itemId, macro.requierdItem.amount))
					{
						pUser->SendSystemMessage(L"You don't have enough required item(s) to use this macro.");
						unguard;
						return;
					}
				}

				CCreature *pTarget = pUser;
				if(pet)
				{
					if(CSummon *pSummon = pUser->GetSummonOrPet())
					{
						if(pSummon->IsInRange(pUser, 600))
						{
							pTarget = pSummon;
						}else
						{
							pUser->SendSystemMessage(L"Your pet/summon is too far away!");
							unguard;
							return;
						}
					}else
					{
						unguard;
						return;
					}
				}

				//check price
				bool paid = false;
				if(macro.price.itemId > 0 && macro.price.amount > 0)
				{
					if(pUser->inventory.HaveItemByClassID(macro.price.itemId, macro.price.amount))
					{
						pUser->DeleteItemInInventory(macro.price.itemId, macro.price.amount);
						paid = true;
					}else
					{
						pUser->SendSystemMessage(L"You don't have enough item(s) to pay for this buff macro.");
						unguard;
						return;
					}
				}else
				{
					paid = true;
				}

				if(paid)
				{
					//use macro
					for(UINT n=0;n<macro.buffs.size();n++)
					{
						pUser->QueueBuff(macro.buffs[n], pet, macro.abnormalTime);
					}
				}

			}else
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find macro[%S] user[%S]!", __FUNCTION__, name.c_str(), pUser->pSD->wszName);
			}
		}
	}
	unguard;
}

bool CSchemeBuffer::RequestBuff(User *pUser, UINT buffId, bool summon, bool remote)
{
	guard;
	if(m_Enabled && CanUse(pUser))
	{
		SchemeBuff& buff = Get(buffId, remote);
		if(buff.pSI)
		{
			if(summon)
			{
				CSummon *pSummon = pUser->GetSummonOrPet();
				if(pSummon == 0)
				{
					unguard;
					return false;
				}
			}

			if(buff.requiredItem.itemId > 0)
			{
				if(pUser->inventory.HaveItemByClassID(buff.requiredItem.itemId, buff.requiredItem.amount) == false)
				{
					g_Log.Add(CLog::Error, "[%s] missing[%d][%d]", __FUNCTION__, buff.requiredItem.itemId, buff.requiredItem.amount);
					pUser->SendSystemMessage(L"You don't have required item(s) to use this buff!");
					unguard;
					return false;
				}
			}
			if(pUser->pSD->nLevel < buff.minLevel)
			{
				pUser->pSocket->SendSystemMessage(m_LowLevelMsg);
				unguard;
				return false;
			}
			if(pUser->pSD->nLevel > buff.maxLevel)
			{
				pUser->pSocket->SendSystemMessage(m_HighLevelMsg);
				unguard;
				return false;
			}

			if(buff.price.itemId > 0 && buff.price.amount > 0)
			{

				if(pUser->inventory.HaveItemByClassID(buff.price.itemId, buff.price.amount) == false)
				{
					pUser->pSocket->SendSystemMessage(m_NotEnoughAdenaMsg);
					unguard;
					return false;
				}else
				{
					pUser->DeleteItemInInventory(buff.price.itemId, buff.price.amount);
				}
			}
			pUser->QueueBuff(buff.pSI, summon, buff.abnormalTime);
			unguard;
			return true;
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Invalid buff[%d] remote[%d] User[%S]", __FUNCTION__, buffId, remote, pUser->pSD->wszName);
		}
	}
	unguard;
	return false;
}

bool CSchemeBuffer::RequestAddToList(User *pUser, UINT buffId, bool summon, bool remote)
{
	guard;
	if(m_Enabled && CanUse(pUser))
	{
		SchemeBuff& buff = Get(buffId, remote);
		if(buff.pSI)
		{
			if(pUser->pSD->nLevel < buff.minLevel)
			{
				pUser->pSocket->SendSystemMessage(m_LowLevelMsg);
				unguard;
				return false;
			}
			if(pUser->pSD->nLevel > buff.maxLevel)
			{
				pUser->pSocket->SendSystemMessage(m_HighLevelMsg);
				unguard;
				return false;
			}

			SchemeBufferUser& sbu = pUser->pED->schemeBufferUser;
			sbu.lock.Enter();
			//remove if exists and push to back
			vector<SkillPchId>& buffList = sbu.GetList(summon, remote);
			for(vector<SkillPchId>::iterator it = buffList.begin();it!=buffList.end();it++)
			{
				if(it->pchId == buffId)
				{
					buffList.erase(it);
					break;
				}
			}
			//if size of list is 48+ remove first buff
			if(buffList.size() > 47)
			{
				buffList.erase(buffList.begin());
			}

			SkillPchId skillPch(buffId);
			buffList.push_back(skillPch);
			sbu.saveList = true;
			sbu.lock.Leave();
			if(summon)
			{
				CSystemMessage msg(m_AddToSummonListMsg);
				msg.AddSkill(skillPch.skillId, skillPch.level);
				pUser->SendSystemMessage(&msg);
			}else
			{
				CSystemMessage msg(m_AddToListMsg);
				msg.AddSkill(skillPch.skillId, skillPch.level);
				pUser->SendSystemMessage(&msg);
			}
			RequestBuff(pUser, buffId, summon, remote);
			unguard;
			return true;
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Invalid buff[%d] remote[%d] User[%S]", __FUNCTION__, buffId, remote, pUser->pSD->wszName);
		}
	}
	unguard;
	return false;
}

void CSchemeBuffer::ValidateBuffList(User *pUser, bool summon, bool remote)
{
	guard;
	//check for min/max level in existing buff list
	vector<SkillPchId> buffs;
	pUser->pED->schemeBufferUser.lock.Enter();
	vector<SkillPchId>& buffList = pUser->pED->schemeBufferUser.GetList(summon, remote);
	for(UINT n=0;n<buffList.size();n++)
	{
		bool valid = false;
		if(remote)
		{
			for(UINT m=0;m<m_RemoteBuffs.size();m++)
			{
				if(m_RemoteBuffs[m].skill == buffList[n])
				{
					if(pUser->pSD->nLevel >= m_RemoteBuffs[m].minLevel && pUser->pSD->nLevel <= m_RemoteBuffs[m].maxLevel)
					{
						valid = true;
					}
					break;
				}
			}
		}else
		{
			for(UINT m=0;m<m_Buffs.size();m++)
			{
				if(m_Buffs[m].skill == buffList[n])
				{
					if(pUser->pSD->nLevel >= m_Buffs[m].minLevel && pUser->pSD->nLevel <= m_Buffs[m].maxLevel)
					{
						valid = true;
					}
					break;
				}
			}
		}
		
		if(valid)
		{
			buffs.push_back(buffList[n]);
		}
	}
	buffList.clear();
	for(UINT n=0;n<buffs.size();n++)
	{
		buffList.push_back(buffs[n]);
	}

	pUser->pED->schemeBufferUser.lock.Leave();
	unguard;
}

bool CSchemeBuffer::RequestUseList(User *pUser, bool summon, bool remote)
{
	guard;
	if(m_Enabled && CanUse(pUser))
	{
		if(m_UseListPrice.itemId > 0 && m_UseListPrice.amount > 0)
		{
			SchemeBufferUser& sbu = pUser->pED->schemeBufferUser;
			if((remote && !summon && sbu.remoteBuffList.size() > 0) || 
				(remote && summon && sbu.remoteSummonBuffList.size() > 0) ||
				(!remote && !summon && sbu.buffList.size() > 0) ||
				(!remote && summon && sbu.summonBuffList.size() > 0) )
			{
				if(pUser->inventory.HaveItemByClassID(m_UseListPrice.itemId, m_UseListPrice.amount))
				{
					pUser->DeleteItemInInventory(m_UseListPrice.itemId, m_UseListPrice.amount);
				}else
				{
					pUser->SendSystemMessage(L"You don't have enogh items to use buff list.");
					unguard;
					return false;
				}
			}else
			{
				pUser->SendSystemMessage(L"Choosen scheme buff list is empty.");
				unguard;
				return false;
			}
		}
		pUser->QueueBuffList(summon, remote);
	}
	unguard;
	return false;
}

bool CSchemeBuffer::RequestClearList(User *pUser, bool summon, bool remote)
{
	guard;
	if(m_Enabled && CanUse(pUser))
	{
		SchemeBufferUser& sbu = pUser->pED->schemeBufferUser;
		sbu.lock.Enter();
		vector<SkillPchId>& list = sbu.GetList(summon, remote);
		if(list.size() > 0)
		{
			list.clear();
		}
		sbu.saveList = true;
		sbu.lock.Leave();
		pUser->SendSystemMessage(L"The buff list has been cleared.");
	}
	unguard;
	return false;
}

bool CSchemeBuffer::RequestRecovery(User *pUser, bool summon)
{
	guard;
	if(m_Enabled && CanUse(pUser))
	{
		if(summon)
		{
			if(CSummon *pSummon = pUser->GetSummonOrPet())
			{
				if(pSummon->pSD->nAlive)
				{
					if(pUser->IsInRange(pSummon, 250))
					{
						pSummon->pSD->fHP = pSummon->pSD->MaxHP;
						pSummon->pSD->fMP = pSummon->pSD->MaxMP;
						pSummon->UpdateStatus(CCreature::UPDATE_HP);
						pSummon->UpdateStatus(CCreature::UPDATE_MP);
					}
				}
			}
		}else
		{
			pUser->pSD->fHP = pUser->pSD->MaxHP;
			pUser->pSD->fMP = pUser->pSD->MaxMP;
			pUser->pSD->fCP = pUser->pSD->MaxCP;
			pUser->UpdateStatus(User::UPDATE_HP);
			pUser->UpdateStatus(User::UPDATE_CP);
			pUser->UpdateStatus(User::UPDATE_MP);
		}
	}
	unguard;
	return false;
}

bool CSchemeBuffer::RequestDispelAll(User *pUser, bool summon)
{
	guard;
	if(m_Enabled && CanUse(pUser))
	{
		if(summon)
		{
			if(CSummon *pSummon = pUser->GetSummonOrPet())
			{
				if(pSummon->pSD->nAlive)
				{
					if(pUser->IsInRange(pSummon, 250))
					{
						pSummon->DispelAllByGM();
					}
				}
			}
		}else
		{
			pUser->DispelAllByGM();
		}
	}
	unguard;
	return false;
}