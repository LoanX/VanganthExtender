#include "StdAfx.h"
#include "Augmentation.h"
#include "DB.h"

INT32 g_CancelPriceC = 15000;
INT32 g_CancelPriceB = 240000;
INT32 g_CancelPriceA = 390000;
INT32 g_CancelPriceS = 480000;

UINT g_WareHouseAugmentationId[32];
UINT g_WareHouseProtectionTimeout[32];

CAugmentation g_Augmentation;

void CAugmentation::Initialize()
{
	InitRates();

	memset(g_WareHouseAugmentationId, 0, sizeof(g_WareHouseAugmentationId));
	memset(g_WareHouseProtectionTimeout, 0, sizeof(g_WareHouseProtectionTimeout));

	g_HookManager.WriteJump(0x83EB3A, PrivateStoreBuyCheckAugmentation, 0);
	g_HookManager.WriteCall(0x55338D, MultiSellDeleteItem, 0); //Fix for Adding /removing SA
	g_CancelPriceC = GetPrivateProfileInt(_T("Augmentation"), _T("CancelPriceC"), 15000, g_ConfigFile);
	g_CancelPriceB = GetPrivateProfileInt(_T("Augmentation"), _T("CancelPriceB"), 240000, g_ConfigFile);
	g_CancelPriceA = GetPrivateProfileInt(_T("Augmentation"), _T("CancelPriceA"), 390000, g_ConfigFile);
	g_CancelPriceS = GetPrivateProfileInt(_T("Augmentation"), _T("CancelPriceS"), 480000, g_ConfigFile);

	g_HookManager.WriteCall(0x54EE0C, DisassembleWareHouseItem, 0);	//list
	g_HookManager.WriteCall(0x54FFC3, DisassembleWareHouseItem, 0);	//withdraw
	g_HookManager.WriteCall(0x550190, WareHouseWithdrawItem, 0);
}

void CAugmentation::InitRates()
{
	SkillChance[LS_LOW] = g_Config.AugmentationInfo.GetSkillChance(CConfig::CAugmentationInfo::LOW);
	SkillChance[LS_MID] = g_Config.AugmentationInfo.GetSkillChance(CConfig::CAugmentationInfo::MID);
	SkillChance[LS_HIGH] = g_Config.AugmentationInfo.GetSkillChance(CConfig::CAugmentationInfo::HIGH);
	SkillChance[LS_TOP] = g_Config.AugmentationInfo.GetSkillChance(CConfig::CAugmentationInfo::TOP);
	BaseStatChance[LS_LOW] = g_Config.AugmentationInfo.GetBaseStatChance(CConfig::CAugmentationInfo::LOW);
	BaseStatChance[LS_MID] = g_Config.AugmentationInfo.GetBaseStatChance(CConfig::CAugmentationInfo::MID);
	BaseStatChance[LS_HIGH] = g_Config.AugmentationInfo.GetBaseStatChance(CConfig::CAugmentationInfo::HIGH);
	BaseStatChance[LS_TOP] = g_Config.AugmentationInfo.GetBaseStatChance(CConfig::CAugmentationInfo::TOP);

}

void CAugmentation::ReadSkillData()
{
	ifstream file("..\\Script\\augmentation_skill.txt");
	if(file)
	{
		mSkills.clear();
		string sLine;
		while(getline(file, sLine))
		{
			INT32 nSkillID = CParser::GetValueInt(sLine, "skill_id");
			INT32 nAugmentationID = CParser::GetValueInt(sLine, "augmentation_id");
			INT32 nLevel = CParser::GetValueInt(sLine, "level");
			AugmentSkill as;
			as.nAugmentID = nAugmentationID;
			as.nSkillID = nSkillID;
			as.nLevel = nLevel;
			mSkills.insert(pair<int, AugmentSkill>(nAugmentationID, as));
			if(nLevel==0)
				g_Log.Add(CLog::Error, "[%s] Skill level = 0 for augmentation[%d]!", __FUNCTION__, nAugmentationID);
		}
		g_Log.Add(CLog::Blue, "[%s] %d skills in database.", __FUNCTION__, mSkills.size());
	}else
		g_Log.Add(CLog::Error, "[%s]Augmentation Skill Data cannot be found!", __FUNCTION__);
	file.close();
}

