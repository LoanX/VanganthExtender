#include "StdAfx.h"
#include "DB.h"
#include "Random.h"
#include "ItemEnchant.h"
#include "VIPSystem.h"

double armorEnchantRate[30];
double magicWeaponEnchantRate[30];
double normalWeaponEnchantRate[30];

UINT g_BlessedEnchantFailLevel = 0;

UINT maxWeaponEnchant = 30;
UINT maxArmorEnchant = 20;
UINT maxSafeWeaponEnchant = 3;
UINT maxSafeArmorEnchant = 3;

INT32 customEnchantArmorId = 9447;
UINT maxCustomArmorEnchant = 7;
UINT minCustomArmorEnchant = 3;
double customArmorEnchantRate = 100;

INT32 customEnchantWeaponId = 9446;
UINT maxCustomWeaponEnchant = 7;
UINT minCustomWeaponEnchant = 3;
double customWeaponEnchantRate = 100;

BOOL g_StackableEnchantScrolls = FALSE;

struct CustomEnchantData
{
	enum Type
	{
		Armor = 0,
		Weapon = 1
	};
	Type type;
	int scrollId;
	double enchantRate;
	UINT minEnchant;
	UINT maxEnchant;
	BOOL isBlessed;
	UINT safeEnchantLevel;
};
map<INT32, CustomEnchantData> g_CustomEnchant;

BOOL itemEnchantSystem = FALSE;

void CItemEnchant::Initialize()
{
	LoadINI();
	//Hooks
	if(itemEnchantSystem)
	{
		g_HookManager.WriteCall( 0x7CA86C, EnchantItem, 0);
		g_HookManager.WriteCall( 0x7CA9BC, EnchantItem, 0);
		g_Log.Add(CLog::Blue, "[%s] Done.", __FUNCTION__);
	}
}

