#include "StdAfx.h"
#include "AIO.h"
#include "SchemeBuffer.h"
#include "TeleportBypass.h"
#include "ClanSys.h"
#include "ClassManager.h"
#include "TvT.h"
#include "Augmentation.h"
#include "OfflineShop.h"
#include "AccountDB.h"
#include "DB.h"
#include "FractionSystem.h"
#include "HTMLCacheManager.h"
#include "Auction.h"
#include "PlayerAction.h"
#include "VisualArmor.h"
#include "Wedding.h"
#include "RaidBossStatus.h"
#include "PKPvPStatus.h"
#include "SubStack.h"
#include "VIPSystem.h"
#include "SkillMaster.h"
#include "Rebirth.h"
#include "ClanPvPStatus.h"

bool CBypass::Handled(User *pUser, wstring wBypass)
{
	if(pUser->IsNowTrade())
	{
		pUser->TradeCancel();
		return true;
	}

	if(pUser->pSD->nStoreMode > 0 && wBypass != L"offline_shop")
	{
		pUser->SendSystemMessage(L"You cannot perform this action while operating private store!");
		return true;
	}

	if(pUser->olympiadUser.GetState() != 0)
	{
		if(wBypass.find(L"teleport_") == 0 || wBypass.find(L"player_teleport") == 0)
		{
			pUser->SendSystemMessage(L"You cannot perform this action while participing in Olympiad!");
			return true;
		}
	}

	if(/*g_HTMLCacheManager.IsValid(pUser->wLastHtmlFileName, wBypass.c_str(), 0)*/ true)
	{
		if(pUser->talkTargetId && CCreature::IsValidServerId(&pUser->talkTargetId))
		{
			try
			{
				CCreature *pTarget = CCreature::GetTalkTarget(&pUser->talkTargetId);
				if(pTarget->ValidCreature() && pUser->CheckDistance(pTarget, 250))
				{
					if( wBypass == L"acquire_clan_skill" )
					{
						ClanSys::RequestShowPledgeSkillList(pUser);
						return true;
					}else if( wBypass == L"augment_item" )
					{
						g_Augmentation.ShowAugmentMixWindow(pUser->GetSocket());
						return true;
					}else if( wBypass == L"remove_augment" )
					{
						g_Augmentation.ShowAugmentRemoveWindow(pUser->GetSocket());
						return true;
					}else if( wBypass.find(L"create_academy?") == 0 )
					{
						wstring wPledgeName = CParser::GetBypassValue(wBypass, L"academy_name=");
						wstring wMasterName;
						ClanSys::CreateSubUnit(pUser, CPledge::ACADEMY, wPledgeName, wMasterName);
						return true;
					}else if( wBypass.find(L"create_subpledge?") == 0 )
					{
						//bypass -h create_subpledge?pledge_name= $pledge_name & master_name= $master_name & pledge_type= 100
						wstring wPledgeName = CParser::GetBypassValue(wBypass, L"pledge_name=");
						wstring wMasterName = CParser::GetBypassValue(wBypass, L"master_name=");
						wstring wPledgeType = CParser::GetBypassValue(wBypass, L"pledge_type=");
						int nPledgeType = _wtoi(wPledgeType.c_str());
						ClanSys::CreateSubUnit(pUser, nPledgeType, wPledgeName, wMasterName);
						return true;
					}else if( wBypass.find(L"rename_subpledge?") == 0 )
					{
						//bypass -h rename_subpledge?pledge_type= 100 & pledge_name= $pledge_name
						wstring wPledgeName = CParser::GetBypassValue(wBypass, L"pledge_name=");
						wstring wPledgeType = CParser::GetBypassValue(wBypass, L"pledge_type=");
						int nPledgeType = _wtoi(wPledgeType.c_str());
						ClanSys::RequestRenameSubPledge(pUser, nPledgeType, wPledgeName);
						return true;
					}else if( wBypass.find(L"transfer_master?") == 0 )
					{
						//bypass -h transfer_master?master_name= $master_name
						wstring wMasterName = CParser::GetBypassValue(wBypass, L"master_name=");
						ClanSys::RequestDelegateClanMaster(pUser, wMasterName);
						return true;
					}else if( wBypass.find(L"transfer_master_instant?") == 0 )
					{
						//bypass -h transfer_master_instant?master_name= $master_name
						wstring wMasterName = CParser::GetBypassValue(wBypass, L"master_name=");
						ClanSys::RequestDelegateClanMasterEx(pUser, wMasterName);
						return true;
					}else if( wBypass == L"transfer_master_cancel" )
					{
						//bypass -h transfer_master_cancel
						wstring wMasterName = CParser::GetBypassValue(wBypass, L"master_name=");
						ClanSys::RequestCancelDelegateClanMaster(pUser);
						return true;
					}else if( wBypass.find(L"update_subpledge_master?") == 0 )
					{
						//bypass -h update_subpledge_master?pledge_type=200 & master_name=$master_name2
						wstring wPledgeType = CParser::GetBypassValue(wBypass, L"pledge_type=");
						wstring wMasterName = CParser::GetBypassValue(wBypass, L"master_name=");
						int nPledgeType = _wtoi(wPledgeType.c_str());
						ClanSys::RequestSetSubUnitCaptain(pUser, nPledgeType, wMasterName);
						return true;
					}else if( wBypass == L"pledge_levelup" )
					{
						ClanSys::RequestLevelUp(pUser);
						return true;
					}else if(wBypass.find(L"setforreputation?") == 0)
					{
						wstring wType = CParser::GetBypassValue(wBypass, L"type=");
						int nParam1 = _wtoi(wType.c_str());
						ClanSys::RequestExchangeReputationForSetItems(pUser, nParam1);
						return true;
					}else if( wBypass.find(L"sb_b_p?") == 0)
					{
						UINT buffId = Parser::GetBypassValueInt(wBypass.c_str(), L"id");
						g_SchemeBuffer.RequestBuff(pUser, buffId, false, false);
						return true;
					}else if( wBypass.find(L"sb_b_s?") == 0)
					{
						UINT buffId = Parser::GetBypassValueInt(wBypass.c_str(), L"id");
						g_SchemeBuffer.RequestBuff(pUser, buffId, true, false);
						return true;
					}else if( wBypass.find(L"sb_a_p?") == 0)
					{
						UINT buffId = Parser::GetBypassValueInt(wBypass.c_str(), L"id");
						g_SchemeBuffer.RequestAddToList(pUser, buffId, false, false);
						return true;
					}else if( wBypass.find(L"sb_a_s?") == 0)
					{
						UINT buffId = Parser::GetBypassValueInt(wBypass.c_str(), L"id");
						g_SchemeBuffer.RequestAddToList(pUser, buffId, true, false);
						return true;
					}else if( wBypass.find(L"sb_u_p") == 0)
					{
						g_SchemeBuffer.RequestUseList(pUser, false, false);
						return true;
					}else if( wBypass.find(L"sb_u_s") == 0)
					{
						g_SchemeBuffer.RequestUseList(pUser, true, false);
						return true;
					}
					else if( wBypass.find(L"sb_u_mp") == 0)
					{
						wstring name = CParser::GetBypassValue(wBypass, L"name");

						g_SchemeBuffer.UseMacro(pUser, name, false, false);
						return true;
					}else if( wBypass.find(L"sb_u_ms") == 0)
					{
						wstring name = CParser::GetBypassValue(wBypass, L"name");

						g_SchemeBuffer.UseMacro(pUser, name, true, false);
						return true;
					}else if( wBypass.find(L"sb_c_p") == 0)
					{
						g_SchemeBuffer.RequestClearList(pUser, false, false);
						return true;
					}else if( wBypass.find(L"sb_c_s") == 0)
					{
						g_SchemeBuffer.RequestClearList(pUser, true, false);
						return true;
					}else if( wBypass.find(L"sb_r_p") == 0)
					{
						g_SchemeBuffer.RequestRecovery(pUser, false);
						return true;
					}else if( wBypass.find(L"sb_r_s") == 0)
					{
						g_SchemeBuffer.RequestRecovery(pUser, true);
						return true;
					}else if( wBypass.find(L"sb_d_p") == 0)
					{
						g_SchemeBuffer.RequestDispelAll(pUser, false);
						return true;
					}else if( wBypass.find(L"sb_d_s") == 0)
					{
						g_SchemeBuffer.RequestDispelAll(pUser, true);
						return true;
					}else if( wBypass.find(L"sm_show?") == 0)
					{
						UINT listId = Parser::GetBypassValueInt(wBypass.c_str(), L"listId", 0);
						if(listId > 0)
						{
							g_SkillMaster.ShowSkillList(pUser, listId);
						}
						return true;
					}else if(wBypass.find(L"siege_stat?") == 0)
					{
						int nCastle = _wtoi(CParser::GetBypassValue(wBypass, L"CastleType=").c_str());
						CSiegeStat::CastleType Castle = CSiegeStat::GetInstance().GetCastle(nCastle);
						int nReportType = _wtoi(CParser::GetBypassValue(wBypass, L"ReportType=").c_str());
						CCacheData::DataType ReportType = CCacheData::GetType(nReportType);
						int nPledgeID = _wtoi(CParser::GetBypassValue(wBypass, L"PledgeID=").c_str()); // optional
						if(Castle != CSiegeStat::NotCastle)
						{
							if( ReportType != CCacheData::None )
							{
								if(nPledgeID == 0 && ReportType == CCacheData::PledgeReport)
								{
									nPledgeID = pUser->pSD->nPledgeID;
								}
								CSiegeStat::GetInstance().RequestSendReport(pUser->GetSocket(), ReportType, Castle, nPledgeID);
								return true;
							}
						}		
					}else if( wBypass.find(L"player_level_down?") == 0)
					{
						int nLevel = _wtoi(CParser::GetBypassValue(wBypass, L"level=").c_str());
						if(nLevel > 0 && nLevel < pUser->pSD->nLevel)
						{
							pUser->SetLevel(nLevel);
							pUser->ValidateParameters();
						}
						return true;
					}else if( wBypass.find(L"player_set_level?") == 0)
					{
						int nLevel = _wtoi(CParser::GetBypassValue(wBypass, L"level=").c_str());
						if(nLevel > 0 && nLevel < 81)
						{
							pUser->SetLevel(nLevel);
							pUser->ValidateParameters();
						}
						return true;
					}else if( wBypass.find(L"player_teleport?") == 0)
					{
						wstring wName = CParser::GetBypassValue(wBypass, L"name=");
						g_TeleportBypass.RequestTeleport(pUser, wName);
						return true;
					}else if ( wBypass.find(L"tvt_register?") == 0)
					{
						int team = CParser::GetBypassValueInt(wBypass, L"team=");
						g_TvT.Register(pUser, team);
						return true;
					}else if ( wBypass.find(L"tvt_unregister") == 0)
					{
						g_TvT.Unregister(pUser);
						return true;
					}else if( wBypass == L"get_married" )
					{
						g_Wedding.Marry(pUser);
						return true;
					}else if( wBypass == L"rebirth_show_npc" )
					{
						g_RebirthSystem.ShowMenu(pUser);
						return true;
					}
				}
			}catch(...)
			{
				g_Log.Add(CLog::Error, "[%s] Exception Detected - 1! [%S]", __FUNCTION__, wBypass.c_str());
			}
		}



		//Bypasses without valid target
		if( wBypass.find(L"sb_r_b_p?") == 0)
		{
			UINT buffId = Parser::GetBypassValueInt(wBypass.c_str(), L"id");
			g_SchemeBuffer.RequestBuff(pUser, buffId, false, true);
			return true;
		}else if( wBypass.find(L"sb_r_b_s?") == 0)
		{
			UINT buffId = Parser::GetBypassValueInt(wBypass.c_str(), L"id");
			g_SchemeBuffer.RequestBuff(pUser, buffId, true, true);
			return true;
		}else if( wBypass.find(L"sb_r_a_p?") == 0)
		{
			UINT buffId = Parser::GetBypassValueInt(wBypass.c_str(), L"id");
			g_SchemeBuffer.RequestAddToList(pUser, buffId, false, true);
			return true;
		}else if( wBypass.find(L"sb_r_a_s?") == 0)
		{
			UINT buffId = Parser::GetBypassValueInt(wBypass.c_str(), L"id");
			g_SchemeBuffer.RequestAddToList(pUser, buffId, true, true);
			return true;
		}else if( wBypass.find(L"sb_r_u_p") == 0)
		{
			g_SchemeBuffer.RequestUseList(pUser, false, true);
			return true;
		}else if( wBypass.find(L"sb_r_u_s") == 0)
		{
			g_SchemeBuffer.RequestUseList(pUser, true, true);
			return true;
		}else if( wBypass.find(L"sb_r_c_p") == 0)
		{
			g_SchemeBuffer.RequestClearList(pUser, false, true);
			return true;
		}else if( wBypass.find(L"sb_r_c_s") == 0)
		{
			g_SchemeBuffer.RequestClearList(pUser, true, true);
			return true;
		}else if( wBypass.find(L"sb_r_r_p") == 0)
		{
			g_SchemeBuffer.RequestRecovery(pUser, false);
			return true;
		}else if( wBypass.find(L"sb_r_r_s") == 0)
		{
			g_SchemeBuffer.RequestRecovery(pUser, true);
			return true;
		}else if( wBypass.find(L"sb_r_d_p") == 0)
		{
			g_SchemeBuffer.RequestDispelAll(pUser, false);
			return true;
		}else if( wBypass.find(L"sb_r_d_s") == 0)
		{
			g_SchemeBuffer.RequestDispelAll(pUser, true);
			return true;
		}else if( wBypass.find(L"sb_r_u_mp") == 0)
		{
			wstring name = CParser::GetBypassValue(wBypass, L"name");

			g_SchemeBuffer.UseMacro(pUser, name, false, true);
			return true;
		}else if( wBypass.find(L"sb_r_u_ms") == 0)
		{
			wstring name = CParser::GetBypassValue(wBypass, L"name");

			g_SchemeBuffer.UseMacro(pUser, name, true, true);
			return true;
		}else if( wBypass.find(L"become_aio?") == 0)
		{
			UINT id = Parser::GetBypassValueInt(wBypass.c_str(), L"id", 0);
			g_AIOSystem.BecomeAIO(pUser, id);
			return true;
		}else if( wBypass.find(L"remove_aio") == 0)
		{
			g_AIOSystem.RemoveAIO(pUser);
			return true;
		}else if( wBypass.find(L"tvt_register?") == 0)
		{
			int team = CParser::GetBypassValueInt(wBypass, L"team=");
			g_TvT.Register(pUser, team);
			return true;
		}else if ( wBypass.find(L"tvt_unregister") == 0)
		{
			g_TvT.Unregister(pUser);
			return true;
		}else if ( wBypass.find(L"cm_nobless") == 0)
		{
			g_ClassManager.RequestNobless(pUser);
			return true;
		}else if ( wBypass.find(L"cm_subclass") == 0)
		{
			g_ClassManager.RequestSubClass(pUser);
			return true;
		}else if ( wBypass.find(L"cm_show_page") == 0)
		{
			g_ClassManager.RequestShowHtml(pUser);
			return true;
		}else if ( wBypass.find(L"cm_change?") == 0)
		{
			int newClass = CParser::GetBypassValueInt(wBypass, L"class");
			g_ClassManager.RequestChange(pUser, newClass);
			return true;
		}else if ( wBypass == L"substack_show")
		{
			g_SubStack.ShowAvailable(pUser);
			return true;
		}else if ( wBypass == L"substack_show_ex")
		{
			g_SubStack.ShowAvailable(pUser, true);
			return true;
		}else if ( wBypass.find(L"substack_add?") == 0)
		{
			UINT newClass = CParser::GetBypassValueInt(wBypass, L"class");
			g_SubStack.RequestStack(pUser, newClass);
			return true;
		}else if ( wBypass.find(L"substack_add_ex?") == 0)
		{
			UINT newClass = CParser::GetBypassValueInt(wBypass, L"class");
			g_SubStack.RequestStack(pUser, newClass, true);
			return true;
		}else if ( wBypass == L"substack_switch")
		{
			g_SubStack.RequestSwitch(pUser);
			return true;
		}else if ( wBypass == L"substack_remove")
		{
			g_SubStack.RequestRemove(pUser);
			return true;
		}else if ( wBypass.find(L"rune_info") == 0)
		{
			pUser->SendRuneInfo();
			return true;
		}else if ( wBypass.find(L"item_time_left") == 0)
		{
			pUser->SendLifeTimeItemsInfo();
			return true;
		}else if ( wBypass.find(L"block_all") == 0)
		{
			if(!pUser->blockAllChat)
			{
				pUser->blockAllChat = true;
				pUser->SendETCBuffStatus();
				pUser->pSocket->SendSystemMessage(961);
			}
			return true;
		}else if ( wBypass.find(L"unblock_all") == 0)
		{
			if(pUser->blockAllChat)
			{
				pUser->blockAllChat = false;
				pUser->SendETCBuffStatus();
				pUser->pSocket->SendSystemMessage(962);
			}
			return true;
		}else if ( wBypass.find(L"offline_shop") == 0)
		{
			g_OfflineShop.RequestActivateOfflineShop(pUser);
			return true;
		}else if ( wBypass.find(L"buy_offline_shop") == 0)
		{
			g_OfflineShop.RequestBuyRequiredItem(pUser);
			return true;
		}else if ( wBypass.find(L"exp_mode") == 0)
		{
			BOOL enabled = CParser::GetBypassValueInt(wBypass, L"state=");
			if(enabled)
			{
				if(!pUser->pSD->pExData->stopIncExp)
				{
					pUser->pSD->pExData->stopIncExp = enabled;
					pUser->pSocket->SendSystemMessage(L"Stop Exp mode have been enabled.");
				}
			}else
			{
				if(pUser->pSD->pExData->stopIncExp)
				{
					pUser->pSD->pExData->stopIncExp = enabled;
					pUser->pSocket->SendSystemMessage(L"Stop Exp mode have been disabled.");
				}
			}
			return true;
		}else if ( wBypass.find(L"auto_loot") == 0)
		{
			BOOL enabled = CParser::GetBypassValueInt(wBypass, L"state=");
			if(enabled)
			{
				if(!pUser->pED->autoLoot)
				{
					pUser->pED->autoLoot = enabled;
					pUser->pSocket->SendSystemMessage(L"Auto Loot mode have been enabled.");
				}
			}else
			{
				if(pUser->pED->autoLoot)
				{
					pUser->pED->autoLoot = enabled;
					pUser->pSocket->SendSystemMessage(L"Auto Loot mode have been disabled.");
				}
			}
			return true;
		}else if (wBypass.find(L"block_item?") == 0)
		{
			int itemId = Parser::GetBypassValueInt(wBypass.c_str(), L"id");
			int userId = Parser::GetBypassValueInt(wBypass.c_str(), L"userId");
			if(itemId && userId && (pUser->pSD->nBuilder == 1 || pUser->nDBID == userId))
			{
				User *pTarget = g_UserDB.GetUserByDBID(userId);
				if(pTarget->ValidUser())
				{
					if(CItem *pItem = pTarget->inventory.GetItemByDBID(itemId))
					{
						UINT nTimeout = pItem->nProtectionTimeout;
						UINT currentTime = time(0);
						if(nTimeout > currentTime)
						{
							nTimeout = nTimeout - currentTime;
						}else
						{
							nTimeout = 0;
						}
						if(pUser->pSD->nBuilder == 1)
						{
							wstringstream htmlStream;
							htmlStream << L"<html><body><center> <font color=\"LEVEL\" >Nickname: " << pTarget->pSD->wszName << L"</font><br>Item: (" << itemId << L") " << g_ItemDBEx.GetItemName(pItem->pSID->nItemID).c_str() << L"<br>Enchant: " << pItem->pSID->nEnchantLevel << L"<br><br>";
							htmlStream << L"Protection Time: " << nTimeout << L" (s)<br>";
							htmlStream << L"Time in seconds<br>3600 = 1 hour<br>86400 = 1 day<br>604800 = 1 week<br>2592000 = 30 days<br>";
							htmlStream << L"<edit var=\"timeout\" width=120><br>";
							htmlStream << L"<button value=\"Set\" action=\"bypass -h block_item_execute?id=" << itemId << L"&userId=" << userId << L"&timeout = $timeout\" width=40 height=15 back=\"sek.cbui94\" fore=\"sek.cbui92\"><br><br>";
							htmlStream << L"<button value=\"Back\" action=\"bypass -h block_item_list?userId=" << userId << L"\" width=40 height=15 back=\"sek.cbui94\" fore=\"sek.cbui92\"><br><br></center></body></html>";
							PlayerAction::ShowHTML(pUser, L"block_item_execute.htm", htmlStream.str().c_str(), 0);
						}else
						{
							wstringstream htmlStream;
							htmlStream << L"<html><body><center> <font color=\"LEVEL\" >Nickname: " << pTarget->pSD->wszName << L"</font><br>Item: (" << itemId << L") " << g_ItemDBEx.GetItemName(pItem->pSID->nItemID).c_str() << L"<br>Enchant: " << pItem->pSID->nEnchantLevel << L"<br><br>";
							htmlStream << L"Protection Time: " << nTimeout << L" (s)<br>";
							htmlStream << L"Press <font color=\"LEVEL\">Block</font> button to block item for 1 week.<br>";
							htmlStream << L"<button value=\"Block\" action=\"bypass -h block_item_execute?id=" << itemId << L"&userId=" << userId << L"&timeout=604800 \" width=40 height=15 back=\"sek.cbui94\" fore=\"sek.cbui92\"><br><br><br>";
							htmlStream << L"<button value=\"Back\" action=\"bypass -h block_item_list?userId=" << userId << L"\" width=40 height=15 back=\"sek.cbui94\" fore=\"sek.cbui92\"><br><br></center></body></html>";
							PlayerAction::ShowHTML(pUser, L"block_item_execute.htm", htmlStream.str().c_str(), 0);
						}
					}else
					{
						pUser->SendSystemMessage(L"Invalid item!");
					}
				}else
				{
					pUser->SendSystemMessage(L"Invalid user!");
				}
			}
			return true;
		}else if (wBypass.find(L"block_item_execute?") == 0)
		{
			int itemId = Parser::GetBypassValueInt(wBypass.c_str(), L"id");
			UINT userId = Parser::GetBypassValueInt(wBypass.c_str(), L"userId");
			UINT timeout = Parser::GetBypassValueInt(wBypass.c_str(), L"timeout");
			if(itemId && userId && (pUser->pSD->nBuilder == 1 || pUser->nDBID == userId))
			{
				User *pTarget = g_UserDB.GetUserByDBID(userId);
				if(pTarget->ValidUser())
				{
					if(CItem *pItem = pTarget->inventory.GetItemByDBID(itemId))
					{
						time_t left = pItem->nProtectionTimeout - time(0);

						if(pUser->pSD->nBuilder != 1)
						{
							if(timeout != 604800)
							{
								timeout = 604800;
							}
						}

						if(left < timeout || pUser->pSD->nBuilder == 1)
						{
							if(timeout)
							{
								pItem->nProtectionTimeout = timeout + time(0);
								g_DB.RequestSaveItemDataEx(userId, pItem);
							}else
							{
								pItem->nProtectionTimeout = 0;
								g_DB.RequestSaveItemDataEx(userId, pItem);
							}
							pUser->SendSystemMessage(L"Setting new Item Protection succeed.");
						}else
						{
							pUser->SendSystemMessage(L"Only GM is able to set shorter protection time!");
						}
						if(pUser->pSD->nBuilder == 1)
						{
							pTarget->ShowBlockableItems(pUser);
						}else
						{
							pUser->ShowBlockableItems();
						}
					}else
					{
						pUser->SendSystemMessage(L"Invalid item!");
					}
				}else
				{
					pUser->SendSystemMessage(L"Invalid user!");
				}
			}
			return true;
		}else if(wBypass.find(L"block_item_list?") == 0)
		{
			UINT userId = Parser::GetBypassValueInt(wBypass.c_str(), L"userId");
			if(pUser->pSD->nBuilder == 1 || pUser->nDBID == userId)
			{
				User *pTarget = g_UserDB.GetUserByDBID(userId);
				if(pTarget->ValidUser())
				{
					if(pUser->pSD->nBuilder == 1)
					{
						pTarget->ShowBlockableItems(pUser);
					}else
					{
						pTarget->ShowBlockableItems();
					}
				}else
				{
					pUser->ShowBlockableItems();
				}
			}else if( userId == 0)
			{
				pUser->ShowBlockableItems();
			}
			return true;
		}else if(wBypass.find(L"lock_account") == 0)
		{
			if(g_AccountDB.LockHwid(pUser->pSocket->accountId, pUser->pSocket->pED->longHWID.hash, pUser->pSD->wszName))
			{
				pUser->SendSystemMessage(L"Your account have been locked.");
			}else
			{
				pUser->SendSystemMessage(L"Your account is already locked.");
			}
			return true;
		}else if(wBypass.find(L"unlock_account") == 0)
		{
			if(g_AccountDB.UnlockHwid(pUser->pSocket->accountId, pUser->pSD->wszName))
			{
				pUser->SendSystemMessage(L"Your account have been unlocked.");
			}else
			{
				pUser->SendSystemMessage(L"Your account is already unlocked.");
			}
			return true;
		}else if(wBypass.find(L"lock_char") == 0)
		{
			if(g_AccountDB.LockHwidChar(pUser->nDBID, pUser->pSocket->pED->longHWID.hash, pUser->pSD->wszName))
			{
				pUser->SendSystemMessage(L"Your character have been locked.");
			}else
			{
				pUser->SendSystemMessage(L"Your character is already locked.");
			}
			return true;
		}else if(wBypass.find(L"unlock_char") == 0)
		{
			if(g_AccountDB.UnlockHwidChar(pUser->nDBID, pUser->pSD->wszName))
			{
				pUser->SendSystemMessage(L"Your character have been unlocked.");
			}else
			{
				pUser->SendSystemMessage(L"Your character is already unlocked.");
			}
			return true;
		}else if (wBypass.find(L"set_additional_box?") == 0)
		{
			UINT timeParam = Parser::GetBypassValueInt(wBypass.c_str(), L"time");
			if(timeParam > 0)
			{
				g_AccountDB.SetAdditionalBox(pUser->pSocket->accountId, (timeParam + time(0)));
				pUser->SendSystemMessage(L"Your additional box have been enabled.");
			}else
			{
				g_AccountDB.SetAdditionalBox(pUser->pSocket->accountId, 0);
				pUser->SendSystemMessage(L"Your additional box have been disabled.");
			}

			return true;
		}else if(wBypass.find(L"fraction_signup?") == 0)
		{
			UINT fractionId = Parser::GetBypassValueInt(wBypass.c_str(), L"id");
			if(fractionId > 0)
			{
				g_FractionSystem.Register(pUser, fractionId);
			}
			return true;
		}else if(wBypass.find(L"fraction_leave") == 0)
		{
			g_FractionSystem.Unregister(pUser);
			return true;
		}else if(wBypass.find(L"_auction_show") == 0)
		{
			wstring category = CParser::GetBypassValue(wBypass, L"category");
			UINT page = Parser::GetBypassValueInt(wBypass.c_str(), L"page", 0);
			g_Auction.RequestShowAuction(pUser, category.c_str(), page);
			return true;
		}else if(wBypass.find(L"_auction_create") == 0)
		{
			g_Auction.RequestSelectAuctionItem(pUser);
			return true;
		}else if(wBypass.find(L"_auction_cancel") == 0)
		{
			g_Auction.RequestCancelAuction(pUser);
			return true;
		}else if(wBypass.find(L"_auction_set_item?") == 0)
		{
			//auction_set_item?amount= $item_amount &price= $item_price
			int itemAmount = Parser::GetBypassValueInt(wBypass.c_str(), L"amount");
			int itemPrice = Parser::GetBypassValueInt(wBypass.c_str(), L"price");
			g_Auction.RequestSetAuction(pUser, itemPrice, itemAmount);
			return true;
		}else if(wBypass.find(L"_auction_buy?") == 0)
		{
			UINT auctionId = Parser::GetBypassValueInt(wBypass.c_str(), L"id", 0);
			g_Auction.RequestSetBuyItem(pUser, auctionId);
			return true;
		}else if(wBypass.find(L"_auction_set_buy?") == 0)
		{
			UINT auctionId = Parser::GetBypassValueInt(wBypass.c_str(), L"id", 0);
			INT32 amount = Parser::GetBypassValueInt(wBypass.c_str(), L"amount", 0);
			g_Auction.RequestConfirmBuy(pUser, auctionId, amount);
			return true;
		}else if(wBypass.find(L"_auction_confirm_buy?") == 0)
		{
			UINT auctionId = Parser::GetBypassValueInt(wBypass.c_str(), L"id", 0);
			INT32 amount = Parser::GetBypassValueInt(wBypass.c_str(), L"amount", 0);
			g_Auction.RequestBuy(pUser, auctionId, amount);
			return true;
		}else if(wBypass == L"va_equip_request")
		{
			g_VisualArmor.OnRequestEquip(pUser);
			return true;
		}else if(wBypass.find(L"va_equip?") == 0)
		{
			UINT armorId = Parser::GetBypassValueInt(wBypass.c_str(), L"id");
			g_VisualArmor.OnEquip(pUser, armorId);
			return true;
		}else if(wBypass == L"va_unequip")
		{
			g_VisualArmor.OnUnequip(pUser);
			return true;
		}else if(wBypass.find(L"rb_status?page") == 0)
		{
			UINT page = Parser::GetBypassValueInt(wBypass.c_str(), L"page");
			g_RaidBossStatus.RequestRaidBossStatus(pUser, page);
			return true;
		}else if(wBypass.find(L"pk_status?page") == 0)
		{
			UINT page = Parser::GetBypassValueInt(wBypass.c_str(), L"page");
			g_PKPvPStatus.RequestPKStatus(pUser, page);
			return true;
		}else if(wBypass.find(L"pvp_status?page") == 0)
		{
			UINT page = Parser::GetBypassValueInt(wBypass.c_str(), L"page");
			g_PKPvPStatus.RequestPvPStatus(pUser, page);
			return true;
		}else if(wBypass.find(L"clan_pvp_status?page") == 0)
		{
			UINT page = Parser::GetBypassValueInt(wBypass.c_str(), L"page");
			g_ClanPvPStatus.RequestPvPStatus(pUser, page);
			return true;
		}else if(wBypass.find(L"vip_help") == 0)
		{
			g_VIPSystem.OnHelp(pUser);
			return true;
		}else if(wBypass.find(L"vip_status") == 0)
		{
			g_VIPSystem.OnStatus(pUser);
			return true;
		}else if(wBypass.find(L"vip_bonus") == 0)
		{
			UINT level = Parser::GetBypassValueInt(wBypass.c_str(), L"level");
			if(level > 0)
			{
				g_VIPSystem.SendBonusInfo(pUser, level);
			}
			return true;
		}else if(wBypass.find(L"vip_upgrade") == 0)
		{
			UINT level = Parser::GetBypassValueInt(wBypass.c_str(), L"level");
			if(level > 0)
			{
				g_VIPSystem.RequestChangeLevel(pUser, level);
			}
			return true;
		}else if(wBypass.find(L"open_website?url") == 0)
		{
			wstring url = wBypass.substr(17, wBypass.size() - 17);
			pUser->pSocket->Send("cdS", 0x87, 0x04, url.c_str());

			return true;
		}else if( wBypass == L"rebirth_show" )
		{
			g_RebirthSystem.ShowMenu(pUser);
			return true;
		}else if( wBypass == L"rebirth_do" )
		{
			g_RebirthSystem.OnRebirth(pUser);
			return true;
		}else if( wBypass == L"rebirth_distribute" )
		{
			g_RebirthSystem.OnDistributeStats(pUser);
			return true;
		}else if( wBypass == L"rebirth_inc_str")
		{
			g_RebirthSystem.OnIncreaseStats(pUser, StatStr);
			return true;
		}else if( wBypass == L"rebirth_inc_int")
		{
			g_RebirthSystem.OnIncreaseStats(pUser, StatInt);
			return true;
		}else if( wBypass == L"rebirth_inc_con")
		{
			g_RebirthSystem.OnIncreaseStats(pUser, StatCon);
			return true;
		}else if( wBypass == L"rebirth_inc_men")
		{
			g_RebirthSystem.OnIncreaseStats(pUser, StatMen);
			return true;
		}else if( wBypass == L"rebirth_inc_dex")
		{
			g_RebirthSystem.OnIncreaseStats(pUser, StatDex);
			return true;
		}else if( wBypass == L"rebirth_inc_wit")
		{
			g_RebirthSystem.OnIncreaseStats(pUser, StatWit);
			return true;
		}else if( wBypass.find(L"_sell_pet?") == 0)
		{
			//ex bypass - h _sell_pet?petId=1012781&petLevel=55&collarId=6650&priceId=57&priceAmount=1000000
			INT32 petClassId = Parser::GetBypassValueInt(wBypass.c_str(), L"petId");
			INT32 petLevel = Parser::GetBypassValueInt(wBypass.c_str(), L"petLevel");
			INT32 collarId = Parser::GetBypassValueInt(wBypass.c_str(), L"collarId");
			INT32 priceId = Parser::GetBypassValueInt(wBypass.c_str(), L"priceId");
			INT32 priceAmount = Parser::GetBypassValueInt(wBypass.c_str(), L"priceAmount");

			if(petClassId && collarId)
			{
				//check requirement
				if(priceId > 0 && priceAmount > 0)
				{
					if(pUser->DeleteItemInInventory(priceId, priceAmount))
					{
						typedef void(*f)(User*, INT32, INT32, INT32);
						f(0x74BED0L)(pUser, collarId, petClassId, petLevel);
					}
				}
			}
			return true;
		}else if( wBypass.find(L"bs_buy_item?") == 0)
		{
			if(!pUser->InCombat())
			{
				//bypass -h bs_buy_item?priceId=57&priceAmount=3000&rewardId=7575&rewardAmount=1&rewardEnchant=0
				INT32 priceId = Parser::GetBypassValueInt(wBypass.c_str(), L"priceId");
				INT32 priceAmount = Parser::GetBypassValueInt(wBypass.c_str(), L"priceAmount");
				INT32 rewardId = Parser::GetBypassValueInt(wBypass.c_str(), L"rewardId");
				INT32 rewardAmount = Parser::GetBypassValueInt(wBypass.c_str(), L"rewardAmount");
				INT32 rewardEnchant = Parser::GetBypassValueInt(wBypass.c_str(), L"rewardEnchant");

				if(priceId > 0 && priceAmount > 0 && rewardId > 0 && rewardAmount > 0)
				{
					if(pUser->DeleteItemInInventory(priceId, priceAmount))
					{
						pUser->AddItemToInventory(rewardId, rewardAmount, false, rewardEnchant, 0, 0);
						g_Logger.Add(L"User[%s][%d] bought item[%d][%d] enchant[%d] price[%d][%d]", pUser->pSD->wszName, pUser->nDBID, rewardId, rewardAmount, rewardEnchant, priceId, priceAmount);
					}
				}
			}else
			{
				pUser->SendSystemMessage(L"You cannot buy items in combat mode!");
			}

			return true;
		}
	}
	return false;
}