#include "StdAfx.h"

map<INT32, INT32> g_MaxSkillLevel;

CSkillDBEx::~CSkillDBEx()
{
	map<CSkillKey, CSkillInfoEx*>::iterator Iter = mData.begin();
	for(;Iter!=mData.end();Iter++)
	{
		delete Iter->second;
		Iter->second = NULL;
	}
}

void CSkillDBEx::ReadData2()
{
	//[s_power_strike12] = 770
	wstring wData = ReadFileW(L"..\\Script\\Skill_Pch.txt");
	wstringstream wsstr;
	wsstr << wData;
	wstring wLine;
	int Skills = 0;
	int Levels = 0;
	while(getline(wsstr, wLine))
	{
		if(wLine.find(L"//") == 0)
			continue;

		wstringstream skillData;
		wstring lineData = Parser::Replace(wLine, L'=', L' ');
		skillData << lineData;
		wstring wName;
		UINT SkillNameID = 0;
		skillData >> wName >> SkillNameID;

		if(SkillNameID > 0)
		{
			Levels++;
			SkillPchId key(SkillNameID);
			map<INT32, INT32>::iterator maxLevel = g_MaxSkillLevel.find(key.skillId);
			if(maxLevel != g_MaxSkillLevel.end())
			{
				if(maxLevel->second < key.level)
				{
					maxLevel->second = key.level;
				}
			}else
			{
				g_MaxSkillLevel.insert(pair<INT32, INT32>(key.skillId, key.level));
			}

			map<int, map<int, wstring>>::iterator Iter = mData2.find(key.skillId);
			if(Iter != mData2.end())
			{
				Skills++;
				Iter->second.insert(pair<int, wstring>(key.level, wName));
			}else
			{
				map<int, wstring> mSkill;
				mSkill.insert(pair<int, wstring>(key.level, wName));
				mData2.insert(pair<int, map<int, wstring>>(key.skillId, mSkill));
			}
		}
	}
	g_Log.Add(CLog::Blue, "[%s] Loaded [%d] skills and [%d] levels.", __FUNCTION__, Skills, Levels);
}

int CSkillDBEx::GetMaxSkillLevel(int SkillID)
{
	map<INT32, INT32>::iterator maxLevel = g_MaxSkillLevel.find(SkillID);
	if(maxLevel != g_MaxSkillLevel.end())
	{
		return maxLevel->second;
	}
	return 0;
}

int CSkillDBEx::GetMaxNotEnchantedSkillLevel(int SkillID)
{
	map<int, map<int, wstring>>::iterator Iter = mData2.find(SkillID);
	if(Iter!=mData2.end())
	{
		int nLevel = -1;
		for(map<int, wstring>::iterator LevelIter = Iter->second.begin();LevelIter!= Iter->second.end();LevelIter++)
		{
			if(LevelIter->first > nLevel)
			{
				if(LevelIter->first < 100)
				{
					nLevel = LevelIter->first;
				}else
					return nLevel;
			}
		}
		return nLevel;
	}
	return -1;
}

void CSkillDBEx::ReadData()
{
	ifstream file("..\\Script\\SkillDataEx.txt");
	if(file)
	{
		string sLine;
		while(getline(file, sLine))
		{
			if(sLine.find("begin") == 0)
			{
				//reading stuff
				string sName = CParser::GetValueString(sLine, "skill_name");
				int nID = CParser::GetValueInt(sLine, "skill_id");
				int nLevel = CParser::GetValueInt(sLine, "level");
				double SoulshotBonus = CParser::GetValueDouble(sLine, "soulshot_bonus");
				bool AvoidShieldBlock = CParser::GetValueInt(sLine, "avoid_shield_block");
				bool MagicShieldBlock = CParser::GetValueInt(sLine, "magic_shield_block");
				bool DirectToHp = CParser::GetValueInt(sLine, "direct_to_hp");
				bool GeoDataTest = CParser::GetValueInt(sLine, "geodata_test");
				int StaticReuseTime = CParser::GetValueInt(sLine, "static_reuse_time");
				CSkillInfoEx *pData = new CSkillInfoEx();
				if(pData)
				{
					pData->name = sName;
					pData->id = nID;
					pData->level = nLevel;
					pData->soulshotBonus = SoulshotBonus;
					pData->avoidShieldBlock = AvoidShieldBlock;
					pData->magicShieldBlock = MagicShieldBlock;
					pData->directToHp = DirectToHp;
					pData->geoDataTest = GeoDataTest;
					pData->staticReuseTime = StaticReuseTime;
					CSkillKey key(nID, nLevel);
					mData.insert(pair<CSkillKey, CSkillInfoEx*>(key, pData));
				}else
					g_Log.Add(CLog::Error, "[%s] Cannot allocate memory for pData", __FUNCTION__);
			}
		}
		g_Log.Add(CLog::Blue, "[%s] Loaded [%d] skills.", __FUNCTION__, mData.size());
	}else
		g_Log.Add(CLog::Error, "[%s] SkillDataEx.txt cannot be found!", __FUNCTION__);
}

void CSkillDBEx::Initialize()
{ 
	g_Log.Add(CLog::Blue, "[%s] Reading Data", __FUNCTION__); 
	ReadData();
	ReadData2();
}

CSkillInfoEx *CSkillDBEx::GetSkillInfoEx(int nSkillID, int nLevel)
{
	CSkillInfoEx *pData = NULL;
	CSkillKey key(nSkillID, nLevel);
	map<CSkillKey, CSkillInfoEx*>::iterator Iter = mData.find(key);
	if(Iter!=mData.end())
	{
		pData = Iter->second;
	}else
	{
		CSkillKey key0(nSkillID, 0);
		Iter = mData.find(key0);
		if(Iter!=mData.end())
			return Iter->second;
	}
	return pData;
};