void CItemEnchant::LoadINI()
{
	const TCHAR* sectionName = _T("ItemEnchantSystem");
	itemEnchantSystem = GetPrivateProfileInt(sectionName, _T("Enabled"), 0, g_ConfigFile);

	g_StackableEnchantScrolls = GetPrivateProfileInt(sectionName, _T("StackableScrolls"), 0, g_ConfigFile);
	
	maxWeaponEnchant = GetPrivateProfileInt(sectionName, _T("MaxWeaponEnchant"), 30, g_ConfigFile);
	maxArmorEnchant = GetPrivateProfileInt(sectionName, _T("MaxArmorEnchant"), 20, g_ConfigFile);
	maxSafeWeaponEnchant = GetPrivateProfileInt(sectionName, _T("MaxSafeWeaponEnchant"), 3, g_ConfigFile);
	maxSafeArmorEnchant = GetPrivateProfileInt(sectionName, _T("MaxSafeArmorEnchant"), 3, g_ConfigFile);

	customEnchantArmorId = GetPrivateProfileInt(sectionName, _T("CustomEnchantArmorId"), 9447, g_ConfigFile);
	maxCustomArmorEnchant = GetPrivateProfileInt(sectionName, _T("MaxCustomArmorEnchant"), 7, g_ConfigFile);
	minCustomArmorEnchant = GetPrivateProfileInt(sectionName, _T("MinCustomArmorEnchant"), 3, g_ConfigFile);
	customArmorEnchantRate = GetPrivateProfileDouble(sectionName, _T("CustomArmorEnchantRate"), 100.0, g_ConfigFile);

	customEnchantWeaponId = GetPrivateProfileInt(sectionName, _T("CustomEnchantWeaponId"), 9446, g_ConfigFile);
	maxCustomWeaponEnchant = GetPrivateProfileInt(sectionName, _T("MaxCustomWeaponEnchant"), 7, g_ConfigFile);
	minCustomWeaponEnchant = GetPrivateProfileInt(sectionName, _T("MinCustomWeaponEnchant"), 3, g_ConfigFile);
	customWeaponEnchantRate = GetPrivateProfileDouble(sectionName, _T("CustomWeaponEnchantRate"), 100.0, g_ConfigFile);
	g_BlessedEnchantFailLevel = GetPrivateProfileInt(sectionName, _T("BlessedEnchantFailLevel"), 0, g_ConfigFile);

	/*
		CustomArmorEnchantList=9447 9446
		EnchantRate_9448=100
		MaxCustomLevel_9446=7
		MinCustomLevel_9446=3
		;0 - armor, 1 - weapon
		EnchantType_9446=0
	*/
	
	WCHAR temp[8190];
	if(GetPrivateProfileString(sectionName, _T("CustomArmorEnchantList"), 0, temp, 8190, g_ConfigFile))
	{
		wstringstream idStream;
		idStream << temp;
		INT32 scrollId = 0;
		while(idStream >> scrollId)
		{
			if(scrollId > 0)
			{
				CustomEnchantData ced;
				{
					wstringstream keyStream;
					keyStream << L"EnchantType_" << scrollId;
					ced.type = (CustomEnchantData::Type)GetPrivateProfileInt(sectionName, keyStream.str().c_str(), 0, g_ConfigFile);
				}
				{
					wstringstream keyStream;
					keyStream << L"EnchantRate_" << scrollId;
					ced.enchantRate = GetPrivateProfileInt(sectionName, keyStream.str().c_str(), 0, g_ConfigFile);
				}
				{
					wstringstream keyStream;
					keyStream << L"MaxCustomLevel_" << scrollId;
					ced.maxEnchant = GetPrivateProfileInt(sectionName, keyStream.str().c_str(), 0, g_ConfigFile);
				}
				{
					wstringstream keyStream;
					keyStream << L"MinCustomLevel_" << scrollId;
					ced.minEnchant = GetPrivateProfileInt(sectionName, keyStream.str().c_str(), 0, g_ConfigFile);
				}
				{
					wstringstream keyStream;
					keyStream << L"BlessedEnchant_" << scrollId;
					ced.isBlessed = GetPrivateProfileInt(sectionName, keyStream.str().c_str(), 0, g_ConfigFile);
				}
				{
					wstringstream keyStream;
					keyStream << L"SafeEnchantLevel_" << scrollId;
					ced.safeEnchantLevel = GetPrivateProfileInt(sectionName, keyStream.str().c_str(), 0, g_ConfigFile);
				}
				ced.scrollId = scrollId;
				g_Log.Add(CLog::Blue, "[%s] Scroll[%d] type[%d] rate[%d] minLevel[%d] maxLevel[%d] blessed[%d] safeEnchantLevel[%d]", 
					__FUNCTION__, scrollId, ced.type, ced.enchantRate, ced.minEnchant, ced.maxEnchant, ced.isBlessed, ced.safeEnchantLevel);
				g_CustomEnchant[scrollId] = ced;
			}
		}
	}
	if(g_CustomEnchant.size() > 0)
	{
		g_Log.Add(CLog::Blue, "[%s] Loaded [%d] custom enchant(s).", __FUNCTION__, g_CustomEnchant.size());
	}

	for(int n=0;n<30;n++)
	{
		if(n > 2)
		{
			{
				tstringstream skey;
				skey << "MagicWeaponEnchantRate" << n ;
				magicWeaponEnchantRate[n] = (double)GetPrivateProfileInt(sectionName, skey.str().c_str(), 40, g_ConfigFile);
			}
			{
				tstringstream skey;
				skey << "NormalWeaponEnchantRate" << n ;
				normalWeaponEnchantRate[n] = (double)GetPrivateProfileInt(sectionName, skey.str().c_str(), 70, g_ConfigFile);
			}
			{
				tstringstream skey;
				skey << "ArmorEnchantRate" << n ;
				armorEnchantRate[n] = (double)GetPrivateProfileInt(sectionName, skey.str().c_str(), 66, g_ConfigFile);
			}
		}else
		{
			armorEnchantRate[n] = 100;
			magicWeaponEnchantRate[n] = 100;
			normalWeaponEnchantRate[n] = 100;
		}
		//g_Log.Add(CLog::Blue, "[%s] level[%d] Magic[%f] Normal[%f] Armor[%f]", __FUNCTION__, n, magicWeaponEnchantRate[n], normalWeaponEnchantRate[n], armorEnchantRate[n]);
	}

}