void CAugmentation::ReadStatData()
{
	string Text;
	string Sign;
	double Value;
	int nAugmentID;
	ifstream file("..\\Script\\augmentation_stat.txt");
	if(file)
	{
		mStats.clear();
		while(file)
		{
			AugmentStat stat;
			file >> Text; //begin 
			if(!file)
				break;
			file >> Text; //AugmentationID
			file >> Sign; // =
			file >> nAugmentID; // value
			stat.nAugmentID = nAugmentID;
			file >> Text; //FirstEffect name
			file >> Sign; // =
			file >> Value; //Value
			if(Text=="stat_PDEF")
			{
				stat.daStat[AugmentStat::PDEF] = Value;
			}else if(Text=="stat_MDEF")
			{
				stat.daStat[AugmentStat::MDEF] = Value;
			}else if(Text=="stat_MAXHP")
			{
				stat.daStat[AugmentStat::MAXHP] = Value;
			}else if(Text=="stat_MAXMP")
			{
				stat.daStat[AugmentStat::MAXMP] = Value;
			}else if(Text=="stat_MAXCP")
			{
				stat.daStat[AugmentStat::MAXCP] = Value;
			}else if(Text=="stat_PATK")
			{
				stat.daStat[AugmentStat::PATK] = Value;
			}else if(Text=="stat_MATK")
			{
				stat.daStat[AugmentStat::MATK] = Value;
			}else if(Text=="stat_HPREGEN")
			{
				stat.daStat[AugmentStat::HPREGEN] = Value;
			}else if(Text=="stat_MPREGEN")
			{
				stat.daStat[AugmentStat::MPREGEN] = Value;
			}else if(Text=="stat_CPREGEN")
			{
				stat.daStat[AugmentStat::PDEF] = Value;
			}else if(Text=="stat_EVASION")
			{
				stat.daStat[AugmentStat::EVASION] = Value;
			}else if(Text=="stat_ACCURACY")
			{
				stat.daStat[AugmentStat::ACCURACY] = Value;
			}else if(Text=="stat_CRITICAL")
			{
				stat.daStat[AugmentStat::CRITICAL] = Value;
			}
			file >> Text; //Second effect name or end
			if(Text != "end") 
			{
				file >> Sign; // =
				file >> Value; // value
				if(Text=="stat_PDEF")
				{
					stat.daStat[AugmentStat::PDEF] += Value;
				}else if(Text=="stat_MDEF")
				{
					stat.daStat[AugmentStat::MDEF] += Value;
				}else if(Text=="stat_MAXHP")
				{
					stat.daStat[AugmentStat::MAXHP] += Value;
				}else if(Text=="stat_MAXMP")
				{
					stat.daStat[AugmentStat::MAXMP] += Value;
				}else if(Text=="stat_MAXCP")
				{
					stat.daStat[AugmentStat::MAXCP] += Value;
				}else if(Text=="stat_PATK")
				{
					stat.daStat[AugmentStat::PATK] += Value;
				}else if(Text=="stat_MATK")
				{
					stat.daStat[AugmentStat::MATK] += Value;
				}else if(Text=="stat_HPREGEN")
				{
					stat.daStat[AugmentStat::HPREGEN] += Value;
				}else if(Text=="stat_MPREGEN")
				{
					stat.daStat[AugmentStat::MPREGEN] += Value;
				}else if(Text=="stat_CPREGEN")
				{
					stat.daStat[AugmentStat::PDEF] += Value;
				}else if(Text=="stat_EVASION")
				{
					stat.daStat[AugmentStat::EVASION] += Value;
				}else if(Text=="stat_ACCURACY")
				{
					stat.daStat[AugmentStat::ACCURACY] += Value;
				}else if(Text=="stat_CRITICAL")
				{
					stat.daStat[AugmentStat::CRITICAL] += Value;
				}
				file >> Text; // end
			}
			mStats.insert(pair< int, AugmentStat> (nAugmentID, stat));
		}
		file.close();
		g_Log.Add(CLog::Blue, "[%s] %d effects in database.", __FUNCTION__, mStats.size());
	}else
		g_Log.Add(CLog::Blue, "[%s] Augmentation Stat Data cannot be found.", __FUNCTION__);
}

void CAugmentation::ReadNameData()
{
	wstring str = ReadFileW(L"..\\Script\\augmentation_name.txt");
	wstringstream sstr;
	sstr << str;
	wstring wLine;
	int Counter = 0;
	mName.clear();
	while(getline(sstr, wLine))
	{
		if(wLine.find(L"//") == 0)
			continue;

		//1 P.Def +12
		wstring ID;
		wstring Name;
		size_t pos = wLine.find_first_of(L"	\t");
		if(pos != wstring::npos)
		{
			ID = wLine.substr(0, pos);
			Name = wLine.substr(pos+1, (wLine.size()-pos-1));
			int nID = _wtoi(ID.c_str());
			if(nID > 0)
			{
				mName.insert(pair<int, wstring>(nID, Name));
				Counter++;
		//		g_Log.Add(CLog::Blue, "[%s] Id[%d] Name[%S]", __FUNCTION__, nID, Name.c_str());
			}
		}
	}
	g_Log.Add(CLog::Blue, "[%s] Loaded [%d] augmentation names.", __FUNCTION__, Counter);
}

bool CAugmentation::IsSkill(UINT nEffect)
{
	if(nEffect > STAT_SIZE && nEffect < BASESTAT_STR)
		return true;
	else
		return false;
}

bool CAugmentation::IsBaseStat(UINT nEffect)
{
	if(nEffect >= BASESTAT_STR && nEffect <= BASESTAT_MEN)
		return true;
	else
		return false;
}

