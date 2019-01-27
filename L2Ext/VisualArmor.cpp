#include "stdafx.h"
#include "VisualArmor.h"
#include "HtmlCache.h"
#include "PlayerAction.h"
#include "DB.h"

CVisualArmor g_VisualArmor;

CVisualArmor::CVisualArmor() : m_Enabled(FALSE), m_ReloadTick(0)
{
}

CVisualArmor::~CVisualArmor()
{
}

void CVisualArmor::Init()
{
	m_ReloadTick = 0;
	LoadData();
	if(m_Enabled)
	{
		g_Log.Add(CLog::Blue, "[%s] Feature is enabled, loaded[%d] armor(s).", __FUNCTION__, m_Armors.size());
	}
}

void CVisualArmor::TimerExpired()
{
	if(m_ReloadTick > 0)
	{
		if(m_ReloadTick < time(0))
		{
			Init();
		}
	}
}

void CVisualArmor::LoadData()
{
	wstringstream file(ReadFileW(L"..//Script//VisualArmor.txt"));
	wstring line;
	m_Lock.Enter();
	m_Armors.clear();
	while(getline(file, line))
	{
		if(line.find(L"//") == 0)
		{
			continue;
		}
		if(line.find(L"setting_begin") == 0)
		{
			m_Enabled = Parser::ParseOptionInt(line, L"enabled") ? true : false;
			m_CheckBodyParts = Parser::ParseOptionInt(line, L"check_body_parts") ? true : false;

		}else if(line.find(L"armor_begin") == 0)
		{
			VisualArmorInfo* pV = new VisualArmorInfo();
			UINT id = Parser::ParseOptionInt(line, L"id", 0);
			wstring armorType = Parser::ParseOptionString(line, L"armor_type", L"armor_none");
			if(armorType == L"armor_light")
			{
				pV->armorType = ArmorLight;
			}else if(armorType == L"armor_heavy")
			{
				pV->armorType = ArmorHeavy;
			}else if(armorType == L"armor_magic")
			{
				pV->armorType = ArmorMagic; 
			}else if(armorType == L"armor_none")
			{
				pV->armorType = ArmorNone;
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Invalid armor_type[%S] (available: armor_light, armor_heavy, armor_magic, armor_none)!", __FUNCTION__, armorType.c_str());
				pV->armorType = ArmorNone;
			}
			pV->chestSlotId = Parser::ParseOptionInt(line, L"slot_chest");
			pV->legsSlotId = Parser::ParseOptionInt(line, L"slot_legs");
			pV->glovesSlotId = Parser::ParseOptionInt(line, L"slot_gloves");
			pV->feetSlotId = Parser::ParseOptionInt(line, L"slot_feet");
			pV->minLevel = Parser::ParseOptionInt(line, L"min_level", 0);
			pV->maxLevel = Parser::ParseOptionInt(line, L"max_level", 80);
			pV->price.itemId = Parser::ParseOptionInt(line, L"price_id");
			pV->price.amount = Parser::ParseOptionInt(line, L"price_amount");

			if(id > 0)
			{
				m_Armors[id] = pV;
			}
		}
	}
	m_Lock.Leave();
	//armor_begin	id=1	armor_type=armor_light	slot_chest=6379	slot_legs=0	slot_gloves=6380	slot_feet=6381 min_level=76	max_level=80	armor_end

}

VisualArmorInfo* CVisualArmor::Get(UINT armorId)
{
	guard;
	VisualArmorInfo* ret = 0;
	m_Lock.Enter();
	map<UINT, VisualArmorInfo*>::iterator it = m_Armors.find(armorId);
	if(it != m_Armors.end())
	{
		ret = it->second;
	}
	m_Lock.Leave();
	unguard;
	return ret;
}

void CVisualArmor::ShowMenu(User *pUser)
{
	if(m_Enabled)
	{
		const WCHAR* wHtml = g_HtmlCache.Get(L"visual_armor_menu.htm");
		if(wHtml)
		{
			pUser->ShowHTML(L"visual_armor_menu.htm", wHtml, 0);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] cannot find visual_armor_menu.htm", __FUNCTION__);
		}
	}
}

void CVisualArmor::OnRequestEquip(User *pUser)
{
	if(m_Enabled)
	{
		const WCHAR* wHtml = g_HtmlCache.Get(L"visual_armor_list.htm");
		if(wHtml)
		{
			wstring html(wHtml);
			//<template><a action="bypass -h va_equip?id=$armor_id">$armor_name</a><br></temblate>
			wstring templateLine;
			size_t startPos = html.find(L"<template>");
			if(startPos != wstring::npos)
			{
				size_t endPos = html.find(L"</template>", startPos);
				if(endPos != wstring::npos)
				{
					templateLine = html.substr(startPos + 10, (endPos - startPos - 10));
					wstringstream lines;
					m_Lock.Enter();
					for(map<UINT, VisualArmorInfo*>::iterator it = m_Armors.begin(); it!=m_Armors.end();it++)
					{
						wstring line = templateLine;
						line = Utils::ReplaceString(line, L"$armor_name", g_ItemDBEx.GetItemName(it->second->chestSlotId), true);
						WCHAR temp[32];
						temp[0] = 0;
						_itow_s(it->first, temp, 31, 10);
						line = Utils::ReplaceString(line, L"$armor_id", temp, true);
						lines << line << endl;
					}
					m_Lock.Leave();
					html = html.replace(startPos, (endPos + 11 - startPos), lines.str());
				}
			}
			pUser->ShowHTML(L"visual_armor_list.htm", html.c_str(), 0);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] cannot find visual_armor_list.htm", __FUNCTION__);
		}
	}
}

