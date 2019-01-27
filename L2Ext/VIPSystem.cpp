#include "StdAfx.h"
#include "VIPSystem.h"
#include "AccountDB.h"
#include "DB.h"
#include "DnN.h"
#include "HtmlCache.h"

CVIPSystem g_VIPSystem;

DOUBLE* g_lpExpRate = (DOUBLE*) 0xBEF278;
DOUBLE* g_lpDropRate = (DOUBLE*)0xBEF268;
DOUBLE* g_lpSpoilRate = (DOUBLE*)0xBEF270;
LPINT g_lpConnectedUsers = (LPINT) 0x1557E0C;

extern CCreature* g_NpcKiller[26];
extern CNPC* g_KilledNpc[26];

int GetUserLimitWrapper(int accountId)
{
	int count = (*g_lpConnectedUsers);

	if(UINT timestamp = g_AccountDB.IsBanned(accountId))
	{
		g_Log.Add(CLog::Blue, "[%s] banned account[%d] timestamp[%d] currentTime[%d]", __FUNCTION__, accountId, timestamp, time(0));
		return INT_MAX;
	}

	int vipLevel = g_AccountDB.GetVIPLevel(accountId);
	if(vipLevel > 0)
	{
		VIPInfo vipInfo = g_VIPSystem.GetInfo(vipLevel);
		if(vipInfo.canLoginWhenFull)
		{
			return 0;
		}
	}
	return count;
}

double GetCraftRateWrapper(CCreature* pCreature, CRecipeInfo* pRecipeInfo)
{
	return g_VIPSystem.GetCraftRate(pCreature, pRecipeInfo);
}

double GetSpoilRateWrapper(CCreature* pCreature)
{
	double spoilRate = (*g_lpSpoilRate);
	if(pCreature->ValidCreature())
	{
		spoilRate *= g_VIPSystem.GetSpoilRate(pCreature);
		spoilRate *= g_DnNSystem.GetRate(DnN::RateSpoil);
		if(User *pUser = pCreature->GetUser())
		{
			spoilRate *= pUser->pSD->pExData->SkillMod.bonusSpoil;
		}
	}

//	g_Log.Add(CLog::Blue, "[%s] [%f] [%S]", __FUNCTION__, spoilRate, pCreature->pSD->wszName);
	return spoilRate;
}

double GetDropRateWrapper()
{
	double dropRate = (*g_lpDropRate);
	UINT threadIdx = GetThreadIndex();
	if(threadIdx < 26 )
	{
		CCreature *pKiller = g_NpcKiller[threadIdx];
		CNPC *pNPC = g_KilledNpc[threadIdx];
		if(pKiller->ValidCreature() && pNPC->ValidCreature() && !pNPC->IsSummon() && !pNPC->IsPet())
		{
			if(pNPC->pContributeData)
			{
				User *pUser = User::GetUserBySID(&pNPC->pContributeData->contributorId);
				if(pUser->ValidUser())
				{
					if(!pNPC->IsInRange(pUser, 2000))
					{
						pUser = 0;
					}
				}

				if(!pUser->ValidUser())
				{
					pUser = pKiller->GetUser();
					if(pKiller->IsSummon())
					{
						pUser = pKiller->CastSummon()->GetMaster();
					}
					if(pUser->ValidUser())
					{
						if(!pNPC->IsInRange(pUser, 2000))
						{
							pUser = 0;
						}
					}
				}
				double itemRateBonus = 1.0;
				double adenaBonus = 1.0;
				double raidBossBonus = 1.0;
				double raidBossOnlyChanceBonus = 1.0;
				double epicBossBonus = 1.0;
				double onlyChanceBonus = 1.0;

				if(pUser->ValidUser())
				{
					pUser->GetDropBonuses(itemRateBonus, adenaBonus, raidBossBonus, raidBossOnlyChanceBonus, epicBossBonus, onlyChanceBonus);

					if(pNPC->IsBoss() || pNPC->IsZZoldagu())
					{
						return 1.0;	
					}else
					{
						dropRate *= g_VIPSystem.GetDropRate(pUser);

						dropRate *= itemRateBonus;
					}
				}
			}

		//	g_Log.Add(CLog::Blue, "[%s] [%f] [%S]", __FUNCTION__, dropRate, pKiller->pSD->wszName);
		}
	}
	return dropRate;
}

