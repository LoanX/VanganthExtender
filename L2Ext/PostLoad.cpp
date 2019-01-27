#include "StdAfx.h"
#include "PostLoad.h"
#include "PacketHandler.h"
#include "HerbSystem.h"
#include "AntiDDoS.h"
#include "DB.h"
#include "VIPSystem.h"
#include "ItemEnchant.h"
#include "OfflineShop.h"
#include "SkillAcquireDB.h"
#include "AIO.h"
#include "ObsceneFilter.h"
#include "TerritoryData.h"
#include "ClanSys.h"
#include "ClassManager.h"
#include "TeleportBypass.h"
#include "TvT.h"
#include "Augmentation.h"
#include "AutoLearn.h"
#include "MiningSystem.h"
#include "CursedWeaponSystem.h"
#include "SocketLimiter.h"
#include "IpBlocker.h"
#include "AutoAnnounce.h"
#include "DropList.h"
#include "AutoLoot.h"
#include "DnN.h"
#include "MultiSellStat.h"
#include "VoteSystem.h"
#include "AutoStart.h"
#include "ItemDelivery.h"
#include "FractionSystem.h"
#include "HeroSystem.h"
#include "WorldCollision.h"
#include "UserExCommand.h"
#include "SchemeBuffer.h"
#include "OnlineMultipler.h"
#include "SkillDataParser.h"
#include "ItemDataParser.h"
#include "DeathPenalty.h"
#include "HtmlFilter.h"
#include "OlympiadSystem.h"
#include "InfinityShot.h"
#include "HtmlCache.h"
#include "DuelSystem.h"
#include "MultiSell.h"
#include "ArmorPenalty.h"
#include "Auction.h"
#include "VisualArmor.h"
#include "PlayerCustomizer.h"
#include "VoteReward.h"
#include "ClanSys.h"
#include "Wedding.h"
#include "NpcDb.h"
#include "RaidBossStatus.h"
#include "CastleSiegeManager.h"
#include "CliExt.h"
#include "PKPvPStatus.h"
#include "ItemAutoConsume.h"
#include "LoginDb.h"
#include "CharacterLock.h"
#include "DailyPvP.h"
#include "SubStack.h"
#include "SpawnProtection.h"
#include "SkillMaster.h"
#include "License.h"
#include "ChatManager.h"
#include "L2NetworkVote.h"
#include "PrivateStore.h"
#include "Rebirth.h"
#include "DBNpcMaker.h"
#include "ChampionNpc.h"
#include "LureProtection.h"
#include "GradePenalty.h"
#include "DoorData.h"
#include "RecipeManager.h"
#include "PIN.h"
#include "PvPAnnounce.h"
#include "ClanPvPStatus.h"
#include "KeyEngine.h"

extern double g_PartyExpRate;


using namespace License;

#pragma optimize("", off)

void CPostLoad::Initialize()
{
	VIRTUALIZER_TIGER_RED_START;
	if(CLicense::GetInstance().CanUse(LicenseExtUse))
	{
		if(!IsHardwareIdHack())
		{
			g_HookManager.WriteCall( 0x6443C2, InitKey, 0);
		}
	}
	VIRTUALIZER_TIGER_RED_END;
}

void CPostLoad::InitKey(LPVOID pInstance, CLog::LogType Type, wchar_t* Msg)
{
	VIRTUALIZER_TIGER_RED_START;
	if(CLicense::GetInstance().CanUse(LicenseExtUse))
	{
		g_HookManager.WriteCall( 0x642F50, FirstLoad, 0);
		g_HookManager.WriteCall( 0x642FD4, SecondLoad, 0);
		g_HookManager.WriteCall( 0x643781, ThirdLoad, 0);
	}
	g_Log.Add(Type, Msg);
	VIRTUALIZER_TIGER_RED_END;
}