bool CAugmentation::CheckItemID(int nItemID)
{
	if(nItemID > 0)
	{
		if(g_Config.AugmentationInfo.GetBlockedItemCount() > 0)
		{
			if(g_Config.AugmentationInfo.IsBlockedItem(nItemID))
				return false;
		}
		if(CShadowItem::IsShadowItem(nItemID))
			return false;
		return true;
	}else
		return false;
}
AugmentSkill CAugmentation::GetAugmentSkill(UINT nAugmentID)
{
	AugmentSkill as;
	if(nAugmentID > STAT_SIZE && nAugmentID < BASESTAT_STR)
	{
		AugmentSkillMap::iterator Iter = mSkills.find(nAugmentID);
		if(Iter != mSkills.end())
		{
			as = Iter->second;
		}else
			g_Log.Add(CLog::Error, "[%s] Cannot find skill for augment[%d]", __FUNCTION__, nAugmentID);
	}
	return as;
}
AugmentStat CAugmentation::GetAugmentStat(UINT nAugmentID)
{
	AugmentStat as;
	if(nAugmentID > 0 && nAugmentID <= STAT_SIZE)
	{
		AugmentStatMap::iterator Iter = mStats.find(nAugmentID);
		if(Iter!= mStats.end())
			return Iter->second;
	}
	return as;
}
UINT CAugmentation::GetFirstEffectID(UINT nAugmentID)
{
	UINT nEffect = 0x0000FFFF&nAugmentID;
	return nEffect;
}
UINT CAugmentation::GetSecondEffectID(UINT nAugmentID)
{
	UINT nEffect = nAugmentID >> 16;
	return nEffect;
}
UINT CAugmentation::GenerateAugmentation(int nLifeStoneGrade, int nLifeStoneLevel)
{
	//Calculating first Effect - stats
	int nBlock = 0; //Standard for lowest life stone grade
	int nSubBlock = 0;
	int nChance = g_MTRandom.RandInt(99);
	switch(nLifeStoneGrade)
	{
	case LS_LOW:
		{
			nBlock = 0;
			if(nChance < 20)
				nBlock = 1;
			if(nChance < 5)
				nBlock = 2;
			break;
		}
	case LS_MID:
		{
			nBlock = 1;
			if(nChance < 20)
				nBlock = 2;
			if(nChance < 5)
				nBlock = 3;
			break;
		}
	case LS_HIGH:
		{
			nBlock = 2;
			if(nChance < 20)
				nBlock = 3;
			break;
		}
	case LS_TOP:
		{
			nBlock = 3;
			break;
		}
	}
	
	UINT nFirstID = 0;
	if(nLifeStoneLevel < 47)
		nLifeStoneLevel = 0;
	else if(nLifeStoneLevel < 50)
		nLifeStoneLevel = 1;
	else if(nLifeStoneLevel < 53)
		nLifeStoneLevel = 2;
	else if(nLifeStoneLevel < 56)
		nLifeStoneLevel = 3;
	else if(nLifeStoneLevel < 59)
		nLifeStoneLevel = 4;
	else if(nLifeStoneLevel < 62)
		nLifeStoneLevel = 5;
	else if(nLifeStoneLevel < 65)
		nLifeStoneLevel = 6;
	else if(nLifeStoneLevel < 68)
		nLifeStoneLevel = 7;
	else if(nLifeStoneLevel < 71)
		nLifeStoneLevel = 8;
	else if(nLifeStoneLevel < 77)
		nLifeStoneLevel = 9;
	else
	{
		g_Log.Add(CLog::Error, "[%s] Invalid LifeStone Level[%d]" , __FUNCTION__, nLifeStoneLevel);
	}
	UINT nColorOffset = (nBlock * (STAT_SUBBLOCKSIZE * 10)) + (nLifeStoneLevel * STAT_SUBBLOCKSIZE);
	nFirstID = ((3 - nLifeStoneGrade) * STAT_BLOCKSIZE) + nColorOffset;
	nFirstID = g_MTRandom.RandInt(STAT_SUBBLOCKSIZE) + nFirstID;
	//Second Effect ID
	UINT nSecondID = 0;
	
	bool bSkill = false;
	bool bBaseStat = false;

	switch(nLifeStoneGrade)
	{
	case LS_LOW:
		{
			if(g_MTRandom.RandInt(99) < BaseStatChance[LS_LOW])
			{
				bBaseStat = true;
				break;
			}else if(g_MTRandom.RandInt(99) < SkillChance[LS_LOW])
			{
				bSkill = true;
			}
			break;
		}
	case LS_MID:
		{
			if(g_MTRandom.RandInt(99) < BaseStatChance[LS_MID])
			{
				bBaseStat = true;
				break;
			}else if(g_MTRandom.RandInt(99) < SkillChance[LS_MID])
			{
				bSkill = true;
			}
			break;
		}
	case LS_HIGH:
		{
			if(g_MTRandom.RandInt(99) < BaseStatChance[LS_HIGH])
			{
				bBaseStat = true;
				break;
			}else if(g_MTRandom.RandInt(99) < SkillChance[LS_HIGH])
			{
				bSkill = true;
			}
			break;
		}
	case LS_TOP:
		{
			if(g_MTRandom.RandInt(99) < BaseStatChance[LS_TOP])
			{
				bBaseStat = true;
				break;
			}else if(g_MTRandom.RandInt(99) < SkillChance[LS_TOP])
			{
				bSkill = true;
			}
			break;
		}
	}

	if(bBaseStat)
	{
		nChance = g_MTRandom.RandInt(3);
		switch(nChance)
		{
		case 1:
			{
				nSecondID = BASESTAT_STR;
				break;
			}
		case 2:
			{
				nSecondID = BASESTAT_CON;
				break;
			}
		case 3:
			{
				nSecondID = BASESTAT_INT;
				break;
			}
		default:
			{
				nSecondID = BASESTAT_MEN;
				break;
			}
		}
	}else if(bSkill)
	{
		//Effect for active/passive/chance skill
		nSecondID = (g_MTRandom.RandInt(177))+1;
		nSecondID += 178*(nLifeStoneLevel);
		nSecondID += STAT_SIZE;
	}else
	{
		nSecondID = (nLifeStoneGrade * STAT_BLOCKSIZE) + nColorOffset;
		nSecondID = g_MTRandom.RandInt(STAT_SUBBLOCKSIZE) + nSecondID;
	}

	UINT nAugmentation = (nSecondID << 16)+nFirstID;
	//g_Log.Add(CLog::Blue, "[%s] Generated Augmentation:%d", __FUNCTION__, nAugmentation);
	return nAugmentation;
}
bool CAugmentation::ShowAugmentMixWindow(CUserSocket *pSocket)
{
	if(pSocket->ValidUserSocket())
	{
		pSocket->Send("ch", 0xFE, 0x50);
	}
	return false;
}
bool CAugmentation::ShowAugmentRemoveWindow(CUserSocket *pSocket)
{
	if(pSocket->ValidUserSocket())
	{
		pSocket->Send("ch", 0xFE, 0x51);
	}
	return false;
}
bool CAugmentation::AugumentWeapon(CUserSocket *pSocket, UINT nWeaponSID, UINT nLifeStoneSID, UINT nGemstoneSID, int nGemstoneCount)
{
	User *pUser = pSocket->GetUser();
	if(pUser->ValidUser())
	{
		CItem *pWeapon = pUser->GetInventory()->GetItemBySID(nWeaponSID)->GetWeapon();
		CItem *pLifeStone = pUser->GetInventory()->GetItemBySID(nLifeStoneSID);
		CItem *pGemStone = pUser->GetInventory()->GetItemBySID(nGemstoneSID);
		if(pWeapon && pLifeStone->IsValidItem() && pGemStone->IsValidItem())
		{
			if(!CheckItemID(pWeapon->pSID->nItemID))
				return false;
			if(pWeapon->nAugmentationID == 0 && pLifeStone->IsLifeStone())
			{
				int nGemStoneID = GetGemstoneID(pWeapon->pII->nCrystalType);
				int nGemStoneCount = GetGemstoneCount(pWeapon->pII->nCrystalType);
				int nLifeStoneLevel = pLifeStone->GetLifeStoneLevel();
				int nLifeStoneGrade = pLifeStone->GetLifeStoneGrade();
				if(nLifeStoneLevel > pUser->pSD->nLevel || nGemStoneID == -1 || nGemStoneCount == -1)
				{
					pSocket->SendSystemMessage(1960);
					return false;
				}
				if(pGemStone->pSID->nItemID == nGemStoneID && pGemStone->pSID->nItemAmount >= nGemStoneCount)
				{
					UINT nAugmentID = GenerateAugmentation(nLifeStoneGrade, nLifeStoneLevel);
					
					UINT nFirstEffect =  0x0000FFFF&nAugmentID;
					UINT nSecondEffect = nAugmentID >> 16;
					pWeapon->nAugmentationID = nAugmentID;
					pUser->DeleteItemInInventory(nGemStoneID, nGemStoneCount);
					pUser->DeleteItemInInventory(pLifeStone->pSID->nItemID, 1);
					g_DB.RequestSaveItemDataEx(pUser->nDBID, pWeapon);
					pUser->inventory.SetInventoryChanged(pWeapon, 2);
					pSocket->Send("chddd", 0xFE, 0x55, nFirstEffect, nSecondEffect, 1);
					g_Logger.Add(L"User[%s] augment weapon[%d] dbid[%d] firstEffet[%d] secondEffect[%d]", pUser->pSD->wszName, pWeapon->pSID->nItemID, pWeapon->nDBID, nFirstEffect, nSecondEffect);
					return true;
				}else
					return false;
			}else
				pSocket->SendSystemMessage(2001);
		}else
			pSocket->SendSystemMessage(2001);
	}
	return false;
}