CVIPSystem::CVIPSystem()
{
}

CVIPSystem::~CVIPSystem()
{
}

void CVIPSystem::Init()
{
	m_NotVIPLevelRestriction = 0;
	LoadINI();
	if(m_Enabled)
	{
		//Hooks
		g_HookManager.WriteCall(0x7737A3, CraftRateHook1, 1);
		g_HookManager.WriteCall(0x772BA9, CraftRateHook2, 1);
	}
	g_HookManager.WriteCall(0x6C45F8,DropRateHook, 3);
	g_HookManager.WriteCall(0x6C49E5,DropRateHook, 3);
	g_HookManager.WriteCall(0x7D092F, SpoilRateHook, 3);
	g_HookManager.WriteCall(0x58E650, UserLimitHook, 1);
}

void CVIPSystem::LoadINI()
{
	m_Info.clear();

	const TCHAR* sectionName = _T("VIPSystem");
	m_Enabled = GetPrivateProfileInt(sectionName, _T("Enabled"), 0, g_ConfigFile);
	UINT availableLevels = GetPrivateProfileInt(sectionName, _T("AvailableLevels"), 0, g_ConfigFile);
	m_NotVIPLevelRestriction = GetPrivateProfileInt(sectionName, _T("NotVIPLevelRestriction"), 0, g_ConfigFile);
	for(UINT n=1;n<=availableLevels;n++)
	{
		VIPInfo info;
		info.level = n;
		{
			tstringstream skey;
			skey << n << "_RequiredItemId";
			info.requiredItem.itemId = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		{
			tstringstream skey;
			skey << n << "_RequiredItemCount";
			info.requiredItem.amount = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		{
			tstringstream skey;
			skey << n << "_RequiredLevel";
			info.requiredLevel = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		{
			tstringstream skey;
			skey << n << "_Timestamp";
			info.timestamp = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		{
			tstringstream skey;
			skey << n << "_MagicWeaponEnchantBonus";
			info.magicWeaponEnchantBonus = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		{
			tstringstream skey;
			skey << n << "_NormalWeaponEnchantBonus";
			info.normalWeaponEnchantBonus = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		{
			tstringstream skey;
			skey << n << "_ArmorEnchantBonus";
			info.armorEnchantBonus = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		{
			tstringstream skey;
			skey << n << "_AdenaRateBonus";
			info.adenaRateBonus = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		{
			tstringstream skey;
			skey << n << "_DropRateBonus";
			info.dropRateBonus = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		{
			tstringstream skey;
			skey << n << "_SpoilRateBonus";
			info.spoilRateBonus = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		{
			tstringstream skey;
			skey << n << "_ExpRateBonus";
			info.expRateBonus = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		{
			tstringstream skey;
			skey << n << "_SpRateBonus";
			info.spRateBonus = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		{
			tstringstream skey;
			skey << n << "_CraftRateBonus";
			info.craftRateBonus = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		{
			tstringstream skey;
			skey << n << "_SpecialChat";
			info.specialChat = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		{
			tstringstream skey;
			skey << n << "_ChatDelay";
			info.chatDelay = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		{
			tstringstream skey;
			skey << n << "_LoginWhenFull";
			info.canLoginWhenFull = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		{
			tstringstream skey;
			skey << n << "_SkillId";
			info.skillId = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		{
			tstringstream skey;
			skey << n << "_SkillLevel";
			info.skillLevel = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		{
			tstringstream skey;
			skey << n << "_RewardId";
			info.reward.itemId = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		{
			tstringstream skey;
			skey << n << "_RewardAmount";
			info.reward.amount = GetPrivateProfileInt(sectionName, skey.str().c_str(), 0, g_ConfigFile);
		}
		m_Info.push_back(info);
	}
}

void CVIPSystem::OnLoad(User *pUser)
{
	if(pUser->pSD->vipLevel)
	{
		SendExpireInfo(pUser);
	}
}

VIPInfo& CVIPSystem::GetInfo(int vipLevel)
{
	for(UINT n=0;n<m_Info.size();n++)
	{
		if(m_Info[n].level == vipLevel)
		{
			return m_Info[n];
		}
	}

	return m_BaseInfo;
}

bool CVIPSystem::RequestChangeLevel(User *pUser, int newLevel)
{
	if(pUser->ValidUser())
	{
		if(m_Enabled)
		{
			if(pUser->IsNowTrade())
				pUser->TradeCancel();

			VIPInfo& info = GetInfo(newLevel);

			if(info.level == 0)
			{
				//2437	1	Invalid VIP status.	0	79	9B	B0	FF			0	0	0	0	0		none
				pUser->pSocket->SendSystemMessage(2437);
				return false;
			}
			if(pUser->pSD->vipLevel > newLevel)
			{
				//2436	1	You already have higher vip status.	0	79	9B	B0	FF			0	0	0	0	0		none
				pUser->pSocket->SendSystemMessage(2436);
				return false;
			}

			if(pUser->pSD->nLevel < info.requiredLevel)
			{
				//2438	1	Your character level is too low to acquire current VIP status.	0	79	9B	B0	FF			0	0	0	0	0		none
				pUser->pSocket->SendSystemMessage(2438);
				return false;
			}
			if(info.requiredItem.amount > 0 && info.requiredItem.itemId > 0)
			{
				if(info.requiredItem.itemId == 57)
				{
					if(pUser->inventory.GetAdenaAmount() < info.requiredItem.amount)
					{
						//2439	1	Invalid item count for VIP status.	0	79	9B	B0	FF			0	0	0	0	0		none
						pUser->pSocket->SendSystemMessage(2439);
						return false;
					}
				}else
				{
					if(!pUser->inventory.HaveItemByClassID(info.requiredItem.itemId, info.requiredItem.amount))
					{
						//2439	1	Invalid item count for VIP status.	0	79	9B	B0	FF			0	0	0	0	0		none
						pUser->pSocket->SendSystemMessage(2439);
						return false;
					}
				}
				pUser->DeleteItemInInventory(info.requiredItem.itemId, info.requiredItem.amount);
			}

			if(info.reward.itemId > 0 && info.reward.amount > 0)
			{
				pUser->AddItemToInventory(info.reward.itemId, info.reward.amount);
			}

			if(pUser->pSD->vipLevel > 0)
			{
				//delete old skill
				VIPInfo& infoEx = GetInfo(pUser->pSD->vipLevel);
				if(infoEx.skillId > 0 && infoEx.skillLevel > 0 && (infoEx.skillId != info.skillId || infoEx.skillLevel > info.skillLevel))
				{
					pUser->DeleteSkill(infoEx.skillId);
					pUser->SendDeleteSkillToDB(infoEx.skillId);
					pUser->SetValidateParametersRequest();
				}
			}

			if(info.skillId > 0 && info.skillLevel > 0)
			{
				pUser->AcquireSkill(info.skillId, info.skillLevel);
				pUser->SendAcquireSkillToDB(info.skillId, info.level);
				pUser->SetValidateParametersRequest();
			}

			pUser->SocialAction(SocialActionLevelUp);
			pUser->pSD->vipLevel = newLevel;
			pUser->pSD->vipTimestamp = time(0) + info.timestamp;

			g_DB.RequestSaveVipStatus(pUser->nDBID, newLevel, pUser->pSD->vipTimestamp);
			g_Logger.Add(L"User[%d] acquired vip status level[%d]", pUser->nDBID, newLevel);
			g_AccountDB.SetVIPLevel(pUser->pSocket->accountId, newLevel, pUser->pSD->vipTimestamp);

			int days = info.timestamp / 60 / 60 / 24;
			//2440	1	Congratulations! You've acquired VIP status for $s1 day(s).	0	79	9B	B0	FF			0	0	0	0	0		none
			CSystemMessage msg(2440);
			msg.AddNumber(days);
			pUser->SendSystemMessage(&msg);

		}else
		{
			//2446	1	The VIP System is currently disabled.	0	79	9B	B0	FF			0	0	0	0	0		none
			pUser->pSocket->SendSystemMessage(2446);
		}
	}
	return false;
}

void CVIPSystem::SendExpireInfo(User *pUser)
{
	if(pUser->ValidUser())
	{
		if(pUser->pSD->vipLevel > 0)
		{
			int days = pUser->pSD->vipTimestamp - time(NULL);
			days = days / 60 / 60 / 24;
			//2440	1	Your VIP status will expire in $s1 day(s).	0	79	9B	B0	FF			0	0	0	0	0		none
			if(days >= 0)
			{
				CSystemMessage msg(2441);
				msg.AddNumber(days);
				pUser->SendSystemMessage(&msg);
			}else
			{
				//2442	1	You don't have the VIP status at the moment.	0	79	9B	B0	FF			0	0	0	0	0		none
				pUser->pSocket->SendSystemMessage(2442);
			}
		}else
		{
			//2442	1	You don't have the VIP status at the moment.	0	79	9B	B0	FF			0	0	0	0	0		none
			pUser->pSocket->SendSystemMessage(2442);
		}
	}
}

void CVIPSystem::SendBonusInfo(User *pUser, int level)
{
	VIPInfo& info = GetInfo(level);

	if(info.level > 0)
	{

		const WCHAR* wHtml = g_HtmlCache.Get(L"vip_bonus_template.htm");
		if(wHtml)
		{
			wstring html(wHtml);
			WCHAR rate[32] = { 0 };
			_itow_s(info.magicWeaponEnchantBonus, rate, 32, 10);
			html = Utils::ReplaceString(html, L"<?magic_enchant?>", rate, true);
			memset(rate, 0, sizeof(rate));
			_itow_s(level, rate, 32, 10);
			html = Utils::ReplaceString(html, L"<?vip_level?>", rate, true);
			memset(rate, 0, sizeof(rate));
			_itow_s(info.normalWeaponEnchantBonus, rate, 32, 10);
			html = Utils::ReplaceString(html, L"<?normal_enchant?>", rate, true);
			memset(rate, 0, sizeof(rate));
			_itow_s(info.armorEnchantBonus, rate, 32, 10);
			html = Utils::ReplaceString(html, L"<?armor_enchant?>", rate, true);
			memset(rate, 0, sizeof(rate));
			_itow_s(info.dropRateBonus, rate, 32, 10);
			html = Utils::ReplaceString(html, L"<?drop_rate?>", rate, true);
			memset(rate, 0, sizeof(rate));
			_itow_s(info.spoilRateBonus, rate, 32, 10);
			html = Utils::ReplaceString(html, L"<?spoil_rate?>", rate, true);
			memset(rate, 0, sizeof(rate));
			_itow_s(info.expRateBonus, rate, 32, 10);
			html = Utils::ReplaceString(html, L"<?exp_rate?>", rate, true);
			memset(rate, 0, sizeof(rate));
			_itow_s(info.spRateBonus, rate, 32, 10);
			html = Utils::ReplaceString(html, L"<?sp_rate?>", rate, true);
			memset(rate, 0, sizeof(rate));
			_itow_s(info.craftRateBonus, rate, 32, 10);
			html = Utils::ReplaceString(html, L"<?craft_rate?>", rate, true);
			
			if(info.specialChat)
			{
				html = Utils::ReplaceString(html, L"<?special_chat?>", L"1", true);
			}else
			{
				html = Utils::ReplaceString(html, L"<?special_chat?>", L"0", true);
			}
			if(info.canLoginWhenFull)
			{
				html = Utils::ReplaceString(html, L"<?full_login?>", L"1", true);
			}else
			{
				html = Utils::ReplaceString(html, L"<?full_login?>", L"0", true);
			}
			
			pUser->ShowHTML(L"vip_bonus_template.htm", html.c_str(), 0);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find vip_bonus_template.htm!", __FUNCTION__);
		}
	}
}

void CVIPSystem::TimerExpired(User *pUser)
{
	guard;
	if(m_Enabled)
	{
		if(pUser->ValidUser())
		{
			if(pUser->pSD->vipLevel > 0)
			{
				int timeLeft = pUser->pSD->vipTimestamp - time(NULL);
				if(timeLeft <= 0)
				{
					VIPInfo& infoEx = GetInfo(pUser->pSD->vipLevel);
					if(infoEx.skillId > 0 && infoEx.skillLevel > 0)
					{
						pUser->DeleteSkill(infoEx.skillId);
						pUser->SendDeleteSkillToDB(infoEx.skillId);
					}

					pUser->SetValidateParametersRequest();

					pUser->pSD->vipLevel = 0;
					pUser->pSD->vipTimestamp = 0;
					g_DB.RequestSaveVipStatus(pUser->nDBID, 0, 0);
					//2444	1	Your VIP status just expired.	0	79	9B	B0	FF			0	0	0	0	0		none
					pUser->pSocket->SendSystemMessage(2444);
					g_AccountDB.SetVIPLevel(pUser->pSocket->accountId, 0, 0);
				}else
				{
					if(timeLeft == 1800)
					{
						//2443	1	Your VIP status will expire in $s1 minute(s).	0	79	9B	B0	FF			0	0	0	0	0		none
						CSystemMessage msg(2443);
						msg.AddNumber(30);
					}else if(timeLeft == 600)
					{
						//2443	1	Your VIP status will expire in $s1 minute(s).	0	79	9B	B0	FF			0	0	0	0	0		none
						CSystemMessage msg(2443);
						msg.AddNumber(10);
					}else if(timeLeft == 300)
					{
						//2443	1	Your VIP status will expire in $s1 minute(s).	0	79	9B	B0	FF			0	0	0	0	0		none
						CSystemMessage msg(2443);
						msg.AddNumber(5);
					}else if(timeLeft == 60)
					{
						//2443	1	Your VIP status will expire in $s1 minute(s).	0	79	9B	B0	FF			0	0	0	0	0		none
						CSystemMessage msg(2443);
						msg.AddNumber(1);
					}
				}
			}
		}
	}
	unguard;
}

void CVIPSystem::HandleCommand(User *pUser, wstring wCommand)
{
	guard;

	if(m_Enabled)
	{
		const WCHAR* wHtml = g_HtmlCache.Get(L"vip.htm");
		if(wHtml)
		{
			pUser->ShowHTML(L"vip.htm", wHtml, 0);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find html vip.htm!", __FUNCTION__);
		}
	}

	unguard;
}

int CVIPSystem::GetSP(int baseSP, int vipLevel)
{
	VIPInfo vipInfo = GetInfo(vipLevel);
	if(vipInfo.spRateBonus)
	{
		double bonus = (double)(vipInfo.spRateBonus + 100);
		bonus /= 100;

		double sp = (double)baseSP;
		sp *= bonus;

//		g_Log.Add(CLog::Blue, "[%s] IncSP[%d] bonus[%f] OutExp[%d]", __FUNCTION__, baseSP, bonus, (int)sp);

		return (int)sp;
	}

	return baseSP;
}

int CVIPSystem::GetExp(int baseExp, int vipLevel)
{
	VIPInfo vipInfo = GetInfo(vipLevel);
	if(vipInfo.expRateBonus)
	{
		double bonus = (double)(vipInfo.expRateBonus + 100);
		bonus /= 100;

		double exp = (double)baseExp;
		exp *= bonus;

//		g_Log.Add(CLog::Blue, "[%s] IncExp[%d] bonus[%f] OutExp[%d]", __FUNCTION__, baseExp, bonus, (int)exp);

		return (int)exp;
	}

	return baseExp;
}

double CVIPSystem::GetSpoilRate(CCreature *pCreature)
{
	double spoilRate = 1.0;

	if(pCreature->ValidUser())
	{
		if(pCreature->pSD->vipLevel)
		{
			VIPInfo vipInfo = GetInfo(pCreature->pSD->vipLevel);
			if(vipInfo.spoilRateBonus)
			{
				double bonus = (double)(vipInfo.spoilRateBonus + 100);
				bonus /= 100;
				spoilRate *= bonus;

//				g_Log.Add(CLog::Blue, "[%s] spoilRate[%f]", __FUNCTION__, spoilRate);
			}
		}
	}

	return spoilRate;
}

double CVIPSystem::GetAdenaRate(CCreature *pCreature)
{
	double dropRate = 1.0;
	if(pCreature->ValidCreature())
	{
		if(pCreature->ValidUser())
		{
			if(pCreature->pSD->vipLevel)
			{
				VIPInfo vipInfo = GetInfo(pCreature->pSD->vipLevel);
				if(vipInfo.adenaRateBonus)
				{
					double bonus = (double)(vipInfo.adenaRateBonus + 100);
					bonus /= 100;
					dropRate *= bonus;

		//			g_Log.Add(CLog::Blue, "[%s] bonus[%f] DropRate[%f]", __FUNCTION__, bonus, dropRate);
				}
			}
		}else if (pCreature->IsSummon())
		{
			CSummon *pSummon = (CSummon*)pCreature;
			if(User *pUser = pSummon->GetMaster())
			{
				if(pUser->pSD->vipLevel)
				{
					VIPInfo vipInfo = GetInfo(pUser->pSD->vipLevel);
					if(vipInfo.adenaRateBonus)
					{
						double bonus = (double)(vipInfo.adenaRateBonus + 100);
						bonus /= 100;
						dropRate *= bonus;
					}
				}
			}
		}
	}

	return dropRate;
}

double CVIPSystem::GetDropRate(CCreature *pCreature)
{
	double dropRate = 1;
	if(pCreature->ValidCreature())
	{
		if(pCreature->ValidUser())
		{
			if(pCreature->pSD->vipLevel)
			{
				VIPInfo vipInfo = GetInfo(pCreature->pSD->vipLevel);
				if(vipInfo.dropRateBonus)
				{
					double bonus = (double)(vipInfo.dropRateBonus + 100);
					bonus /= 100;
					dropRate *= bonus;

		//			g_Log.Add(CLog::Blue, "[%s] bonus[%f] DropRate[%f]", __FUNCTION__, bonus, dropRate);
				}
			}
		}else if (pCreature->IsSummon())
		{
			CSummon *pSummon = (CSummon*)pCreature;
			if(User *pUser = pSummon->GetMaster())
			{
				if(pUser->pSD->vipLevel)
				{
					VIPInfo vipInfo = GetInfo(pUser->pSD->vipLevel);
					if(vipInfo.dropRateBonus)
					{
						double bonus = (double)(vipInfo.dropRateBonus + 100);
						bonus /= 100;
						dropRate *= bonus;
					}
				}
			}
		}
	}

	return dropRate;
}

double CVIPSystem::GetCraftRate(CCreature *pCreature, CRecipeInfo *pRecipeInfo)
{
	double craftRate = (double)pRecipeInfo->SuccessRate;

	if(pCreature->ValidUser())
	{
		if(pCreature->pSD->vipLevel)
		{
			VIPInfo vipInfo = GetInfo(pCreature->pSD->vipLevel);
			if(vipInfo.craftRateBonus)
			{
				double bonus = (double)(vipInfo.craftRateBonus + 100);
				bonus /= 100;
				craftRate *= bonus;

			//	g_Log.Add(CLog::Blue, "[%s] bonus[%f] rate[%f] user[%S]", __FUNCTION__, bonus, craftRate, pCreature->pSD->wszName);
			}
		}
	}

	return craftRate;
}

void CVIPSystem::OnHelp(User *pUser)
{
	guard;

	if(m_Enabled)
	{
		const WCHAR* wHtml = g_HtmlCache.Get(L"vip_help.htm");
		if(wHtml)
		{
			pUser->ShowHTML(L"vip_help.htm", wHtml, 0);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find vip_help.htm!", __FUNCTION__);
		}
	}

	unguard;
}

void CVIPSystem::OnStatus(User *pUser)
{
	guard;
	if(m_Enabled)
	{
		const WCHAR* wHtml = g_HtmlCache.Get(L"vip_status.htm");
		if(wHtml)
		{
			wstring html(wHtml);
			WCHAR tmp[32] = { 0 };
			_itow_s(pUser->pSD->vipLevel, tmp, 32, 10);
			html = Utils::ReplaceString(html, L"<?vip_level?>", tmp, true);

			UINT timestamp = pUser->pSD->vipTimestamp - time(0);
			UINT seconds = timestamp % 60;
			timestamp /= 60;
			UINT minutes = timestamp % 60;
			timestamp /= 60;
			UINT hours = timestamp % 24;
			timestamp /= 24;
			UINT days = timestamp;
			if(pUser->pSD->vipLevel == 0)
			{
				minutes = hours = days = 0;
			}
			
			//Your VIP status will expire in <font color="LEVEL"><?vip_day?> day(s) <?vip_hour?> hour(s) and <?vip_minute?> minute(s)</font> .</br></br>

			_itow_s(days, tmp, 32, 10);
			html = Utils::ReplaceString(html, L"<?vip_day?>", tmp, true);
			_itow_s(hours, tmp, 32, 10);
			html = Utils::ReplaceString(html, L"<?vip_hour?>", tmp, true);
			_itow_s(minutes, tmp, 32, 10);
			html = Utils::ReplaceString(html, L"<?vip_minute?>", tmp, true);

			pUser->ShowHTML(L"vip_status.htm", html.c_str(), 0);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find vip_status.htm!", __FUNCTION__);
		}
	}
	unguard;
}