#include "StdAfx.h"

map<INT32, DWORD> g_ItemNicknameColor;
map<INT32, DWORD> g_ItemTitleColor;
map<INT32, ItemExSkillList> g_ItemExSkillList;

CItemDBEx::CItemDBEx()
{

}

CItemDBEx::~CItemDBEx()
{

}

void CItemDBEx::Init()
{
	Clear();
	ReadData();
	LoadItemName();
	LoadItemGrp();
}

void CItemDBEx::Clear()
{
	Lock();
	g_ItemNicknameColor.clear();
	g_ItemTitleColor.clear();
	g_ItemExSkillList.clear();
	m_ItemName.clear();
	m_ItemSA.clear();
	m_ItemIcon.clear();
	m_LifeTime.clear();
	for(map<int, CItemDataEx*>::iterator Iter = mData.begin();Iter!=mData.end();Iter++)
	{
		if(Iter->second)
			delete Iter->second;
	}
	mData.clear();
	Unlock();
}

void CItemDBEx::LoadItemGrp()
{
	guard;
	wstringstream file(ReadFileW(L"..\\Script\\ItemGrp.txt"));
	wstring line;
	while(getline(file, line))
	{
		if(line.find(L"\\") == 0)
		{
			continue;
		}

		//2	17	0	3	8	4	0	dropitems.drop_quiver_m00			dropitemstex.drop_quiver_t00			icon.etc_wooden_quiver_i00					-1	6	13	0	0	1	LineageWeapons.wooden_arrow_m00_et	1	LineageWeaponsTex.wooden_arrow_t00_et	ItemSound.itemdrop_arrow		2	2	0
		vector<wstring> parts = Parser::Split(line, L"\t");
		if(parts.size() > 13)
		{
			int itemId = _wtoi(parts[1].c_str());
			if(itemId > 0)
			{
				m_ItemIcon.insert(pair<INT32, wstring>(itemId, parts[13]));
			}
		}
	}

	
	g_Log.Add(CLog::Blue, "[%s] Loaded [%d] items.", __FUNCTION__, m_ItemIcon.size());

	unguard;
}
void CItemDBEx::LoadItemName()
{
	guard;
	wstringstream file(ReadFileW(L"..\\Script\\ItemName.txt"));
	wstring line;
	while(getline(file, line))
	{
		if(line.find(L"\\") == 0)
		{
			continue;
		}

		//687	Darin's Letter		a,The love letter that Darin has written to Roxxy. Darin has asked you to deliver this letter to Gatekeeper Roxxy.\0	-1	a,	a,	a,	a,	0	0	0	a,

		vector<wstring> parts = Parser::Split(line, L"\t");
		if(parts.size() > 2)
		{
			int itemId = _wtoi(parts[0].c_str());
			if(itemId > 0)
			{
				m_ItemName.insert(pair<INT32, wstring>(itemId, parts[1]));
				m_ItemSA.insert(pair<INT32, wstring>(itemId, parts[2]));
			}
		}
	}
	g_Log.Add(CLog::Blue, "[%s] Loaded [%d] items.", __FUNCTION__, m_ItemName.size());
	unguard;
}

wstring CItemDBEx::GetItemName(int itemId)
{
	map<INT32, wstring>::iterator iter = m_ItemName.find(itemId);
	if(iter != m_ItemName.end())
	{
		return iter->second;
	}
	return L"NoName";
}

wstring CItemDBEx::GetItemSA(int itemId)
{
	map<INT32, wstring>::iterator it = m_ItemSA.find(itemId);
	if(it != m_ItemSA.end())
	{
		return it->second;
	}
	return L"";
}

wstring CItemDBEx::GetItemIcon(int itemId)
{
	map<INT32, wstring>::iterator it = m_ItemIcon.find(itemId);
	if(it != m_ItemIcon.end())
	{
		return it->second;
	}
	return L"";
}