bool CAugmentation::DeleteActiveSkill(CCreature *pCreature, UINT nAugmentation)
{
	UINT nEffect[2] = { 0, 0 };
	nEffect[0] = GetFirstEffectID(nAugmentation);
	nEffect[1] = GetSecondEffectID(nAugmentation);

	if(IsSkill(nEffect[0]))
	{
		AugmentSkill as = GetAugmentSkill(nEffect[0]);
		if(as.nSkillID > 0 && as.nLevel > 0)
		{
			if(as.nSkillID >= 3123 && as.nSkillID <= 3206)
			{
				pCreature->DeleteSkill(as.nSkillID);
			}
		}
	}
	
	if(IsSkill(nEffect[1]))
	{
		AugmentSkill as = GetAugmentSkill(nEffect[1]);
		if(as.nSkillID > 0 && as.nLevel > 0)
		{
			if(as.nSkillID >= 3123 && as.nSkillID <= 3206)
			{
				pCreature->DeleteSkill(as.nSkillID);
			}
		}
	}

	return false;
}
void CAugmentation::ValidateAugmentation(CCreature *pCreature)
{
	if(pCreature->ValidCreature())
	{
		if(User *pUser = pCreature->GetUser())
		{
			pUser->ValidateBaseData();
			if(pUser->pSD->nAugmentationID)
				DeleteActiveSkill(pUser, pUser->pSD->nAugmentationID);
			pUser->pSD->nAugmentationID = 0;
			CSharedItemData *pData = pUser->GetEquipedWeapon();
			if(pData)
			{
				CItem *pWeapon = pUser->inventory.GetItemByIndex(pData->nItemIndex);
				if(pWeapon)
				{
					UINT nAugmentation = pWeapon->nAugmentationID;
					if(nAugmentation)
					{
						UINT nEffect[2] = { 0, 0 };
						nEffect[0] = GetFirstEffectID(nAugmentation);
						nEffect[1] = GetSecondEffectID(nAugmentation);
						for(int n=0;n<2;n++)
						{
							if(IsSkill(nEffect[n]))
							{
								AugmentSkill as = GetAugmentSkill(nEffect[n]);
								if(as.nSkillID > 0 && as.nLevel > 0)
								{
									CSkillInfo *pSI = g_SkillDB.GetSkillInfo(as.nSkillID, as.nLevel);
									if(pSI)
									{
										/* Dispel on passive start */
										if(g_Config.AugmentationInfo.IsDispelOnPassive())
										{
											if(as.nSkillID >= 3238 && as.nSkillID <= 3259)
											{
												if(pUser->HasAbnormal(177))
												{
													pUser->pSD->pExData->TimerHandler.AddTimer(CCreatureTimerHandler::DISPEL_AUGMENTATION, 1);
												}
											}
										}
										/* Dispel on passive end */
										if(as.nSkillID >= 3123 && as.nSkillID <= 3206)
										{
											pUser->AcquireSkill(as.nSkillID, as.nLevel);
										}else
										{
											pUser->AttachSkill(as.nSkillID, as.nLevel, pSI->operateType);
										}
									}								
								}							
							}else if(IsBaseStat(nEffect[n]))
							{
								if(g_Config.AugmentationInfo.IsDispelOnAttribute())
								{
									/* Dispel Augmentation buff */
									if(pUser->HasAbnormal(177))
									{
										pUser->pSD->pExData->TimerHandler.AddTimer(CCreatureTimerHandler::DISPEL_AUGMENTATION, 1);
									}
									/* End Dispel Augmentation buff */
								}

								pUser->lpSharedDataLock->Lock(__FILEW__, __LINE__);
								switch(nEffect[n])
								{
								case BASESTAT_STR:
									pUser->pSD->nSTR += 1;
									break;
								case BASESTAT_INT:
									pUser->pSD->nINT += 1;
									break;
								case BASESTAT_CON:
									pUser->pSD->nCON += 1;
									break;
								case BASESTAT_MEN:
									pUser->pSD->nMEN += 1;
									break;
								};
								pUser->lpSharedDataLock->Unlock();
							}
						}
						pUser->pSD->nAugmentationID = nAugmentation;
						pUser->SetUserInfoChanged();
					}
				}else
					g_Log.Add(CLog::Error, "[%s] Cannot find item by index[%d]", __FUNCTION__, pData->nItemIndex);
			}
		}
	}
}

