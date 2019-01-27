#include "StdAfx.h"
#include "ArmorPenalty.h"

CArmorMasteryDB g_ArmorMasteryDB;
CArmorPenalty g_ArmorPenalty;



CArmorMasteryDB::CArmorMasteryDB() 
{
	list<int> lData;
	mData.insert(pair<ArmorMasteryType, list<int>>(LIGHT_MASTERY, lData));
	mData.insert(pair<ArmorMasteryType, list<int>>(HEAVY_MASTERY, lData));
	mData.insert(pair<ArmorMasteryType, list<int>>(ROBE_MASTERY, lData));
};
void CArmorMasteryDB::ReadData()
{
	ifstream file("..\\Script\\ArmorPenaltyData.txt");
	if(file)
	{
		mData[LIGHT_MASTERY].clear();
		mData[HEAVY_MASTERY].clear();
		mData[ROBE_MASTERY].clear();
		string sLine;
		while(getline(file, sLine))
		{
			//ParseData
			if(sLine.find("begin") == 0)
			{
				int nSkillID = CParser::GetValueInt(sLine, "skill_id");
				string sType = CParser::GetValueString(sLine, "armor_type");
				if(nSkillID)
				{
					if(sType == "ROBE_MASTERY")
					{
						mData[ROBE_MASTERY].push_back(nSkillID);
					}else if(sType == "LIGHT_MASTERY")
					{
						mData[LIGHT_MASTERY].push_back(nSkillID);
					}else if(sType == "HEAVY_MASTERY")
					{
						mData[HEAVY_MASTERY].push_back(nSkillID);
					}else
						g_Log.Add(CLog::Error, "[%s] Invalid armor type[%s]", __FUNCTION__, sType.c_str());
				}
			}
		}
		file.close();
	}else
		g_Log.Add(CLog::Error, "[%s] Cannot find file[..\\Script\\ArmorPenaltyData.txt]", __FUNCTION__);
}
bool CArmorMasteryDB::CanEquip(User *pUser, int Type)
{
	switch(Type)
	{
	case ROBE_MASTERY:
		{
			for(list<int>::iterator Iter = mData[ROBE_MASTERY].begin(); Iter!=mData[ROBE_MASTERY].end();Iter++)
			{
				int nSkillID = *Iter;
				if(pUser->GetAcquiredSkillLevel(nSkillID) > 0)
				{
					return true;
				}
			}
			break;
		}
	case LIGHT_MASTERY:
		{
			for(list<int>::iterator Iter = mData[LIGHT_MASTERY].begin(); Iter!=mData[LIGHT_MASTERY].end();Iter++)
			{
				int nSkillID = *Iter;
				if(pUser->GetAcquiredSkillLevel(nSkillID) > 0)
				{
					return true;
				}
			}
			break;
		}
	case HEAVY_MASTERY:
		{
			for(list<int>::iterator Iter = mData[HEAVY_MASTERY].begin(); Iter!=mData[HEAVY_MASTERY].end();Iter++)
			{
				int nSkillID = *Iter;
				if(pUser->GetAcquiredSkillLevel(nSkillID) > 0)
				{
					return true;
				}
			}
			break;
		}
	default:
		{
			g_Log.Add(CLog::Blue, "[%s] Invalid Armor Type[%d]", __FUNCTION__, Type);
			return true;
			break;
		}
	}
	return false;
};
void CArmorPenalty::Initialize()
{
	const TCHAR* section = _T("ArmorPenalty");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), FALSE, g_ConfigFile);
	if(m_Enabled)
	{
		m_SkillID = GetPrivateProfileInt(section, _T("SkillID"), 0, g_ConfigFile);
		m_PenaltyForMixedItems = GetPrivateProfileInt(section, _T("PenaltyForMixedItems"), 0, g_ConfigFile);
		g_Log.Add(CLog::Blue, "[%s] Initializing Armor Penalty", __FUNCTION__);
	}
}