void CVisualArmor::OnEquip(User *pUser, UINT armorId)
{
	guard;
	if(m_Enabled)
	{
		if(pUser->pED->visualArmorUser.floodTimestamp < time(0))
		{
			if(pUser->olympiadUser.GetState() != 0)
			{
				pUser->SendSystemMessage(L"You cannot perform this action while participing in Olympiad!");
				unguard;
				return;
			}
			if(pUser->IsInCombatMode() || pUser->pSD->bGuilty)
			{
				pUser->SendSystemMessage(L"You cannot perform this action in combat mode!");
				unguard;
				return;
			}
			if(pUser->IsNowTrade() || pUser->pSD->nStoreMode != 0)
			{
				pUser->SendSystemMessage(L"You cannot perform this action while operating a private store or trading!");
				unguard;
				return;
			}
			
			if(VisualArmorInfo* pV = Get(armorId))
			{
				if(pUser->pSD->nLevel >= pV->minLevel)
				{
					if(pUser->pSD->nLevel <= pV->maxLevel)
					{
						if(pV->price.itemId > 0 && pV->price.amount > 0)
						{
							if(!pUser->DeleteItemInInventory(pV->price.itemId, pV->price.amount))
							{
								pUser->SendSystemMessage(L"You don't have enough item(s) to use this Visual Armor!");
								unguard;
								return;
							}
						}
						pUser->pED->visualArmorUser.floodTimestamp = time(0) + 10;
						VisualArmorUser& vUser = pUser->pED->visualArmorUser;
						vUser.armorId = armorId;
						vUser.armorType = pV->armorType;
						vUser.chestSlotId = pV->chestSlotId;
						vUser.feetSlotId = pV->feetSlotId;
						vUser.glovesSlotId = pV->glovesSlotId;
						vUser.legsSlotId = pV->legsSlotId;
						g_DB.RequestSaveVisualArmor(pUser->nDBID, armorId);
						PlayerAction::OnTeleportToLocation(pUser, static_cast<int>(pUser->pSD->Pos.x), static_cast<int>(pUser->pSD->Pos.y), static_cast<int>(pUser->pSD->Pos.z), 0);
						pUser->SendSystemMessage(L"You have equipped a Visual Armor.");
					}else
					{
						pUser->SendSystemMessage(L"Your level is too high to equip this armor!");
					}
				}else
				{
					pUser->SendSystemMessage(L"Your level is too low to equip this armor!");
				}
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Invalid armor[%d] selected by user[%S]!", __FUNCTION__, armorId, pUser->pSD->wszName);
			}
		}else
		{
			pUser->SendSystemMessage(L"You have to wait at least 10 seconds before you try to change a visual armor.");
		}
	}
	unguard;
}

void CVisualArmor::OnUnequip(User *pUser)
{
	guard;
	if(m_Enabled)
	{
		if(pUser->pED->visualArmorUser.floodTimestamp < time(0))
		{
			if(pUser->olympiadUser.GetState() != 0)
			{
				pUser->SendSystemMessage(L"You cannot perform this action while participing in Olympiad!");
				unguard;
				return;
			}
			if(pUser->IsInCombatMode() || pUser->pSD->bGuilty)
			{
				pUser->SendSystemMessage(L"You cannot perform this action in combat mode!");
				unguard;
				return;
			}
			if(pUser->IsNowTrade() || pUser->pSD->nStoreMode != 0)
			{
				pUser->SendSystemMessage(L"You cannot perform this action while operating a private store or trading!");
				unguard;
				return;
			}
			if(pUser->pED->visualArmorUser.armorId > 0)
			{
				pUser->pED->visualArmorUser.floodTimestamp = time(0) + 10;
				pUser->pED->visualArmorUser.Clear();
				PlayerAction::OnTeleportToLocation(pUser, static_cast<int>(pUser->pSD->Pos.x), static_cast<int>(pUser->pSD->Pos.y), static_cast<int>(pUser->pSD->Pos.z), 0);
				pUser->SendSystemMessage(L"The Visual Armor has been removed.");
			}else
			{
				pUser->SendSystemMessage(L"There is nothing to unequip!");
			}
		}else
		{
			pUser->SendSystemMessage(L"You have to wait at least 10 seconds before you try to change a visual armor.");
		}
	}
	unguard;
}