bool CAugmentation::RemoveAugmentation(CItem *pWeapon, User *pUser)
{
	if(pWeapon->IsValid(VT_ITEMWEAPON) && pUser->ValidUser())
	{
		pWeapon->nAugmentationID = 0;
		g_DB.RequestSaveItemDataEx(pUser->nDBID, pWeapon);
		pUser->inventory.SetInventoryChanged(pWeapon, 2);
		return true;
	}
	return false;
}
bool CAugmentation::BlockItemAction(UINT nObjectID)
{
	CItem *pItem = (CItem*)L2Server::GetObjectByID(nObjectID);
	if(pItem->IsValidItem())
	{
		if(pItem->nAugmentationID)
			return true;
	}
	return false;
}

void CAugmentation::ValidateBaseStat(User *pUser)
{
	if(pUser->ValidUser())
	{
		pUser->ValidateBaseData();
		CSharedItemData *pSID = pUser->GetEquipedWeapon();
		if(pSID)
		{
			CItem *pWeapon = pUser->inventory.GetItemBySID(pSID->nObjectID);
			if(pWeapon->IsValidItem())
			{
				UINT nAugmentation = pWeapon->nAugmentationID;
				if(nAugmentation)
				{
					UINT nEffect[2] = { 0, 0 };
					nEffect[0] = GetFirstEffectID(nAugmentation);
					nEffect[1] = GetSecondEffectID(nAugmentation);
					for(int n=0;n<2;n++)
					{
						if(IsBaseStat(nEffect[n]))
						{
							if(g_Config.AugmentationInfo.IsDispelOnAttribute())
							{
								/* Dispel Augmentation buff */
								if(pUser->HasAbnormal(177))
								{
									pUser->pSD->pExData->TimerHandler.AddTimer(CCreatureTimerHandler::DISPEL_AUGMENTATION, 1);
								}
								/* End Dispel Augmentation buff */
							}

							pUser->lpSharedDataLock->Lock(__FILEW__, __LINE__);
							switch(nEffect[n])
							{
							case BASESTAT_STR:
								pUser->pSD->nSTR += 1;
								break;
							case BASESTAT_INT:
								pUser->pSD->nINT += 1;
								break;
							case BASESTAT_CON:
								pUser->pSD->nCON += 1;
								break;
							case BASESTAT_MEN:
								pUser->pSD->nMEN += 1;
								break;
							};
							pUser->lpSharedDataLock->Unlock();
						}
					}
				}
			}
		}
	}
}