void CArmorPenalty::Activate(User *pUser)
{
	if(m_SkillID > 0)
	{
		if(!pUser->pSD->pExData->ArmorPenalty)
		{
			pUser->pSD->pExData->ArmorPenalty = true;
			pUser->AcquireSkill(m_SkillID, 1);
			pUser->pSocket->SendSystemMessage(L"You can feel The Armor Penalty effect.");
			pUser->SendETCBuffStatus();
		}
	}
}

void CArmorPenalty::Deactivate(User *pUser)
{
	if(m_SkillID > 0)
	{
		if(pUser->pSD->pExData->ArmorPenalty)
		{
			pUser->pSD->pExData->ArmorPenalty = false;
			if(pUser->HaveSkill(m_SkillID, 1))
			{
				pUser->DeleteSkill(m_SkillID);
			}
			pUser->pSocket->SendSystemMessage(L"The Armor Penalty effect has been removed.");
			pUser->SendETCBuffStatus();
		}
	}
}

void CArmorPenalty::Validate(User *pUser)
{
	if(m_Enabled)
	{
		if(m_PenaltyForMixedItems)
		{
			bool activatePenalty = false;
			UINT armorType = ArmorNone;
			for(UINT n=0; n < 18; n++)
			{
				int itemIndex = pUser->pSD->nEquipment[n];
				if(itemIndex > 0)
				{
					if(CItem *pItem = pUser->inventory.GetItemByIndex(itemIndex))
					{
						if(pItem->pII->armorType > 0)
						{
							if(armorType == 0)
							{
								armorType = pItem->pII->armorType;
							}else
							{
								//check if armorType matches the item
								if(armorType != pItem->pII->armorType)
								{
									activatePenalty = true;
									break;
								}
							}
						}
					}
				}
			}

			if(activatePenalty)
			{
				if(pUser->pSD->pExData->ArmorPenalty == false)
				{
					Activate(pUser);
				}
			}else
			{
				if(pUser->pSD->pExData->ArmorPenalty)
				{
					Deactivate(pUser);
				}
			}
		}else
		{
			if(!CCategoryDataDB::IsInCategory(pUser->pSD->nClass, CCategoryDataDB::FIRST_CLASS_GROUP))
			{
				int nItemIndex = pUser->pSD->nEquipment[CItem::Chest];
				if(nItemIndex > 0)
				{
					CItem *pArmor = pUser->GetInventory()->GetItemByIndex(nItemIndex);
					if(pArmor->IsValidItem())
					{
						if(pArmor->pII->nCrystalType > CrystalD)
						{
							if(g_ArmorMasteryDB.CanEquip(pUser, pArmor->pII->armorType))
							{
								nItemIndex = pUser->pSD->nEquipment[CItem::Legs];
								if(nItemIndex > 0)
								{
									CItem *pPants = pUser->GetInventory()->GetItemByIndex(nItemIndex);
									if(pPants->IsValidItem())
									{
										if(pArmor->pII->armorType != pPants->pII->armorType)
										{
											if(pPants->pII->nCrystalType > CrystalD)
											{
												if(g_ArmorMasteryDB.CanEquip(pUser, pPants->pII->armorType))
												{
													Deactivate(pUser);
													return;
												}else
												{
													Activate(pUser);
													return;
												}
											}else
											{
												Deactivate(pUser);
												return;
											}
										}
									}
								}
							}else
							{
								Activate(pUser);
								return;
							}
						}else
						{
							Deactivate(pUser);
							return;
						}
					}
				}
				nItemIndex = pUser->pSD->nEquipment[CItem::OnePiece];
				if(nItemIndex > 0)
				{
					CItem *pArmor = pUser->GetInventory()->GetItemByIndex(nItemIndex);
					if(pArmor->IsValidItem())
					{
						if(pArmor->pII->nCrystalType > CrystalD)
						{
							if(g_ArmorMasteryDB.CanEquip(pUser, pArmor->pII->armorType))
							{
								Deactivate(pUser);
								return;
							}else
							{
								Activate(pUser);
								return;
							}
						}else
						{
							Deactivate(pUser);
							return;
						}
					}
				}
				Deactivate(pUser);
				return;
			}
		}
	}
};