void CItemEnchant::CustomEnchantItem(User *pUser, CItem *pItem, int nEnchant)
{
	if(pUser->ValidUser())
	{
		if(pItem->IsValidItem())
		{
			if(nEnchant > 0)
			{
				g_DB.RequestCustomEnchantItem(pUser->nDBID, pItem->nDBID, nEnchant);
			}else
				g_Log.Add(CLog::Error, "[%s] Invalid enchant level[%d]", __FUNCTION__, nEnchant);
		}else
			g_Log.Add(CLog::Error, "[%s] Invalid Item", __FUNCTION__);
	}else
		g_Log.Add(CLog::Error, "[%s] Invalid User.", __FUNCTION__);
}

bool CItemEnchant::EnchantItem(CItem *pScroll, CItem *pItem, User *pUser)
{
	if(pScroll && pItem && pUser->ValidUser())
	{
		if(pUser->IsNowTrade())
			pUser->TradeCancel();

		if(pUser->pSD->nStoreMode != 0)
		{
			pUser->SendSystemMessage(L"You cannot enchant any item at current state!");
			CancelEnchant(pUser);
			return true;
		}

		if(pItem->nManaLeft > 0)
		{
			pUser->SendSystemMessage(L"You cannot enchant this item.");
			CancelEnchant(pUser);
			return true;
		}

		map<INT32, CustomEnchantData>::iterator scrollIter = g_CustomEnchant.find(pScroll->pSID->nItemID);
		if(scrollIter != g_CustomEnchant.end())
		{
			CustomEnchantData& cad = scrollIter->second;
			if(cad.type == CustomEnchantData::Weapon)
			{
				//Weapone enchant
				//100% enchant weapon
				UINT nEnchantLevel = pItem->pSID->nEnchantLevel;
				if(pItem->pSID->nItemType == 0)
				{
					//Weapon
					if(nEnchantLevel < cad.maxEnchant)
					{
						if(nEnchantLevel >= cad.minEnchant)
						{
							if(cad.safeEnchantLevel > nEnchantLevel)
							{
								if(nEnchantLevel)
								{
									pUser->pSocket->Send("cdddddd", 0x64, 63, 2, 1, nEnchantLevel, 3, pItem->pSID->nItemID); //Your +$S1 $S2 has been successfully enchanted.
								}else
								{
									pUser->pSocket->Send("cdddd", 0x64, 62, 1, 3, pItem->pSID->nItemID); //Your $s1 has been successfully enchanted.
								}
								if(g_StackableEnchantScrolls)
								{
									g_DB.RequestCustomEnchantItem(pUser->nDBID, pItem->nDBID, (nEnchantLevel+1), pScroll->pSID->nItemID);
								}else
								{
									g_DB.RequestEnchantItem(pScroll, pItem, (nEnchantLevel+1), pUser);
								}
								return true;
							}else if(cad.enchantRate > g_Random.RandDouble(100.0))
							{
								CSocket *pSocket = pUser->GetSocket();
								if(pSocket)
								{
									if(nEnchantLevel)
									{
										pUser->pSocket->Send("cdddddd", 0x64, 63, 2, 1, nEnchantLevel, 3, pItem->pSID->nItemID); //Your +$S1 $S2 has been successfully enchanted.
									}else
									{
										pUser->pSocket->Send("cdddd", 0x64, 62, 1, 3, pItem->pSID->nItemID); //Your $s1 has been successfully enchanted.
									}
								}
								if(g_StackableEnchantScrolls)
								{
									g_DB.RequestCustomEnchantItem(pUser->nDBID, pItem->nDBID, (nEnchantLevel+1), pScroll->pSID->nItemID);
								}else
								{
									g_DB.RequestEnchantItem(pScroll, pItem, (nEnchantLevel+1), pUser);
								}
								return true;
							}else
							{
								//fail
								if(cad.isBlessed)
								{
									if(nEnchantLevel)
									{
										pUser->pSocket->Send("cdddddd", 0x64, 65, 2, 1 , pItem->pSID->nEnchantLevel, 3, pItem->pSID->nItemID);
									}else
									{
										pUser->pSocket->Send("cdddd", 0x64, 64, 1, 3, pItem->pSID->nItemID);
									}
									if(g_StackableEnchantScrolls)
									{
										g_DB.RequestCustomEnchantItem(pUser->nDBID, pItem->nDBID, g_BlessedEnchantFailLevel, pScroll->pSID->nItemID);
									}else
									{
										g_DB.RequestEnchantItem(pScroll, pItem, g_BlessedEnchantFailLevel, pUser);
									}
								}else
								{
									if(nEnchantLevel)
									{
										pUser->pSocket->Send("cdddddd", 0x64, 65, 2, 1 , pItem->pSID->nEnchantLevel, 3, pItem->pSID->nItemID);
									}else
									{
										pUser->pSocket->Send("cdddd", 0x64, 64, 1, 3, pItem->pSID->nItemID);
									}
									if(g_StackableEnchantScrolls)
									{
										INT32 crystalId = 0;
										INT32 crystalCount = pItem->pII->nCrystalCount;
										if(nEnchantLevel > 0)
										{
											crystalCount += (pItem->pII->nCrystalCount / 25 * nEnchantLevel);
										}
										if(pItem->pII->nCrystalType > 0)
										{
											crystalId = 1457 + pItem->pII->nCrystalType;
											pUser->AddItemToInventory(crystalId, crystalCount);
										}
										g_DB.RequestDestroyItem(pItem, 1, pUser);
										g_DB.RequestDestroyItem(pScroll, 1, pUser);
										pUser->pSocket->Send("cd", 0x81, 0);
										pUser->isEnchantingItem = 0;
										typedef void (*f)(UINT64, UINT);
										UINT64 addr = (UINT64)pUser + 0x2930;
										f(0x7F0210)(addr, 0);
										pUser->SendItemList(true);
									}else
									{
										g_DB.RequestEnchantFail(pScroll, pItem, pUser);
									}
								}
								return true;
							}
						}else
							pUser->GetSocket()->SendSystemMessage(424);
					}else
						pUser->GetSocket()->SendSystemMessage(424);
				}else
					pUser->GetSocket()->SendSystemMessage(424);

				CancelEnchant(pUser);
				return true;
			}else
			{
				//armor enchant
				//100% enchant armor
				UINT nEnchantLevel = pItem->pSID->nEnchantLevel;
				if(pItem->pSID->nItemType == 1 || pItem->pSID->nItemType == 2)
				{
					if(nEnchantLevel < cad.maxEnchant)
					{
						if(nEnchantLevel >= cad.minEnchant)
						{
							if(cad.safeEnchantLevel > nEnchantLevel)
							{
								if(nEnchantLevel)
								{
									pUser->pSocket->Send("cdddddd", 0x64, 63, 2, 1, nEnchantLevel, 3, pItem->pSID->nItemID); //Your +$S1 $S2 has been successfully enchanted.
								}else
								{
									pUser->pSocket->Send("cdddd", 0x64, 62, 1, 3, pItem->pSID->nItemID); //Your $s1 has been successfully enchanted.
								}
								if(g_StackableEnchantScrolls)
								{
									g_DB.RequestCustomEnchantItem(pUser->nDBID, pItem->nDBID, (nEnchantLevel+1), pScroll->pSID->nItemID);
								}else
								{
									g_DB.RequestEnchantItem(pScroll, pItem, (nEnchantLevel+1), pUser);
								}
								return true;
							}else if(cad.enchantRate > g_Random.RandDouble(100.0))
							{
								CSocket *pSocket = pUser->GetSocket();
								if(pSocket)
								{
									if(nEnchantLevel)
									{
										pUser->pSocket->Send("cdddddd", 0x64, 63, 2, 1, nEnchantLevel, 3, pItem->pSID->nItemID); //Your +$S1 $S2 has been successfully enchanted.
									}else
										pUser->pSocket->Send("cdddd", 0x64, 62, 1, 3, pItem->pSID->nItemID); //Your $s1 has been successfully enchanted.
								}
								if(g_StackableEnchantScrolls)
								{
									g_DB.RequestCustomEnchantItem(pUser->nDBID, pItem->nDBID, (nEnchantLevel+1), pScroll->pSID->nItemID);
								}else
								{
									g_DB.RequestEnchantItem(pScroll, pItem, (nEnchantLevel+1), pUser);
								}
								return true;
							}else
							{
								if(cad.isBlessed)
								{
									//fail
									if(nEnchantLevel)
									{
										pUser->pSocket->Send("cdddddd", 0x64, 65, 2, 1 , pItem->pSID->nEnchantLevel, 3, pItem->pSID->nItemID);
									}else
									{
										pUser->pSocket->Send("cdddd", 0x64, 64, 1, 3, pItem->pSID->nItemID);
									}
									if(g_StackableEnchantScrolls)
									{
										g_DB.RequestCustomEnchantItem(pUser->nDBID, pItem->nDBID, g_BlessedEnchantFailLevel, pScroll->pSID->nItemID);
									}else
									{
										g_DB.RequestEnchantItem(pScroll, pItem, g_BlessedEnchantFailLevel, pUser);
									}
								}else
								{
									if(nEnchantLevel)
									{
										pUser->pSocket->Send("cdddddd", 0x64, 65, 2, 1 , pItem->pSID->nEnchantLevel, 3, pItem->pSID->nItemID);
									}else
									{
										pUser->pSocket->Send("cdddd", 0x64, 64, 1, 3, pItem->pSID->nItemID);
									}
									if(g_StackableEnchantScrolls)
									{
										INT32 crystalId = 0;
										INT32 crystalCount = pItem->pII->nCrystalCount;
										if(nEnchantLevel > 0)
										{
											crystalCount += (pItem->pII->nCrystalCount / 25 * nEnchantLevel);
										}
										if(pItem->pII->nCrystalType > 0)
										{
											crystalId = 1457 + pItem->pII->nCrystalType;
											pUser->AddItemToInventory(crystalId, crystalCount);
										}
										g_DB.RequestDestroyItem(pItem, 1, pUser);
										g_DB.RequestDestroyItem(pScroll, 1, pUser);
										pUser->pSocket->Send("cd", 0x81, 0);
										pUser->isEnchantingItem = 0;
										typedef void (*f)(UINT64, UINT);
										UINT64 addr = (UINT64)pUser + 0x2930;
										f(0x7F0210)(addr, 0);
										pUser->SendItemList(true);
									}else
									{
										g_DB.RequestEnchantFail(pScroll, pItem, pUser);
									}
								}
								return true;
							}
						}else
							pUser->GetSocket()->SendSystemMessage(424);
					}else
						pUser->GetSocket()->SendSystemMessage(424);
				}else
					pUser->GetSocket()->SendSystemMessage(424);

				CancelEnchant(pUser);
				return true;
			}
		}else
		{
			//Normal Scroll
			UINT nEnchantLevel = pItem->pSID->nEnchantLevel;
			CUserSocket* pSocket = pUser->GetSocket();
			if(pItem->pSID->nItemType == 0)	//Weapon
			{
				if(nEnchantLevel < maxWeaponEnchant)
				{
					if(nEnchantLevel < maxSafeWeaponEnchant)
					{
						if(nEnchantLevel)
						{
							pSocket->Send("cdddddd", 0x64, 63, 2, 1, nEnchantLevel, 3, pItem->pSID->nItemID); //Your +$S1 $S2 has been successfully enchanted.
						}else
						{
							pSocket->Send("cdddd", 0x64, 62, 1, 3, pItem->pSID->nItemID); //Your $s1 has been successfully enchanted.
						}
						if(g_StackableEnchantScrolls)
						{
							g_DB.RequestCustomEnchantItem(pUser->nDBID, pItem->nDBID, (nEnchantLevel+1), pScroll->pSID->nItemID);
						}else
						{
							g_DB.RequestEnchantItem(pScroll, pItem, (nEnchantLevel+1), pUser);
						}
					}else if(pItem->pII->magicWeapon)
					{
						//magic weapon
						double chance = 100;
						
						if(nEnchantLevel < 30)
						{
							chance = magicWeaponEnchantRate[nEnchantLevel];
						}else
						{
							chance = magicWeaponEnchantRate[29];
						}

						if(pUser->pSD->vipLevel > 0)
						{
							VIPInfo vipInfo = g_VIPSystem.GetInfo(pUser->pSD->vipLevel);
							if(vipInfo.magicWeaponEnchantBonus)
							{
								double bonus = (double)vipInfo.magicWeaponEnchantBonus;
								bonus += 100;
								bonus /= 100;
								chance *= bonus;
							}
						}

						double dice = g_Random.RandDouble(100);

						if( dice < chance)
						{
							g_Logger.Add(L"User[%d] item[%d] magic weapon enchant succeeded, prev enchanted[%d], dice[%f], prob[%f]", pUser->nDBID, pItem->nDBID, nEnchantLevel, dice, chance);

							pSocket->Send("cdddddd", 0x64, 63, 2, 1, nEnchantLevel, 3, pItem->pSID->nItemID); //Your +$S1 $S2 has been successfully enchanted.
							if(g_StackableEnchantScrolls)
							{
								g_DB.RequestCustomEnchantItem(pUser->nDBID, pItem->nDBID, (nEnchantLevel+1), pScroll->pSID->nItemID);
							}else
							{
								g_DB.RequestEnchantItem(pScroll, pItem, (nEnchantLevel+1), pUser);
							}
						}else
						{
							g_Logger.Add(L"User[%d] item[%d] magic weapon enchant failed, prev enchanted[%d], dice[%f], prob[%f]", pUser->nDBID, pItem->nDBID, nEnchantLevel, dice, chance);
							if(pScroll->pII->etcType == EtcItemBlessedScrollEnchantWeapon || pScroll->pII->etcType == EtcItemBlessedScrollEnchantArmor)
							{
								//1517	1	Failed in Blessed Enchant. The enchant value of the item became 0.\0	0	79	9B	B0	FF	a,ItemSound3.sys_enchant_failed\0	a,	0	0	0	0	0	a,	a,none\0
								pSocket->SendSystemMessage(1517);
								if(g_StackableEnchantScrolls)
								{
									g_DB.RequestCustomEnchantItem(pUser->nDBID, pItem->nDBID, g_BlessedEnchantFailLevel, pScroll->pSID->nItemID);
								}else
								{
									g_DB.RequestEnchantItem(pScroll, pItem, g_BlessedEnchantFailLevel, pUser);
								}
							}else
							{
								if(nEnchantLevel)
								{
									pSocket->Send("cdddddd", 0x64, 65, 2, 1 , pItem->pSID->nEnchantLevel, 3, pItem->pSID->nItemID);
								}else
								{
									pSocket->Send("cdddd", 0x64, 64, 1, 3, pItem->pSID->nItemID);
								}
								if(g_StackableEnchantScrolls)
								{
									INT32 crystalId = 0;
									INT32 crystalCount = pItem->pII->nCrystalCount;
									if(nEnchantLevel > 0)
									{
										crystalCount += (pItem->pII->nCrystalCount / 25 * nEnchantLevel);
									}
									if(pItem->pII->nCrystalType > 0)
									{
										crystalId = 1457 + pItem->pII->nCrystalType;
										pUser->AddItemToInventory(crystalId, crystalCount);
									}
									g_DB.RequestDestroyItem(pItem, 1, pUser);
									g_DB.RequestDestroyItem(pScroll, 1, pUser);
									pUser->pSocket->Send("cd", 0x81, 0);
									pUser->isEnchantingItem = 0;
									typedef void (*f)(UINT64, UINT);
									UINT64 addr = (UINT64)pUser + 0x2930;
									f(0x7F0210)(addr, 0);
									pUser->SendItemList(true);
								}else
								{
									g_DB.RequestEnchantFail(pScroll, pItem, pUser);
								}
							}
						}
					}else
					{
						//normal weapon
						double chance = 100;
						if(nEnchantLevel < 30)
						{
							chance = normalWeaponEnchantRate[nEnchantLevel];
						}else
						{
							chance = normalWeaponEnchantRate[29];
						}

						if(pUser->pSD->vipLevel > 0)
						{
							VIPInfo vipInfo = g_VIPSystem.GetInfo(pUser->pSD->vipLevel);
							if(vipInfo.magicWeaponEnchantBonus)
							{
								double bonus = (double)vipInfo.normalWeaponEnchantBonus;
								bonus += 100;
								bonus /= 100;
								chance *= bonus;
							}
						}

						double dice = g_Random.RandDouble(100);

						if( dice < chance)
						{
							g_Logger.Add(L"User[%d] item[%d] normal weapon enchant succeeded, prev enchanted[%d], dice[%f], prob[%f]", pUser->nDBID, pItem->nDBID, nEnchantLevel, dice, chance);

							pSocket->Send("cdddddd", 0x64, 63, 2, 1, nEnchantLevel, 3, pItem->pSID->nItemID); //Your +$S1 $S2 has been successfully enchanted.
							if(g_StackableEnchantScrolls)
							{
								g_DB.RequestCustomEnchantItem(pUser->nDBID, pItem->nDBID, (nEnchantLevel+1), pScroll->pSID->nItemID);
							}else
							{
								g_DB.RequestEnchantItem(pScroll, pItem, (nEnchantLevel+1), pUser);
							}
						}else
						{
							g_Logger.Add(L"User[%d] item[%d] normal weapon enchant failed, prev enchanted[%d], dice[%f], prob[%f]", pUser->nDBID, pItem->nDBID, nEnchantLevel, dice, chance);

							if(pScroll->pII->etcType == EtcItemBlessedScrollEnchantWeapon || pScroll->pII->etcType == EtcItemBlessedScrollEnchantArmor)
							{
								//1517	1	Failed in Blessed Enchant. The enchant value of the item became 0.\0	0	79	9B	B0	FF	a,ItemSound3.sys_enchant_failed\0	a,	0	0	0	0	0	a,	a,none\0
								pSocket->SendSystemMessage(1517);
								if(g_StackableEnchantScrolls)
								{
									g_DB.RequestCustomEnchantItem(pUser->nDBID, pItem->nDBID, g_BlessedEnchantFailLevel, pScroll->pSID->nItemID);
								}else
								{
									g_DB.RequestEnchantItem(pScroll, pItem, g_BlessedEnchantFailLevel, pUser);
								}
							}else
							{
								if(nEnchantLevel)
								{
									pSocket->Send("cdddddd", 0x64, 65, 2, 1 , pItem->pSID->nEnchantLevel, 3, pItem->pSID->nItemID);
								}else
								{
									pSocket->Send("cdddd", 0x64, 64, 1, 3, pItem->pSID->nItemID);
								}
								if(g_StackableEnchantScrolls)
								{
									INT32 crystalId = 0;
									INT32 crystalCount = pItem->pII->nCrystalCount;
									if(nEnchantLevel > 0)
									{
										crystalCount += (pItem->pII->nCrystalCount / 25 * nEnchantLevel);
									}
									if(pItem->pII->nCrystalType > 0)
									{
										crystalId = 1457 + pItem->pII->nCrystalType;
										pUser->AddItemToInventory(crystalId, crystalCount);
									}
									g_DB.RequestDestroyItem(pItem, 1, pUser);
									g_DB.RequestDestroyItem(pScroll, 1, pUser);
									pUser->pSocket->Send("cd", 0x81, 0);
									pUser->isEnchantingItem = 0;
									typedef void (*f)(UINT64, UINT);
									UINT64 addr = (UINT64)pUser + 0x2930;
									f(0x7F0210)(addr, 0);
									pUser->SendItemList(true);
								}else
								{
									g_DB.RequestEnchantFail(pScroll, pItem, pUser);
								}
							}
						}
					}
				}else
				{
					pUser->SendSystemMessage(L"This weapon already got max enchant level.");
					CancelEnchant(pUser);
				}
			}else
			{
				//Armor
				if(nEnchantLevel < maxArmorEnchant)
				{
					if(nEnchantLevel < maxSafeArmorEnchant)
					{
						if(nEnchantLevel)
						{
							pSocket->Send("cdddddd", 0x64, 63, 2, 1, nEnchantLevel, 3, pItem->pSID->nItemID); //Your +$S1 $S2 has been successfully enchanted.
						}else
						{
							pSocket->Send("cdddd", 0x64, 62, 1, 3, pItem->pSID->nItemID); //Your $s1 has been successfully enchanted.
						}
						if(g_StackableEnchantScrolls)
						{
							g_DB.RequestCustomEnchantItem(pUser->nDBID, pItem->nDBID, (nEnchantLevel+1), pScroll->pSID->nItemID);
						}else
						{
							g_DB.RequestEnchantItem(pScroll, pItem, (nEnchantLevel+1), pUser);
						}
					}else
					{
						double chance = 100;

						bool onePiece = 1 & (pItem->pSID->nSlotType >> 15);

						if(onePiece)
						{
							if(nEnchantLevel < 30)
							{
								chance = armorEnchantRate[nEnchantLevel-1];
							}else
							{
								chance = armorEnchantRate[29];
							}
						}else
						{
							if(nEnchantLevel < 30)
							{
								chance = armorEnchantRate[nEnchantLevel];
							}else
							{
								chance = armorEnchantRate[29];
							}
						}

						if(pUser->pSD->vipLevel > 0)
						{
							VIPInfo vipInfo = g_VIPSystem.GetInfo(pUser->pSD->vipLevel);
							if(vipInfo.magicWeaponEnchantBonus)
							{
								double bonus = (double)vipInfo.armorEnchantBonus;
								bonus += 100;
								bonus /= 100;
								chance *= bonus;
							}
						}
						
						double dice = g_Random.RandDouble(100);

						if( dice < chance)
						{
							g_Logger.Add(L"User[%d] item[%d] armor enchant succeeded, prev enchanted[%d], dice[%f], prob[%f]", pUser->nDBID, pItem->nDBID, nEnchantLevel, dice, chance);
							pSocket->Send("cdddddd", 0x64, 63, 2, 1, nEnchantLevel, 3, pItem->pSID->nItemID); //Your +$S1 $S2 has been successfully enchanted.
							if(g_StackableEnchantScrolls)
							{
								g_DB.RequestCustomEnchantItem(pUser->nDBID, pItem->nDBID, (nEnchantLevel+1), pScroll->pSID->nItemID);
							}else
							{
								g_DB.RequestEnchantItem(pScroll, pItem, (nEnchantLevel+1), pUser);
							}
						}else
						{
							g_Logger.Add(L"User[%d] item[%d] armor enchant failed, prev enchanted[%d], dice[%f], prob[%f]", pUser->nDBID, pItem->nDBID, nEnchantLevel, dice, chance);
							if(pScroll->pII->etcType == EtcItemBlessedScrollEnchantWeapon || pScroll->pII->etcType == EtcItemBlessedScrollEnchantArmor)
							{
								//1517	1	a,Failed in Blessed Enchant. The enchant value of the item became 0.\0	0	79	9B	B0	FF	a,ItemSound3.sys_enchant_failed\0	a,	0	0	0	0	0	a,	a,none\0
								pSocket->SendSystemMessage(1517);
								if(g_StackableEnchantScrolls)
								{
									g_DB.RequestCustomEnchantItem(pUser->nDBID, pItem->nDBID, g_BlessedEnchantFailLevel, pScroll->pSID->nItemID);
								}else
								{
									g_DB.RequestEnchantItem(pScroll, pItem, g_BlessedEnchantFailLevel, pUser);
								}
							}else
							{
								if(nEnchantLevel)
								{
									pSocket->Send("cdddddd", 0x64, 65, 2, 1 , pItem->pSID->nEnchantLevel, 3, pItem->pSID->nItemID);
								}else
								{
									pSocket->Send("cdddd", 0x64, 64, 1, 3, pItem->pSID->nItemID);
								}
								if(g_StackableEnchantScrolls)
								{
									INT32 crystalId = 0;
									INT32 crystalCount = pItem->pII->nCrystalCount;
									if(nEnchantLevel > 0)
									{
										crystalCount += (pItem->pII->nCrystalCount / 25 * nEnchantLevel);
									}
									if(pItem->pII->nCrystalType > 0)
									{
										crystalId = 1457 + pItem->pII->nCrystalType;
										pUser->AddItemToInventory(crystalId, crystalCount);
									}
									g_DB.RequestDestroyItem(pItem, 1, pUser);
									g_DB.RequestDestroyItem(pScroll, 1, pUser);
									pUser->pSocket->Send("cd", 0x81, 0);
									pUser->isEnchantingItem = 0;
									typedef void (*f)(UINT64, UINT);
									UINT64 addr = (UINT64)pUser + 0x2930;
									f(0x7F0210)(addr, 0);
									pUser->SendItemList(true);
								}else
								{
									g_DB.RequestEnchantFail(pScroll, pItem, pUser);
								}
							}
						}
					}
				}else
				{
					pUser->SendSystemMessage(L"This item already got max enchant level.");
					CancelEnchant(pUser);
				}
			}
		}
	}
	
	return true;
}

void CItemEnchant::CancelEnchant(User *pUser)
{
	if(pUser->ValidUser())
	{
		(*(BYTE*)((INT64)pUser + 0x1AAC)) = 0;
	}
}