void CPostLoad::FirstLoad(LPVOID Instance)
{
	VIRTUALIZER_TIGER_RED_START;

	typedef void (*_f)(LPVOID);
	_f LoadPathNode = (_f) 0x0073FE70;
	LoadPathNode(Instance);

	CKeyEngine& ke = CKeyEngine::getInstance();
	ke.init();
	if(ke.validated() && ke.getState() == KeyEngine::StateValid)
	{
		if(CLicense::GetInstance().CanUse(LicenseExtUse) && !IsHardwareIdHack())
		{
			g_Log.Add(CLog::Blue, "[Ext] Initializing Online Multipler.");
			g_OnlineMultipler.Init();
			if(int UserLimit = g_Config.L2ServerConfig.GetUserLimit())
			{
				g_OnlineMultipler.SetMaxUsers(UserLimit);
				g_Log.Add(CLog::Blue, "[Ext] Setting new user limit[%d].", UserLimit);
			}

			g_HtmlFilter.Init();

			g_Log.Add(CLog::Blue, "[Ext] Initializing Clan System.");
			ClanSys::Initialize();
			ClanSys::ApplyNewPenalty();
			g_ResidenceDBEx.Load();

			g_Log.Add(CLog::Blue, "[Ext] Initializing Item Enchant System.");
			CItemEnchant::Initialize();

			g_Log.Add(CLog::Blue, "[Ext] Initializing Auto Learn Skill System");
			CAutoLearn::Initialize();

			SocketLimiter::Initialize();

			g_DuelSystem.Initialize();

			g_Log.Add(CLog::Blue, "[Ext] Initializing Manor System");
			CManor::Initialize();

			if(g_Config.IsSet(CConfig::SIEGE_REPORT))
			{
				g_Log.Add(CLog::Blue, "[Ext] Initializing Siege Stats System");
				CSiegeStat::GetInstance().Initialize();
			}

			g_Log.Add(CLog::Blue, "[Ext] Initializing Offline Shop System");
			g_OfflineShop.Initialize();

			g_ArmorMasteryDB.ReadData();
			g_ArmorPenalty.Initialize();
			
			g_SpawnProtection.Initialize();
			
			if(g_Config.IsSet(CConfig::ITEM_REUSE_MANAGER))
			{
				g_ItemReuseManager.Initialize();
			}
			if(g_Config.IsSet(CConfig::SKILL_REUSE_MANAGER))
			{
				g_SkillReuseManager.Initialize();
			}
			
			g_PlayerCustomizer.Init();

			if(g_Config.IsSet(CConfig::SPIRIT_SYSTEM))
			{
				g_SpiritSystem.Initialize();
			}

			g_MiningSystem.Init();

			g_Log.Add(CLog::Blue, "[Ext] Initializing Logger");
			g_Logger.Initialize();

			g_Log.Add(CLog::Blue, "[Ext] Initializing Skill Data Parser");
			SkillDataParser::Initialize();

			g_Log.Add(CLog::Blue, "[Ext] Initializing Item Data Parser");
			CItemDataParser::Init();

			g_Log.Add(CLog::Blue, "[Ext] Initializing Packet Handler");
			PacketHandler::Initialize();

			g_UserExCommand.Init();

			CDBPacket::Init();
			g_Log.Add(CLog::Blue, "[Ext] Initializing Team Container");
			g_TeamContainer.Initialize();

			g_Log.Add(CLog::Blue, "[Ext] Initializing VIP System");
			g_VIPSystem.Init();

			g_Log.Add(CLog::Blue, "[Ext] Initializing Enchanted Set System");
			g_EnchantedSet.Initialize();
			g_Log.Add(CLog::Blue, "[Ext] Loading Augmentation Data");
			g_Augmentation.ReadStatData();
			g_Augmentation.ReadSkillData();
			g_Augmentation.ReadNameData();
			g_Log.Add(CLog::Blue, "[Ext] Augmenation : BlockedGlows[%d] BlockedItems[%d]", g_Config.AugmentationInfo.GetBlockedGlowCount(), g_Config.AugmentationInfo.GetBlockedItemCount());
			
			g_SkillDBEx.Initialize();
			g_NpcDb.Init();
			g_UserDB.Initialize();
			g_TeleportBypass.Init();
			g_AIOSystem.Init();
			g_ObsceneFilter.Init();
			CNpcSocket::Initialize();
			CL2LevelUP::StartSystem();
			g_IpBlocker.Initialize();
			srand(time(NULL));
			InitializeSkillEnchanting2();

			g_ItemAutoConsume.Init();

			g_PrivateStoreSystem.Init();

			g_WorldCollision.Init();
			g_HtmlCache.Init();
			g_LoginDb.Init();
		}
	}
	else
	{
		ke.showInvalidKeyMessage();
		ExitProcess(-1);
	}
	VIRTUALIZER_TIGER_RED_END;
}