void CAugmentation::SetAugmentationSkillMod(CCreature *pCreature, UINT nAugmentationID)
{
	if(nAugmentationID && pCreature->ValidUser())
	{
		int nState = 0;
		try
		{
			UINT nFirst = GetFirstEffectID(nAugmentationID);
			UINT nSecond = GetSecondEffectID(nAugmentationID);
			nState = 1;
			//g_Log.Add(CLog::Blue, "[%s] First Effect: %d Second Effect: %d", __FUNCTION__, nFirst, nSecond);
			if(nFirst > 0 && nFirst <= STAT_SIZE)
			{
				AugmentStat as;
				as = GetAugmentStat(nFirst);
				nState = 2;
				for(int n=0;n<13;n++)
				{
					nState = 3;
					if(as.daStat[n]!=0)
					{
						switch(n)
						{
						case AugmentStat::ACCURACY:
							{
								pCreature->accuracyDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::CPREGEN:
							{
								//There's no cp regen in l2 cp = hp regen;
								pCreature->hpRegenDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::CRITICAL:
							{
								pCreature->criticalRateDIFF += (as.daStat[n]*0.1);
								break;
							}
						case AugmentStat::EVASION:
							{
								pCreature->evasionDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::HPREGEN:
							{
								pCreature->hpRegenDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::MATK:
							{
								pCreature->magicalAttackDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::MAXCP:
							{
								pCreature->maxCpDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::MAXHP:
							{
								pCreature->maxHpDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::MAXMP:
							{
								pCreature->maxMpDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::MDEF:
							{
								pCreature->magicalDefenceDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::MPREGEN:
							{
								pCreature->mpRegenDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::PATK:
							{
								pCreature->physicalAttackDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::PDEF:
							{
								pCreature->physicalDefenceDIFF += as.daStat[n];
								break;
							}
						default:
							{
								break;
							}
						}
					}
				}
			}
			if(nSecond > 0 && nSecond <= STAT_SIZE)
			{
				AugmentStat as;
				as = GetAugmentStat(nSecond);
				nState = 4;
				for(int n=0;n<13;n++)
				{
					nState = 5;
					if(as.daStat[n]!=0)
					{
						switch(n)
						{
						case AugmentStat::ACCURACY:
							{
								pCreature->accuracyDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::CPREGEN:
							{
								//There's no cp regen in l2 cp = hp regen;
								pCreature->hpRegenDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::CRITICAL:
							{
								pCreature->criticalRateDIFF += (as.daStat[n]*0.1);
								break;
							}
						case AugmentStat::EVASION:
							{
								pCreature->evasionDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::HPREGEN:
							{
								pCreature->hpRegenDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::MATK:
							{
								pCreature->magicalAttackDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::MAXCP:
							{
								pCreature->maxCpDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::MAXHP:
							{
								pCreature->maxHpDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::MAXMP:
							{
								pCreature->maxMpDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::MDEF:
							{
								pCreature->magicalDefenceDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::MPREGEN:
							{
								pCreature->mpRegenDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::PATK:
							{
								pCreature->physicalAttackDIFF += as.daStat[n];
								break;
							}
						case AugmentStat::PDEF:
							{
								pCreature->physicalDefenceDIFF += as.daStat[n];
								break;
							}
						default:
							{
								break;
							}
						}
					}
				}
			}
		}
		catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected [%d].", __FUNCTION__, nState);
			return;
		}
	}
}
bool CAugmentation::RequestConfirmTargetItem(CUserSocket *pSocket, const unsigned char* packet)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	User *pUser = pSocket->GetUser();
	if(pUser->ValidUser() && CCreature::IsValidServerId(&pUser->talkTargetId))
	{
		CCreature *pTarget = CCreature::GetTalkTarget(&pUser->talkTargetId);
		if(pTarget && pTarget->IsNPC() && pUser->CheckDistance(pTarget, 200))
		{
			UINT nObjectID = 0;
			Disassemble(packet, "d", &nObjectID);
			//g_Log.Add(CLog::Blue, "[%s] ItemObjID[0x%x]", __FUNCTION__, nObjectID);
			CItem *pItem = pUser->GetInventory()->GetItemBySID(nObjectID);
			if(pItem->IsValid(VT_ITEMWEAPON) && pItem->pII->nCrystalType >= 2 && CheckItemID(pItem->pSID->nItemID))
			{
				CWeapon *pWeapon = (CWeapon*)pItem;
				if(pWeapon->nAugmentationID == 0)
				{
					pSocket->SendSystemMessage(1958); //Select the crystal for Augmentation
					pSocket->Send("chddd", 0xFE, 0x52, nObjectID, 1, 1);
				}else
					pSocket->SendSystemMessage(1970); //Cannot augument again
			}else
				pSocket->SendSystemMessage(1960); //Invalid item
		}
	}
	return false;
}
bool CAugmentation::RequestConfirmRefinerItem(CUserSocket *pSocket, const unsigned char* packet)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	User *pUser = pSocket->GetUser();
	if(pUser->ValidUser() && CCreature::IsValidServerId(&pUser->talkTargetId))
	{
		CCreature *pTarget = CCreature::GetTalkTarget(&pUser->talkTargetId);
		if(pTarget && pTarget->IsNPC() && pUser->CheckDistance(pTarget, 200))
		{
			UINT nWeaponSID = 0;
			UINT nLifeStoneSID = 0;
			Disassemble(packet, "dd", &nWeaponSID, &nLifeStoneSID);
			CItem *pWeapon = pUser->GetInventory()->GetItemBySID(nWeaponSID);
			CItem *pLifeStone = pUser->GetInventory()->GetItemBySID(nLifeStoneSID);
			if(pWeapon->IsValid(VT_ITEMWEAPON) && pLifeStone->IsValidItem())
			{
				if(pLifeStone->IsLifeStone())
				{
					int nGemStoneID = GetGemstoneID(pWeapon->pII->nCrystalType);
					int nGemCount = GetGemstoneCount(pWeapon->pII->nCrystalType);
					int nLifeStoneLevel = pLifeStone->GetLifeStoneLevel();
					if(nLifeStoneLevel > pUser->pSD->nLevel || nGemStoneID == -1 || nGemCount == -1)
					{
						pSocket->SendSystemMessage(1960);
						return false;
					}
					pSocket->Send("chddddd", 0xFE, 0x53, nLifeStoneSID, pLifeStone->pSID->nItemID, nGemStoneID, nGemCount, 1);
					pSocket->Send("cdddddd", 0x64, 1959, 2, 1, nGemCount, 3, nGemStoneID);
				}else
					pSocket->SendSystemMessage(1960);
			}else
				pSocket->SendSystemMessage(1960);
		}
	}
	return false;
}
bool CAugmentation::RequestConfirmGemStone(CUserSocket *pSocket, const unsigned char* packet)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	User *pUser = pSocket->GetUser();
	if(pUser->ValidUser() && CCreature::IsValidServerId(&pUser->talkTargetId))
	{
		CCreature *pTarget = CCreature::GetTalkTarget(&pUser->talkTargetId);
		if(pTarget && pTarget->IsNPC() && pUser->CheckDistance(pTarget, 200))
		{
			UINT nWeaponSID, nLifeStoneSID, nGemstoneSID;
			int nGemCount;
			Disassemble(packet, "dddd", &nWeaponSID, &nLifeStoneSID, &nGemstoneSID, &nGemCount);
			CItem *pWeapon = pUser->GetInventory()->GetItemBySID(nWeaponSID);
			CItem *pGemstone = pUser->GetInventory()->GetItemBySID(nGemstoneSID);
			CItem *pLifeStone = pUser->GetInventory()->GetItemBySID(nLifeStoneSID);
			if(pWeapon->IsValid(VT_ITEMWEAPON) && pGemstone->IsValidItem() && pLifeStone->IsValidItem())
			{
				int nGemstoneID = pGemstone->pSID->nItemID;
				if(nGemstoneID == GemstoneD || nGemstoneID == GemstoneC)
				{
					if(nGemstoneID == GetGemstoneID(pWeapon->pII->nCrystalType) && nGemCount == GetGemstoneCount(pWeapon->pII->nCrystalType))
					{
						pSocket->Send("chddddd", 0xFE, 0x54, nGemstoneSID, 1, nGemCount, 1, 1);
						pSocket->SendSystemMessage(1984);
					}else
					{
						pSocket->SendSystemMessage(1961);
						return false;
					}
				}else
					pSocket->SendSystemMessage(1960);
			}
		}
	}
	return false;
}
bool CAugmentation::RequestAugmentItem(CUserSocket *pSocket, const unsigned char* packet)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	User *pUser = pSocket->GetUser();
	if(pUser->ValidUser())
	{
		UINT nWeaponSID, nLifeStoneSID, nGemstoneSID;
		int nGemstoneCount;
		Disassemble(packet, "dddd", &nWeaponSID, &nLifeStoneSID, &nGemstoneSID, &nGemstoneCount);
		if(CCreature::IsValidServerId(&pUser->talkTargetId))
		{
			CCreature *pTarget = CCreature::GetTalkTarget(&pUser->talkTargetId);
			if(pTarget && pTarget->IsNPC() && pUser->CheckDistance(pTarget, 200))
			{
				if(CAugmentation::AugumentWeapon(pSocket, nWeaponSID, nLifeStoneSID, nGemstoneSID, nGemstoneCount))
				{
					pSocket->SendSystemMessage(1962);
					pUser->SetItemListChanged();
				}else
					pSocket->Send("chddd", 0xFE, 0x55, 0, 0, 0);
			}else
			{
				pSocket->Send("chddd", 0xFE, 0x55, 0, 0 ,0);
				pSocket->SendSystemMessage(L"You're too far from talking npc.");
			}
		}
	}
	return false;
}
bool CAugmentation::RequestCancelItem(CUserSocket *pSocket, const unsigned char* packet)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	User *pUser = pSocket->GetUser();
	if(pUser->ValidUser())
	{
		int nObjectID;
		Disassemble(packet, "d", &nObjectID);
		CItem *pWeapon = L2Server::GetObjectByID(nObjectID)->CastItem();
		if(pWeapon->IsValid(VT_ITEMWEAPON))
		{
			if(pWeapon->nAugmentationID)
			{
				int nPrice = 0;
				switch(pWeapon->pII->nCrystalType)
				{
				case 2:
					{
						nPrice = g_CancelPriceC;
						break;
					}
				case 3:
					{
						nPrice = g_CancelPriceB;
						break;
					}
				case 4:
					{
						nPrice = g_CancelPriceA;
						break;
					}
				case 5:
					{
						nPrice = g_CancelPriceS;
						break;
					}
				default:
					{
						pSocket->Send("chdd", 0xFE, 0x57, 1, 0);
						return false;
						break;
					}
				}
				
				int nAdena = pUser->GetInventory()->GetAdenaAmount();
				if(nAdena > nPrice)
				{
					if(CAugmentation::RemoveAugmentation(pWeapon, pUser))
					{
						pUser->DeleteItemInInventory(57, nPrice);
						pUser->SendItemList();
						pSocket->SendSystemMessage(1965);
						pSocket->Send("chdd", 0xFE, 0x57, 1, 1);
						pUser->ValidateParameters();
					}
				}
				
			}else
				pSocket->SendSystemMessage(1964);
		}else
			pSocket->SendSystemMessage(1964);
	}
	return false;
}
bool CAugmentation::RequestConfirmCancelItem(CUserSocket *pSocket, const unsigned char* packet)
{
	//static CSharedGuard sg(__FUNCTIONW__);
	//CTLS TLS(__FUNCTIONW__);
	User *pUser = pSocket->GetUser();
	if(pUser->ValidUser())
	{
		int nObjectID;
		Disassemble(packet, "d", &nObjectID);
		
		CItem *pWeapon = L2Server::GetObjectByID(nObjectID)->CastItem();
		if(pWeapon->IsValid(VT_ITEMWEAPON))
		{
			if(pWeapon->nAugmentationID)
			{
				int nPrice = 0;
				switch(pWeapon->pII->nCrystalType)
				{
				case 2:
					{
						nPrice = g_CancelPriceC;
						break;
					}
				case 3:
					{
						nPrice = g_CancelPriceB;
						break;
					}
				case 4:
					{
						nPrice = g_CancelPriceA;
						break;
					}
				case 5:
					{
						nPrice = g_CancelPriceS;
						break;
					}
				default:
					{
						return false;
						break;
					}
				}
				pSocket->Send("chddddddd", 0xFE, 0x56, 0x40A97712, nObjectID, 0x27, 0x2006, nPrice, 0, 0x01);
			}else
				pSocket->SendSystemMessage(1964);
		}else
			pSocket->SendSystemMessage(1964);
	}
	return false;
}

bool CAugmentation::MultiSellDeleteItem(CInventory *pInventory, CItem *pItem, int nType)
{
	if(pItem->IsValidItem())
	{
		if(pItem->nAugmentationID)
		{
			UINT nAugmentation = pItem->nAugmentationID;
			UINT nProtectionTimeout = pItem->nProtectionTimeout;
			User *pOwner = pInventory->GetOwner()->GetUser();
			if(pOwner->ValidUser())
			{
				if(pOwner->pSD->nAugmentationTmp == 0)
				{
					pOwner->pSD->nAugmentationTmp = nAugmentation;
				}
				if(pOwner->pSD->nProtectionTimeoutTmp == 0)
				{
					pOwner->pSD->nProtectionTimeoutTmp = nProtectionTimeout;
				}
			}
		}
	}
	return pInventory->SetInventoryChanged(pItem, nType);
}

void CAugmentation::MultiSellAddItem(User *pUser, CItem *pItem)
{
	if(pItem->IsValidItem())
	{
		if(pUser->ValidUser())
		{
			bool save = false;
			if(pUser->pSD->nAugmentationTmp)
			{
				pItem->nAugmentationID = pUser->pSD->nAugmentationTmp;
				pUser->pSD->nAugmentationTmp = 0;
				save = true;
				
			}
			if(pUser->pSD->nProtectionTimeoutTmp)
			{
				pItem->nProtectionTimeout = pUser->pSD->nProtectionTimeoutTmp;
				pUser->pSD->nProtectionTimeoutTmp = 0;
				save = true;
			}
			if(pItem->nManaLeft != 0)
			{
				save = true;
			}
			if(save)
			{
				g_DB.RequestSaveItemDataEx(pUser->nDBID, pItem);
			}
		}
	}
}
int CAugmentation::GetGemstoneID(int nCrystalType)
{
	switch(nCrystalType)
	{
		case CrystalC:
			{
				return GemstoneD;
				break;
			}
		case CrystalB:
			{
				return GemstoneD;
				break;
			}
		case CrystalA:
			{
				return GemstoneC;								
				break;
			}
		case CrystalS:
			{
				return GemstoneC;
				break;
			}
		default:
			{
				return -1;
				break;
			}
	}
}
int CAugmentation::GetGemstoneCount(int nCrystalType)
{
	switch(nCrystalType)
	{
		case CrystalC:
			{
				return 20;
				break;
			}
		case CrystalB:
			{
				return 30;
				break;
			}
		case CrystalA:
			{
				return 20;								
				break;
			}
		case CrystalS:
			{
				return 25;
				break;
			}
		default:
			{
				return -1;
				break;
			}
	}
}

wstring CAugmentation::GetName(UINT nEffect)
{
	map<UINT, wstring>::iterator Iter = mName.find(nEffect);
	if(Iter!=mName.end())
	{
		return Iter->second;
	}else
		return L"";
}

const unsigned char* CAugmentation::DisassembleWareHouseItem(const unsigned char* packet, const char* format, LPINT lpItemId, LPINT lpItemType, LPINT lpAmount, LPINT lpEnchant, LPINT lpEroded, LPINT lpBless, LPINT lpIdent, LPINT lpWished)
{
	//old format: dddddddd
	//new format: ddddddddd
	UINT currentThreadIdx = GetThreadIndex();
	if(currentThreadIdx < 32)
	{
		return Disassemble(packet, "dddddddddd", lpItemId, lpItemType, lpAmount, lpEnchant, lpEroded, lpBless, lpIdent, lpWished, &g_WareHouseAugmentationId[currentThreadIdx], &g_WareHouseProtectionTimeout[currentThreadIdx]);
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Invalid thread index[%d]!", __FUNCTION__, currentThreadIdx);
		UINT augmentation = 0;
		UINT protectionTimeout = 0;
		return Disassemble(packet, "dddddddddd", lpItemId, lpItemType, lpAmount, lpEnchant, lpEroded, lpBless, lpIdent, lpWished, &augmentation, &protectionTimeout);
	}
}

bool CAugmentation::WareHouseWithdrawItem(CInventory *pInventory, CItem *pItem, int updateType)
{
	typedef bool (*f)(CInventory*, CItem*, int);

	if(CWeapon *pWeapon = pItem->GetWeapon())
	{
		UINT currentThreadIdx = GetThreadIndex();
		if(currentThreadIdx < 32)
		{
			pWeapon->nAugmentationID = g_WareHouseAugmentationId[currentThreadIdx];

		}else
		{
			g_Log.Add(CLog::Error, "[%s] Invalid thread index[%d]!", __FUNCTION__, currentThreadIdx);
		}
	}
	if(pItem->IsValidItem())
	{
		UINT currentThreadIdx = GetThreadIndex();
		if(currentThreadIdx < 32)
		{
			pItem->nProtectionTimeout = g_WareHouseProtectionTimeout[currentThreadIdx];

		}else
		{
			g_Log.Add(CLog::Error, "[%s] Invalid thread index[%d]!", __FUNCTION__, currentThreadIdx);
		}
	}

	return f(0x61C0B0L)(pInventory, pItem, updateType);
}