#include "StdAfx.h"
#include "MenuExtension.h"
#include "HerbSystem.h"
#include "AntiDDoS.h"
#include "ItemEnchant.h"
#include "BuildCmdHandler.h"
#include "TerritoryData.h"
#include "SchemeBuffer.h"
#include "TvT.h"
#include "Augmentation.h"
#include "MiningSystem.h"
#include "IpBlocker.h"
#include "TeleportBypass.h"
#include "VoteSystem.h"
#include "AutoAnnounce.h"
#include "ObsceneFilter.h"
#include "ArmorPenalty.h"
#include "VisualArmor.h"
#include "CliExt.h"
#include "ChatManager.h"
#include "DBNpcMaker.h"
#include "ClassManager.h"

void CMenuExtension::Install()
{
	g_HookManager.WriteCall(0x63FB68, CMenuExtension::HandleCommand, 1);
	g_HookManager.WriteCall(0x64438E, ShowWindowHook, 1);
};

BOOL CMenuExtension::ShowWindowHook(HWND hWnd, int nCmdShow)
{
	HMENU hOriginalMenu = GetMenu(hWnd);	
	HMENU hExtMenu = CreateMenu();
	AppendMenu(hExtMenu, 0, KILL_DEATH_STAT_ID, _T("Switch Kill/Death Stats"));
	AppendMenu(hExtMenu, 0, RELOAD_ANTI_DDOS, _T("Reload AntiDDoS"));
	AppendMenu(hExtMenu, 0, RELOAD_ARMOR_PENALTY_ID, _T("Reload Armor Penalty"));
	AppendMenu(hExtMenu, 0, RELOAD_AUGMENTATION_ID, _T("Reload Augmentation"));
	AppendMenu(hExtMenu, 0, RELOAD_AUTO_ANNOUNCE, _T("Reload Auto Announce"));
	AppendMenu(hExtMenu, 0, RELOAD_ALLOWED_HWID, _T("Reload Allowed HWID"));
	AppendMenu(hExtMenu, 0, RELOAD_BANLIST, _T("Reload BanList"));
	AppendMenu(hExtMenu, 0, RELOAD_BUILDER_LIST, _T("Reload Builder List"));
	AppendMenu(hExtMenu, 0, RELOAD_CHAT_MANAGER, _T("Reload Chat Manager"));
	AppendMenu(hExtMenu, 0, RELOAD_CLASS_MANAGER, _T("Reload Class Manager"));
	AppendMenu(hExtMenu, 0, RELOAD_DB_NPC_MAKER, _T("Reload DB NpcMaker"));
	AppendMenu(hExtMenu, 0, RELOAD_ENCHANT_ID, _T("Reload Enchant Rates"));
	AppendMenu(hExtMenu, 0, RELOAD_ITEM_DATA_EX, _T("Reload ItemDataEx"));
	AppendMenu(hExtMenu, 0, RELOAD_ITEM_ENCHANT, _T("Reload Item Enchant Rates"));
	AppendMenu(hExtMenu, 0, RELOAD_HERBS_ID, _T("Reload Herb Data"));
	AppendMenu(hExtMenu, 0, RELOAD_MINING_DATA, _T("Reload MiningData"));
	AppendMenu(hExtMenu, 0, RELOAD_NPC_POS_EX, _T("Reload NpcPosEx"));
	AppendMenu(hExtMenu, 0, RELOAD_OBSCENE_FILTER, _T("Reload Obscene Filter"));
	AppendMenu(hExtMenu, 0, RELOAD_SCHEME_BUFFER, _T("Reload Scheme Buffer"));
	AppendMenu(hExtMenu, 0, RELOAD_SPIRIT_SYSTEM_ID, _T("Reload Spirit System"));
	AppendMenu(hExtMenu, 0, RELOAD_TELEPORT_DATA_ID, _T("Reload Teleport Data"));
	AppendMenu(hExtMenu, 0, RELOAD_TVT, _T("Reload TvT"));
	AppendMenu(hExtMenu, 0, RELOAD_TERRITORY_DATA_ID, _T("Reload Territory Data"));
	AppendMenu(hExtMenu, 0, RELOAD_VISUAL_ARMOR, _T("Reload VisualArmor Data"));
	AppendMenu(hExtMenu, 0, RELOAD_VOTE_SYSTEM, _T("Reload Vote System"));
	
	InsertMenu(hOriginalMenu, -1, MF_BYPOSITION|MF_POPUP, (UINT_PTR)hExtMenu, _T("Extender"));	
	return ShowWindow(hWnd, nCmdShow);
};