void CItemDBEx::ReadData()
{
	ifstream file("..\\Script\\ItemDataEx.txt");
	if(file)
	{
		Lock();
		string sLine;
		while(getline(file, sLine))
		{
			if(sLine[0] == '/' || sLine[0] == ';')
			{
				continue;
			}
			if(sLine.find("begin") == 0)
			{
				int ItemID = CParser::GetValueInt(sLine, "item_id");
				UINT lifeTime = CParser::GetValueInt(sLine, "life_time");
				int ShadowTime = CParser::GetValueInt(sLine, "shadow_time");
				int ReuseGroup = CParser::GetValueInt(sLine, "reuse_group", -1);
				int maxStackableLevel = CParser::GetValueInt(sLine, "max_stackable_level", 0);
				int stackType = Parser::ParseOptionInt(sLine, "stack_type", 0);
				DWORD nicknameColor = CParser::GetValueInt(sLine, "nickname_color", 0);
				DWORD titleColor = CParser::GetValueInt(sLine, "title_color", 0);
				string skillList = CParser::GetValueString(sLine, "skill_list");
				if(skillList.size() > 3)
				{
					ItemExSkillList iesl;

					skillList = CParser::Replace(skillList, '{', ' ');
					skillList = CParser::Replace(skillList, '}', ' ');
					skillList = CParser::Replace(skillList, ';', ' ');
					wstring skillListW = StringToWString(skillList);
					wstringstream nameStream;
					nameStream << skillListW;
					wstring skillName;
					while(nameStream >> skillName)
					{
						int skillId = 0, level = 0;
						g_SkillDB.GetSkillIdLevelByName((PWCHAR)skillName.c_str(), &skillId, &level);
						if(skillId > 0 && level > 0)
						{
							CSkillKey key(skillId, level);
							iesl.m_Skills.push_back(key);
						}
					}
					if(iesl.m_Skills.size() > 0)
					{
						iesl.m_maxStackableLevel = maxStackableLevel;
						iesl.m_stackType = stackType;
						g_ItemExSkillList.insert(pair<INT32, ItemExSkillList>(ItemID, iesl));
					}
				}
				if(ItemID)
				{
					CItemDataEx *pData = new CItemDataEx();
					if(pData)
					{
						if(lifeTime > 0)
						{
							m_LifeTime[ItemID] = lifeTime;
						}
						if(nicknameColor != 0)
						{
							g_ItemNicknameColor.insert(pair<INT32, DWORD>(ItemID, nicknameColor));
						}
						if(titleColor != 0)
						{
							g_ItemTitleColor.insert(pair<INT32, DWORD>(ItemID, titleColor));
						}
						pData->Set(ItemID, ShadowTime, ReuseGroup);
						mData.insert(pair<int, CItemDataEx*>(ItemID, pData));
					}else
						g_Log.Add(CLog::Error, "[%s] Cannot allocate memory for CItemDataEx!",__FUNCTION__);
				}

			}
		}
		Unlock();
		g_Log.Add(CLog::Blue, "[%s] Done. [%d] Items in database and [%d] item with skills.", __FUNCTION__, mData.size(), g_ItemExSkillList.size());
	}else
		g_Log.Add(CLog::Error, "[%s] missing file [..\\Script\\ItemDataEx.txt]!", __FUNCTION__);
}

CItemDataEx* CItemDBEx::GetItemDataEx(int item_id)
{
	CItemDataEx *pData = NULL;
	map<int, CItemDataEx*>::iterator Iter = mData.find(item_id);
	if(Iter!=mData.end())
		pData = Iter->second;
	return pData;
}

UINT CItemDBEx::GetLifeTime(int itemId)
{
	guard;
	UINT ot = 0;
	map<INT32, UINT>::iterator it = m_LifeTime.find(itemId);
	if(it!=m_LifeTime.end())
	{
		ot = it->second;
	}
	unguard;
	return ot;
}