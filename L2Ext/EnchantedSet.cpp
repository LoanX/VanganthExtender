#include "StdAfx.h"
#include "EnchantedSet.h"
#include "ObjectDB.h"

void CEnchantedSet::Initialize()
{
	ReadData("..\\Script\\EnchantedSetData.txt");
}

void CEnchantedSet::ReadData(string sFile)
{
	ifstream file(sFile.c_str());
	if(file)
	{
		string sLine;
		int nLine = 0;
		while(getline(file, sLine))
		{
			nLine++;
			//Parse data
			if(sLine.find("set_begin") == 0)
			{
				int nSetID = CParser::GetValueInt(sLine, "set_id");
				string sGrade = CParser::GetValueString(sLine, "set_grade");
				string sType = CParser::GetValueString(sLine, "set_type");
				EnchantedSetInfo* pInfo = new EnchantedSetInfo;
				memset(pInfo, 0, sizeof(EnchantedSetInfo));
				pInfo->set_id = nSetID;
				if(sGrade == "d")
				{
					if(sType == "heavy")
						pInfo->skill_id = ENCHANTED_SET_D_HEAVY;
					else if(sType == "light")
						pInfo->skill_id = ENCHANTED_SET_D_LIGHT;
					else if(sType == "robe")
						pInfo->skill_id = ENCHANTED_SET_D_ROBE;
				}else if(sGrade == "c")
				{
					if(sType == "heavy")
						pInfo->skill_id = ENCHANTED_SET_C_HEAVY;
					else if(sType == "light")
						pInfo->skill_id = ENCHANTED_SET_C_LIGHT;
					else if(sType == "robe")
						pInfo->skill_id = ENCHANTED_SET_C_ROBE;
				}else if(sGrade == "b")
				{
					if(sType == "heavy")
						pInfo->skill_id = ENCHANTED_SET_B_HEAVY;
					else if(sType == "light")
						pInfo->skill_id = ENCHANTED_SET_B_LIGHT;
					else if(sType == "robe")
						pInfo->skill_id = ENCHANTED_SET_B_ROBE;
				}else if(sGrade == "a")
				{
					if(sType == "heavy")
						pInfo->skill_id = ENCHANTED_SET_A_HEAVY;
					else if(sType == "light")
						pInfo->skill_id = ENCHANTED_SET_A_LIGHT;
					else if(sType == "robe")
						pInfo->skill_id = ENCHANTED_SET_A_ROBE;
				}else if(sGrade == "s")
				{
					if(sType == "heavy")
						pInfo->skill_id = ENCHANTED_SET_S_HEAVY;
					else if(sType == "light")
						pInfo->skill_id = ENCHANTED_SET_S_LIGHT;
					else if(sType == "robe")
						pInfo->skill_id = ENCHANTED_SET_S_ROBE;
				}
				if(pInfo->set_id > 0 && pInfo->skill_id > 0)
				{
					mData.insert(pair<int, EnchantedSetInfo*>(pInfo->set_id, pInfo));
				}else
				{
					g_Log.Add(CLog::Error, "[%s] Invalid set data - line[%d]", __FUNCTION__, nLine);
				}
			}
		}
		file.close();
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Cannot find file [%s]", __FUNCTION__, sFile.c_str());
	}
};
void CEnchantedSet::Validate(User *pUser)
{
	if(pUser->ValidUser())
	{
		if(pUser->currentSetID > 0)
		{
			CSetObject *pSetObj = g_ObjectDB.GetSetObject(pUser->currentSetID);
			if(pSetObj)
			{
				bool Bonus = true;
				for(int n=0;n<18;n++)
				{
					if(n > 0 && n == pSetObj->nAdditionalSlotType)
						continue;

					if(pSetObj->nEquipmentClassId[n] > 0)
					{
						if(pUser->pSD->nEquipment[n] > 0)
						{
							CItem *pItem = pUser->inventory.GetItemByIndex(pUser->pSD->nEquipment[n]);
							if(pItem->IsValidItem())
							{
								if(pItem->pSID->nEnchantLevel < 6)
								{
									Bonus = false;
									break;
								}
							}
						}
					}
				}

				if(Bonus)
				{
					map<int, EnchantedSetInfo*>::iterator Iter = mData.find(pUser->currentSetID);
					if(Iter!=mData.end())
					{
						pUser->AttachSkill(Iter->second->skill_id, 1, OperatePassive);
					}
				}
			}else
				g_Log.Add(CLog::Error, "[%s] Cannot find set[%d] object.", __FUNCTION__, pUser->currentSetID);
		}
	}
}