LRESULT CALLBACK CMenuExtension::HandleCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_COMMAND)
	{
		switch (LOWORD(wParam)) 
		{ 
		case RELOAD_HERBS_ID:
			{
				g_Log.Add(CLog::Blue, "Reloading Herb Data...");
				g_HerbSystem.Init();
				return TRUE;
				break;
			}
		case RELOAD_ENCHANT_ID:
			{
				g_Log.Add(CLog::Blue, "Reloading Enchant Rates...");
				CItemEnchant::LoadINI();
				return TRUE;
				break;
			}
		case RELOAD_CLASS_MANAGER:
			{
				g_Log.Add(CLog::Blue, "Reloading Class Manager...");
				g_ClassManager.Init();
				return TRUE;
			}
		case RELOAD_SPIRIT_SYSTEM_ID:
			{
				g_Log.Add(CLog::Blue, "Reloading Spirit System...");
				g_Config.SpiritSystemInfo.Load();
				g_SpiritSystem.Initialize();
				return TRUE;
				break;
			}
		case RELOAD_CHAT_MANAGER:
			{
				g_ChatManager.SetReloadTimestamp(time(0) + 2);
				return TRUE;
			}
		case RELOAD_SCHEME_BUFFER:
			{
				g_Log.Add(CLog::Blue, "Reloading Scheme Buffer...");
				g_SchemeBuffer.RequestReload();
				return TRUE;
				break;
			}
		case RELOAD_ARMOR_PENALTY_ID:
			{
				g_Log.Add(CLog::Blue, "Reloading Armor Penalty System...");
				g_ArmorPenalty.Initialize();
				g_ArmorMasteryDB.ReadData();
				g_Log.Add(CLog::Blue, "Armor Penalty System has been reloaded.");
				return TRUE;
				break;
			}
		case RELOAD_DB_NPC_MAKER:
			{
				g_DBNpcMaker.Init();
				return TRUE;
			}
		case KILL_DEATH_STAT_ID:
			{
				bool IsSet = g_Config.IsSet(CConfig::KILL_DEATH_STAT);
				if(IsSet)
				{
					g_Log.Add(CLog::Blue, "Kill / Death Stats have been disabled.");
					g_Config.SetSystem(CConfig::KILL_DEATH_STAT, false);
				}else
				{
					g_Log.Add(CLog::Blue, "Kill / Death Stats have been enabled.");
					g_Config.SetSystem(CConfig::KILL_DEATH_STAT, true);
				}
				return TRUE;
				break;
			}
		case RELOAD_TERRITORY_DATA_ID:
			{
				g_TerritoryChecker.Initialize();
				return TRUE;
				break;
			}
		case RELOAD_AUGMENTATION_ID:
			{
				g_Config.AugmentationInfo.Load();
				g_Log.Add(CLog::Blue, "Augmentation - Reloaded chances from ini file.");
				g_Augmentation.ReadSkillData();
				g_Augmentation.ReadStatData();
				g_Augmentation.ReadNameData();
				return TRUE;
				break;
			}
		case RELOAD_ITEM_DATA_EX:
			{
				g_ItemDBEx.Init();
				return TRUE;
				break;
			}
		case RELOAD_BANLIST:
			{
				g_IpBlocker.ReadData();
				return TRUE;
				break;
			}
		case RELOAD_MINING_DATA:
			{
				g_MiningSystem.LoadData();
				return TRUE;
				break;
			}
		case RELOAD_NPC_POS_EX:
			{
				g_NpcMaker.SetRequestReload();
				return TRUE;
				break;
			}
		case RELOAD_TVT:
			{
				g_TvT.Init();
				return TRUE;
				break;
			}
		case RELOAD_ANTI_DDOS:
			{
				g_AntiDDoS.LoadINI();
				return TRUE;
				break;
			}
		case RELOAD_ITEM_ENCHANT:
			{
				CItemEnchant::LoadINI();
				g_Log.Add(CLog::Blue, "Item Enchant - Reloaded data from ini file.");
				return TRUE;
				break;
			}
		case RELOAD_BUILDER_LIST:
			{
				CBuilderCommand::LoadBuilderList();
				g_Log.Add(CLog::Blue, "Reloaded builder ip list.");
				return TRUE;
				break;
			}
		case RELOAD_TELEPORT_DATA_ID:
			{
				g_TeleportBypass.Init();
				return TRUE;
				break;
			}
		case RELOAD_VOTE_SYSTEM:
			{
				g_VoteSystem.Init();
				return TRUE;
				break;
			}
		case RELOAD_AUTO_ANNOUNCE:
			{
				g_AutoAnnounce.SetReload();
				return TRUE;
				break;
			}
		case RELOAD_OBSCENE_FILTER:
			{
				g_ObsceneFilter.RequestReload();
				return TRUE;
				break;
			}
		case RELOAD_VISUAL_ARMOR:
			{
				g_VisualArmor.RequestReload();
				return TRUE;
			}
		case RELOAD_ALLOWED_HWID:
			{
				g_CliExt.ReloadAllowedHWID();
				return TRUE;
			}
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}