void CPostLoad::SecondLoad(LPVOID Instance)
{
	VIRTUALIZER_TIGER_RED_START;

	typedef void (*_f)(LPVOID);
	_f LoadItems = (_f) 0x006FC060;
	LoadItems(Instance);

	if(CLicense::GetInstance().CanUse(LicenseExtUse) && !IsHardwareIdHack())
	{
		g_Log.Add(CLog::Blue, "[Ext] Initializing MultiSell Parser");
		g_MultiSellDB.Init();
		g_MultiSellStat.Init();
		g_CursedWeaponSystem.Initialize();
		g_SkillAcquireDB.Init();
		g_ClassManager.Init();
		g_ItemDBEx.Init();

		g_L2Time.SetTime(7*60);

		if(g_Config.IsSet(CConfig::ADMIN_SOCKET))
			g_AdminSocket.Init();

		g_NpcMaker.Initialize();
		g_TvT.Init();
		g_AntiDDoS.LoadINI();
		g_DropList.Init();
		g_AutoLoot.Init();
		g_HerbSystem.Init();

		g_DeathPenalty.Init();

		g_TerritoryChecker.Initialize();
		g_SchemeBuffer.Init();
		g_InfinityShot.Init();
		COlympiadSystem::Initialize();
		g_VisualArmor.Init();
		g_Wedding.Init();
		g_SkillMaster.Init();
		g_RebirthSystem.Init();
		g_ChampionNpc.Init();
		g_GradePenalty.Init();
		CRecipeManager::Get().Init();
		
		double *lpPartyExpRate = (double*)0xBEF280;
		g_PartyExpRate = (*lpPartyExpRate);
		(*lpPartyExpRate) = 1.0;
	}

	VIRTUALIZER_TIGER_RED_END;
}

void CPostLoad::ThirdLoad(LPVOID pInstance)
{
	VIRTUALIZER_TIGER_RED_START;

	typedef void (*f)(LPVOID);
	f(0x65AEB0L)(pInstance);

	if(CLicense::GetInstance().CanUse(LicenseExtUse) && !IsHardwareIdHack())
	{
		if(g_VIPSystem.IsEnabled())
		{
			g_Log.Add(CLog::Blue, "[Ext] Loading VIP accounts...");
			g_DB.RequestLoadVIPAccounts();
		}

		g_CliExt.Init();

		g_DB.RequestLoadBannedAccounts();
		g_DB.RequestLoadHwidLockData();
		g_DB.RequestLoadAdditionalBoxData();

		g_AutoAnnounce.Init();
		g_DnNSystem.Init();
		g_VoteSystem.Init();
		g_HeroSystem.Init();
		g_FractionSystem.Init();
		g_SubStack.Init();
		g_ItemDelivery.Init();
		g_Auction.Init();
		
		g_PINSystem.Init();

		g_ChatManager.Init();
		g_DoorData.Init();

		g_HtmlCache.LoadHtmls();

		g_AutoStart.Init();
		g_AutoStart.StartApps();

		g_VoteReward.Init();
		g_RaidBossStatus.Init();
		g_CastleSiegeManager.Init();
		g_PKPvPStatus.Init();
		g_ClanPvPStatus.Init();
		g_CharacterLock.Init();
		g_DailyPvP.Init();
		g_L2NetworkVote.Init();
		g_DBNpcMaker.Init();
		g_LureProtection.Init();
		PvP::CPvPAnnounce::GetInstance().Init();
	}

	VIRTUALIZER_TIGER_RED_END;
}

bool CPostLoad::IsHardwareIdHack()
{
	VIRTUALIZER_TIGER_RED_START;
	bool ret = false;

	//typedef bool (__stdcall *UpdateEnvironmentFn)(void);


	//typedef bool (__stdcall *VerifyKeyFn)(const char *name, const char *code);




	VIRTUALIZER_TIGER_RED_END;
	return ret;
}

#pragma optimize("", on)
