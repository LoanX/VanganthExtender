#include "StdAfx.h"
#include "PlayerAction.h"
#include "VIPSystem.h"
#include "ClanSys.h"
#include "TerritoryData.h"
#include "TvT.h"
#include "OfflineShop.h"
#include "AIO.h"
#include "BuildCmdHandler.h"
#include "PrivateStore.h"
#include "CursedWeaponSystem.h"
#include "OlympiadSystem.h"
#include "Augmentation.h"
#include "SocketDB.h"
#include "DB.h"
#include "AccountDB.h"
#include "VoteReward.h"
#include "AutoLearn.h"
#include "AutoAnnounce.h"
#include "HTMLCacheManager.h"
#include "DnN.h"
#include "FractionSystem.h"
#include "ItemDelivery.h"
#include "UserCache.h"
#include "HeroSystem.h"
#include "AutoLoot.h"
#include "SchemeBuffer.h"
#include "DeathPenalty.h"
#include "OlympiadField.h"
#include "HtmlFilter.h"
#include "InfinityShot.h"
#include "DuelSystem.h"
#include "HTMLCache.h"
#include "ArmorPenalty.h"
#include "Auction.h"
#include "Wedding.h"
#include "CliExt.h"
#include "ItemAutoConsume.h"
#include "CharacterLock.h"
#include "DailyPvP.h"
#include "SubStack.h"
#include "SpawnProtection.h"
#include "L2NetworkVote.h"
#include "VisualArmor.h"
#include "Rebirth.h"
#include "GradePenalty.h"
#include "RecipeManager.h"
#include "ObjectDB.h"
#include "PvPAnnounce.h"

extern BOOL g_AttackWarWithoutCTRL;
extern BOOL g_ValidateAcquiredSkills;
extern list<int> g_OlympiadForbiddenItemList;
extern vector<UINT> g_OlympiadForbiddenItemGrade;
extern INT32 g_OlympiadMaxAllowedWeaponEnchant;
extern INT32 g_OlympiadMaxAllowedArmorEnchant;
extern map<INT32, DWORD> g_ItemNicknameColor;
extern map<INT32, DWORD> g_ItemTitleColor;
extern OlympiadPeriod g_OlympiadPeriod[7][2];
extern BOOL g_ShowInventoryOnEnterWorld;
extern BOOL g_ValidateStackableItems;
extern BOOL g_DisablePets;
extern BOOL g_ShowEnterWorldHtml;
extern INT32 g_MaxPartyLevelRange;
extern BOOL g_IsDistributingExpSp[32];
extern CCreature* g_DistributingCreature[32];
extern double g_PartyExpRate;
extern BOOL g_CustomAbnormalInfoPacket;
extern BOOL g_OlympiadStopActionOnFightStart;
extern UINT g_SaveAbnormalsIntervalTime;

vector<INT32> g_InfinityArrow;

double g_ExpCurve[85];

void SendSkillReuseMsg(CUserSocket *pSocket, const char* format, BYTE opCode, DWORD msgId, DWORD varCount, DWORD varType, DWORD skillId, DWORD skillLevel)
{
	if(User *pUser = pSocket->GetUser())
	{
		if(pUser->olympiadUser.IsInside())
		{
			DWORD secLeft = pUser->GetSkillReuse(skillId) - GetTickCount();
			secLeft /= 1000;
			if(secLeft > 0)
			{	//There are $s1 second(s) remaining in $s2's re-use time.
				CSystemMessage msg(2415);
				msg.AddNumber(secLeft);
				msg.AddSkill(skillId, skillLevel);
				pUser->SendSystemMessage(&msg);
				return;
			}
		}
	}
	pSocket->Send(format, opCode, msgId, varCount, varType, skillId, skillLevel);
}

void PlayerAction::Initialize()
{
	TCHAR temp[8190] = { 0 };
	if(GetPrivateProfileString(_T("L2Server"), _T("InfinityArrowList"), 0, temp, 8190, g_ConfigFile))
	{
		wstringstream itemStream(temp);
		int itemId = 0;
		while(itemStream >> itemId)
		{
			g_InfinityArrow.push_back(itemId);
		}
	}

	for(UINT n=0;n<85;n++)
	{
		g_ExpCurve[n] = 1.0;
	}

	for(int n=0;n<85;n++)
	{
		wstringstream key;
		key << L"ExpCurveParam_" << n;
		g_ExpCurve[n] = GetPrivateProfileDouble(_T("Features"), key.str().c_str(), 1.0, g_ConfigFile);
	}
	g_HookManager.WriteCall(0x70A821, COlympiadEntry_RemoveEntry, 0);
	g_HookManager.WriteCall(0x70A996, COlympiadEntry_RemoveEntry, 0);

	g_HookManager.WriteCall(0x70EAD9, COlympiadEntry_AddEntry, 0);
	g_HookManager.WriteCall(0x70E909, COlympiadEntry_AddEntry, 0);
	g_HookManager.WriteCall(0x70DE2E, COlympiadEntry_AddEntry, 0);
	g_HookManager.WriteCall(0x70DDB6, COlympiadEntry_AddEntry, 0);
	g_HookManager.WriteCall(0x70DC09, COlympiadEntry_AddEntry, 0);
	g_HookManager.WriteCall(0x70DB8F, COlympiadEntry_AddEntry, 0);
	g_HookManager.WriteCall(0x70D9D4, COlympiadEntry_AddEntry, 0);
	g_HookManager.WriteCall(0x70D95F, COlympiadEntry_AddEntry, 0);


	g_HookManager.WriteCall( 0x57602E, OnEnterWorld, 0);
	g_HookManager.WriteCall( 0x850006, OnLeaveWorld, 0);
	g_HookManager.WriteMemoryDWORD(0xA87760, (DWORD)OnDie);
	g_HookManager.WriteCall( 0x581B1E, OnPickUpItem, 0);
	g_HookManager.WriteCall( 0x553DA8, OnAddItemToInventory, 0);
	g_HookManager.WriteCall( 0x553587, OnAddItemToInventory, 0);
	g_HookManager.WriteCall( 0x553860, OnAddItemToInventory, 0);
	g_HookManager.WriteMemoryDWORD(0xA876D0, (DWORD)UseItem);
	g_HookManager.WriteMemoryDWORD(0xA876C8, (DWORD)UnequipItem);
	g_HookManager.WriteCall( 0x70D185, RegisterInOlympiad, 0);
	
	g_HookManager.WriteCall( 0x53FEAF, OnChangeSubjobEnd, 0);
	g_HookManager.WriteCall( 0x56FE7A, OnChangeSubjobEnd, 0);
	g_HookManager.WriteCall( 0x5715EE, OnChangeSubjobEnd, 0);

	g_HookManager.WriteMemoryDWORD(0xA876E0, (DWORD)OnDropItem);
	g_HookManager.WriteCall( 0x513BDE, OnGetRelationTo, 0);
	g_HookManager.WriteCall( 0x4D8959, OnGetRelationTo, 0);
	g_HookManager.WriteCall( 0x800C71, OnGetRelationTo, 0);
	g_HookManager.WriteCall( 0x80FFC5, OnGetRelationTo, 0);
	g_HookManager.WriteCall( 0x8102D5, OnGetRelationTo, 0);
	g_HookManager.WriteCall( 0x8107ED, OnGetRelationTo, 0);
	g_HookManager.WriteCall( 0x853000, OnGetRelationTo, 0);
	g_HookManager.WriteCall( 0x852C72, OnGetRelationTo, 0);
	g_HookManager.WriteCall( 0x8536F1, OnGetRelationTo, 0);
	g_HookManager.WriteCall( 0x705F2C, OnOlympiadEscaping, 0);
	g_HookManager.WriteCall( 0x703B14, OnOlympiadFinishMatch, 0);
	g_HookManager.WriteCall( 0x7000B9, OnEnteringOlympiadStadion, 0);
	g_HookManager.WriteCall( 0x8465D3, OnUpdateHP, 0);
	g_HookManager.WriteCall( 0x8000B2, OnSendETCBuffStatusInfo, 0);
	g_HookManager.WriteCall( 0x7032AC, OnOlympiadStartMatch, 0);
	g_HookManager.WriteMemoryDWORD(0xA87758, (DWORD)OnTeleportToLocation);
	g_HookManager.WriteMemoryDWORD(0xA876B8, (DWORD)OnEquipItem);
	g_HookManager.WriteCall( 0x81E607, OnDieLooseExp, 0);
	g_HookManager.WriteCall( 0x81E667, OnDieLooseExp, 0);
	g_HookManager.WriteCall( 0x81E6D3, OnDieLooseExp, 0);
	g_HookManager.WriteCall( 0x81E88E, OnDieDropItem, 0);
	g_HookManager.WriteCall( 0x512844, IncreaseKarma, 0);
	g_HookManager.WriteMemoryDWORD(0xA87230, (DWORD)OnTimerExpired);
	g_HookManager.WriteCall(0x53A900, SaveCharacterInfo, 1);
	g_HookManager.WriteCall(0x572095, LoadItemsPacket, 0);

	g_HookManager.WriteCall(0x436CCA, ObserverStart, 0);
	g_HookManager.WriteCall(0x44A7BF, ObserverStart, 0);
	g_HookManager.WriteCall(0x895A32, ObserverStart, 0);

	g_HookManager.WriteCall(0x449D48, ObserverEnd, 0);
	g_HookManager.WriteCall(0x869361, ObserverEnd, 0);
	g_HookManager.WriteCall(0x845ED3, ObserverEnd, 0);
	g_HookManager.WriteCall(0x6FF350, ObserverEnd, 0);
	g_HookManager.WriteCall(0x6FF3BB, ObserverEnd, 0);

	g_HookManager.WriteCall(0x467C61, OnChangeSubjobBegin, 0);
	g_HookManager.WriteCall(0x467E98, OnChangeSubjobBegin, 0);
	g_HookManager.WriteCall(0x467F8F, OnChangeSubjobBegin, 0);
	g_HookManager.WriteCall(0x6E3912, OnChangeSubjobBegin, 0);
	g_HookManager.WriteCall(0x6E3410, OnChangeSubjobBegin, 0);
	g_HookManager.WriteCall(0x6E3D3F, OnChangeSubjobBegin, 0);

	g_HookManager.WriteCall(0x6F743F, OnSendXPnSP, 0);
	g_HookManager.WriteCall(0x6F750D, OnSendXPnSP, 0);
	g_HookManager.WriteCall(0x6F8F3C, OnSendXPnSP, 0);

	g_HookManager.WriteMemoryDWORD(0xA874C8, (DWORD)PlayerAction::OnClassChange);

	g_HookManager.WriteMemoryDWORD(0xA878D8, (DWORD)PlayerAction::OnHear);

	g_HookManager.WriteCall(0x6B3487, SendSkillReuseMsg, 0);
	g_HookManager.WriteCall(0x6B56F5, SendSkillReuseMsg, 0);

	g_HookManager.WriteMemoryDWORD(0xA87730, (DWORD)PlayerAction::OnDeleteItemInInventoryBeforeCommit, 0);

	g_HookManager.WriteCall(0x8A2E58, PlayerAction::DeleteShipTicket, 4);

	g_HookManager.WriteCall(0x80F48A, PlayerAction::TradeCancelled, 0);	//in User::TradeCancel
	g_HookManager.WriteCall(0x7CDB4B, PlayerAction::SummonPet, 0);
	if(g_CustomAbnormalInfoPacket)
	{
		g_HookManager.WriteMemoryDWORD(0xA87900, (DWORD)PlayerAction::SendAbnormalStatusInfo);
	}

	g_HookManager.WriteCall(0x411D0F, PlayerAction::ShowHTML);
	g_HookManager.WriteCall(0x437A10, PlayerAction::ShowHTML);
	g_HookManager.WriteCall(0x43800A, PlayerAction::ShowHTML);
	g_HookManager.WriteCall(0x4FB072, PlayerAction::ShowHTML);
	g_HookManager.WriteCall(0x550E31, PlayerAction::ShowHTML);
	g_HookManager.WriteCall(0x551AC8, PlayerAction::ShowHTML);
	g_HookManager.WriteCall(0x5F2C2B, PlayerAction::ShowHTML);
	g_HookManager.WriteCall(0x6D9BEF, PlayerAction::ShowHTML);
	g_HookManager.WriteCall(0x6E589A, PlayerAction::ShowHTML);
	g_HookManager.WriteCall(0x6ED0D7, PlayerAction::ShowHTML);
	g_HookManager.WriteCall(0x6ED6CE, PlayerAction::ShowHTML);
	g_HookManager.WriteCall(0x710098, PlayerAction::ShowHTML);
	g_HookManager.WriteCall(0x711D95, PlayerAction::ShowHTML);
	g_HookManager.WriteCall(0x7705DD, PlayerAction::ShowHTML);
	g_HookManager.WriteCall(0x797140, PlayerAction::ShowHTML);
	g_HookManager.WriteCall(0x797249, PlayerAction::ShowHTML);
	g_HookManager.WriteCall(0x797514, PlayerAction::ShowHTML);
	g_HookManager.WriteCall(0x894538, PlayerAction::ShowHTML);
	g_HookManager.WriteCall(0x8947B1, PlayerAction::ShowHTML);

	g_HookManager.WriteCall(0x60DFBC, PlayerAction::NpcHtmlSender);

	g_HookManager.WriteCall(0x6DD674, PlayerAction::ShowTutorialPage);	//ShowTutorialPage
	g_HookManager.WriteCall(0x8718C4, PlayerAction::ShowTutorialPage);	//TutorialLinkHTML
	g_HookManager.WriteCall(0x464D1E, PlayerAction::ShowTutorialPage);	//BuilderCmd_show_tutorial_html

	g_HookManager.WriteCall(0x825356, PlayerAction::OnRestartSendSaveCharacterInfo);
	g_HookManager.WriteCall(0x7137C6, PlayerAction::ShowCommunityBoard);
	g_HookManager.WriteCall(0x7138EF, PlayerAction::ShowCommunityBoard);
	g_HookManager.WriteCall(0x713A10, PlayerAction::ShowCommunityBoard);
	g_HookManager.WriteCall(0x713B26, PlayerAction::ShowCommunityBoard);

	g_HookManager.WriteCall(0x76A5BF, PlayerAction::OnTakeWithdrawPenalty);
	g_HookManager.NopMemory(0x846197, 8);

	g_HookManager.WriteCall(0x5128DF, PlayerAction::DBIncreaseUserDuelCount);
	g_HookManager.WriteMemoryDWORD(0xA8EEF0, (DWORD)PlayerAction::CUserSocket_TimerExpired);

	g_HookManager.WriteMemoryDWORD(0xA877C8, (DWORD)PlayerAction::OnAction);

	g_HookManager.WriteMemoryDWORD(0xA87690, (DWORD)PlayerAction::OnBroadcastCombatModeStart);

	g_HookManager.WriteCall(0x467FB6, PlayerAction::OnDBRequestRenewSubjob);
	g_HookManager.WriteCall(0x6E3DB4, PlayerAction::OnDBRequestRenewSubjob);

	g_HookManager.WriteCall(0x7FDCF4, PlayerAction::OnValidateCpRegen);

	g_HookManager.WriteCall(0x866573, PlayerAction::OnSendSkillAcquireList);

	g_HookManager.WriteCall(0x773FFC, PlayerAction::OnRecipeMakeItem, 0);
}

void PlayerAction::OnRecipeMakeItem(PVOID instancePtr, User *pUser, UINT recipeId, UINT result, UINT ingrCount, UINT buffLen, PCHAR buffer)
{
	CTL;
	const unsigned char *tempPtr = (const unsigned char*)buffer;
	if(result > 0)
	{
		if(CRecipeManager::Get().Enabled())
		{
			int systemMessageId = 0;
			ItemReq newItem;
			if(CRecipeManager::Get().OnCraft(pUser, recipeId, newItem, systemMessageId))
			{
				if(CItem *pTemplate = g_ObjectDB.GetTemplateObject(newItem.itemId)->SafeCastItem())
				{
					UINT newItemCount = 0;
					for(UINT n=0;n<ingrCount;n++)
					{
						INT32 itemClassId, amount, stackableType, param4, param5;
						tempPtr = Disassemble(tempPtr, "ddddd", &itemClassId, &amount, &stackableType, &param4, &param5);
					//	g_Log.Add(CLog::Blue, "[%s] recipe[%d] ingrCount[%d] cur[%d], param1[%d] param2[%d] stackableType[%d] param4[%d] param5[%d]", __FUNCTION__, recipeId, ingrCount, n, itemClassId, amount, stackableType, param4, param5);
						if(amount > 0)
						{
							//replace this item
							if(newItem.amount > 1 && pTemplate->pSID->nConsumeType == ConsumeNormal)
							{
								//simple replace
								PINT32 paramPtr = (PINT32)&tempPtr[-20];
								(*paramPtr) = newItem.itemId;
								(*(paramPtr+1)) = 1;
								(*(paramPtr+2)) = pTemplate->pSID->nConsumeType;
								for(int i = 1; i < newItem.amount; i++)
								{
									buffLen += Assemble(&buffer[buffLen], (8190 - buffLen), "ddddd", newItem.itemId, 1, ConsumeNormal, 0, 0);
									newItemCount++;
								}
							}else
							{
								//simple replace
								PINT32 paramPtr = (PINT32)&tempPtr[-20];
								(*paramPtr) = newItem.itemId;
								(*(paramPtr+1)) = newItem.amount;
								(*(paramPtr+2)) = pTemplate->pSID->nConsumeType;
								(*(paramPtr+3)) = 0;
								(*(paramPtr+4)) = 0;
							}
							break;
						}
					}
					if(newItemCount)
						ingrCount+=newItemCount;

					if(systemMessageId)
						pUser->pSocket->SendSystemMessage(systemMessageId);
				}
			}
		}
	}

	typedef void(*f)(PVOID, User*, UINT, UINT, UINT, UINT, PCHAR);
	f(0x52AD40L)(instancePtr, pUser, recipeId, result, ingrCount, buffLen, buffer);
}

void PlayerAction::OnSendSkillAcquireList(User *pUser, UINT type)
{
	guard;

	pUser->pED->skillMasterFlag = 0;
	typedef void(*f)(User*, UINT);
	f(0x84E5E0L)(pUser, type);

	unguard;
}

bool PlayerAction::OnValidateCpRegen(User *pUser)
{
	guard;

	bool ret = false;

	typedef bool (*f)(User*);
	ret = f(0x4DBBD0L)(pUser);

	//apply our p_regen_cp
	if(pUser->pSD->nIsPC)
	{
		pUser->pSD->cpRegen *= pUser->pSD->pExData->SkillMod.regenCpMul;
		pUser->pSD->cpRegen += pUser->pSD->pExData->SkillMod.regenCpAdd;
	//	g_Log.Add(CLog::Blue, "[%s] [%f] [%f] total regen[%f]", __FUNCTION__, pUser->pSD->pExData->SkillMod.regenCpMul, pUser->pSD->pExData->SkillMod.regenCpAdd, pUser->pSD->cpRegen);

	}


	unguard;
	return ret;
}

void PlayerAction::OnDBRequestRenewSubjob(PVOID pInstance, User *pUser, UINT subjobId, UINT newClass, bool byBuilder, int npcIndex, int userIndex)
{
	guard;

	if(g_SubStack.IsEnabled())
	{
		if(subjobId < 4)
		{
			//check player substack
			if(pUser->pED->subStackInfo.classId[subjobId][0] != UINT_MAX)
			{
				//clear it
				pUser->pED->subStackInfo.classId[subjobId][0] = UINT_MAX;
				pUser->pED->subStackInfo.classId[subjobId][1] = UINT_MAX;
				g_DB.RequestSaveSubStack(pUser->nDBID, subjobId, UINT_MAX, UINT_MAX);
				g_Logger.Add(L"User[%s] removed subjob[%d] - deleting stacks!", pUser->pSD->wszName, subjobId);
			}
		}
	}

	typedef void(*f)(PVOID, User*, UINT, UINT, bool, int, int);
	f(0x533B60L)(pInstance, pUser, subjobId, newClass, byBuilder, npcIndex, userIndex);

	unguard;
}

void PlayerAction::OnBroadcastCombatModeStart(User *pUser)
{
	guard;

	pUser->pED->broadcastCombatModeTimestamp = time(0) + 3;
	typedef void(*f)(User*);
	f(0x4EBA50L)(pUser);

	unguard;
}

void PlayerAction::OnAction(User *pUser, CObject *pTarget, bool force, bool notMove)
{
//	g_Log.Add(CLog::Blue, "[%s] User[%s] force[%d] notMove[%d]", __FUNCTION__, pUser->pSD->wszName, force, notMove);
	typedef void (*f)(User*, CObject*, bool, bool);
	f(0x4E46C0L)(pUser, pTarget, force, notMove);
}

void PlayerAction::CUserSocket_TimerExpired(CUserSocket *pSocket, UINT id)
{
	guard;

//	g_Log.Add(CLog::Blue, "[%s] id[%d] s1[%d]s2[%d] [%d]", __FUNCTION__, id, pSocket->state, pSocket->socketStatus, GetTickCount());

	typedef void(*f)(CUserSocket*, UINT);
	f(0x877BA0L)(pSocket, id);

	if(pSocket->socketStatus != 2)
	{
		g_CliExt.TimerExpired(pSocket);
	}

	unguard;
}

void PlayerAction::DBIncreaseUserDuelCount(LPVOID pInstance, UINT charId)
{
	guard;
	if(charId > 0)
	{
		typedef void(*f)(LPVOID, UINT);
		f(0x52A9F0L)(pInstance, charId);
	}
	unguard;
}

void PlayerAction::OnTakeWithdrawPenalty(User *pUser)
{
	guard;
	if(pUser->pSD->nPledgeType != CPledge::ACADEMY)
	{
		typedef void(*f)(User*);
		f(0x803BE0L)(pUser);
	}
	unguard;
}

void PlayerAction::OnRestartSendSaveCharacterInfo(LPVOID lpInstance, User *pUser, UINT param1, bool param2)
{
	guard;
	typedef void(*f)(LPVOID, User*, UINT, bool);
	f(0x53A250L)(lpInstance, pUser, 1, true);
	unguard;
}

void PlayerAction::ShowTutorialPage(CUserSocket *pSocket, const char* format, BYTE opCode, const WCHAR* wHtml)
{
	guard;
	if(User *pUser = pSocket->pUser->SafeCastUser())
	{
		g_HtmlFilter.OnSendHtml(pUser, L"ShowTutorialPage.htm", wHtml, -1);
		pSocket->Send(format, opCode, wHtml);
	}
	unguard;
}

void PlayerAction::NpcHtmlSender(CUserSocket *pSocket, const char* format, BYTE opCode, UINT talkTargetId, const WCHAR* wHtml)
{
	guard;
	if(User *pUser = pSocket->pUser->SafeCastUser())
	{
		g_HtmlFilter.OnSendHtml(pUser, L"NpcHtmlSender.htm", wHtml, 0);
		pSocket->Send(format, opCode, talkTargetId, wHtml);
	}
	unguard;
}

void PlayerAction::ShowCommunityBoard(CUserSocket *pSocket, const char* format, BYTE opCode, BYTE param1, const WCHAR* wBypass2, const WCHAR* wBypass3, const WCHAR* wBypass4, const WCHAR* wBypass5, const WCHAR* wBypass6, const WCHAR* wBypass7, const WCHAR* wBypass8, const WCHAR* wBypass9, const WCHAR* wHtml)
{
	guard;

	if(g_HtmlFilter.IsEnabled())
	{
		wstringstream dataStream;
		dataStream << wBypass2 << endl << wBypass3 << endl << wBypass4 << endl << wBypass5 << endl << wBypass6 << endl << wBypass7 << endl << wBypass8 << endl << wBypass9 << endl << wHtml << endl;
		g_HtmlFilter.OnSendHtml(pSocket->pUser, L"community_board.htm", dataStream.str().c_str(), 0);
	}

	pSocket->Send(format, opCode, param1, wBypass2, wBypass3, wBypass4, wBypass5, wBypass6, wBypass7, wBypass8, wBypass9, wHtml);

	unguard;
}

void PlayerAction::ShowHTML(User *pUser, const WCHAR *wName, const WCHAR *wHtml, UINT itemClassId)
{
	guard;
	if( pUser->SafeCastUser())
	{
		g_HtmlFilter.OnSendHtml(pUser, wName, wHtml, itemClassId);

		typedef void (*f)(User*, const WCHAR*, const WCHAR*, UINT);
		f(0x80A060L)(pUser, wName, wHtml, itemClassId);
	}

	unguard;
}

void PlayerAction::SendAbnormalStatusInfo(User *pUser)
{
	guard;
	pUser->SendAbnormalStatusInfo();
	unguard;
}

void PlayerAction::SummonPet(User *pUser, CItem *pPetCollar)
{
	guard;

	if(!g_DisablePets)
	{
		if(pUser->ValidUser())
		{
			if(pUser->IsNowTrade())
			{
				pUser->TradeCancel();
			}
		}

		typedef void(*f)(User*, CItem*);
		f(0x8169F0L)(pUser, pPetCollar);
	}else
	{
		pUser->pSocket->SendSystemMessage(L"Pet System is temporary disabled and you cannot summon any pet.");
	}

	unguard;
}

void PlayerAction::TradeCancelled(CTrade *pTrade, User *pTrader)
{
	guard;

	UINT trader1Id = pTrade->traderId[0];
	UINT trader2Id = pTrade->traderId[1];

	typedef void(*f)(CTrade*, User*);
	f(0x7F5330L)(pTrade, pTrader);

	if(User *p1 = User::GetUserBySID(&trader1Id))
	{
		p1->SetItemListChanged();
	}
	
	if(User *p1 = User::GetUserBySID(&trader2Id))
	{
		p1->SetItemListChanged();
	}
	unguard;
}

bool PlayerAction::OnTeleportToLocation(User *pUser, int nX, int nY, int nZ, int FadeScreen)
{
	typedef bool (*f)(User*, int, int, int, int);

	if(pUser->ValidUser() && !pUser->pSD->nInBattleField && pUser->pSD->nInPeaceZone && pUser->pSD->nKarma == 0)
	{
		if(pUser->pED->tvtUser.status == TvT::UserNone)
		{
			if(g_SpawnProtection.IsEnabled())
			{
				pUser->pED->spawnProtection = 1;	//start request
			}
		}
	}

	pUser->pCreatureController->AsyncDoNothing();
	
	if(pUser->IsNowTrade())
	{
		pUser->TradeCancel();
	}

	if(pUser->pSD->nIsPledgeLeader && pUser->HasAbnormal(179))
	{
		pUser->DispelByAbnormal(179, true);
	}

	if(pUser->ValidUser())
	{
		pUser->pSD->pExData->teleportTick = GetTickCount();
	}

	if(f(0x8151E0)(pUser, nX, nY, nZ, FadeScreen))
	{	
		g_CursedWeaponSystem.OnTeleport(pUser);
		return true;
	}

	return false;
}

void PlayerAction::OnTimerExpired(User *pUser, int nTimerID)
{
	guard;

	if(nTimerID == 1986)
	{
		if(pUser->ValidUser())
		{
			//Scheme Buffer
			pUser->DispatchQueuedBuff();
		}
		unguard;
		return;
	}

	if(nTimerID == VALIDATE_AUTO_HP_TIMER)
	{
		g_ItemAutoConsume.OnTiggerHp(pUser);
		unguard;
		return;
	}else if(nTimerID == VALIDATE_AUTO_CP_TIMER)
	{
		g_ItemAutoConsume.OnTiggerCp(pUser);
		unguard;
		return;
	}else if(nTimerID == VALIDATE_AUTO_CP_ADV_TIMER)
	{
		g_ItemAutoConsume.OnTiggerCpAdv(pUser);
		unguard;
		return;
	}else if(nTimerID == VALIDATE_AUTO_MP_TIMER)
	{
		g_ItemAutoConsume.OnTiggerMp(pUser);
		unguard;
		return;
	}

	if(nTimerID == 0)
	{
		if(pUser->ValidUser())
		{
			pUser->pSD->nTimerTick++;

			UINT currentTime = static_cast<UINT>(time(0));

			if(g_SaveAbnormalsIntervalTime > 0)
			{
				if(pUser->pSD->nAlive)
				{
					if(pUser->pED->leftWorld == FALSE)
					{
						if((pUser->pSD->nTimerTick % g_SaveAbnormalsIntervalTime) == 0)
						{
							pUser->SaveInfectedSkills();
						}
					}
				}
			}

			pUser->TimerExpiredForHiddenAbnormals();

			g_ItemAutoConsume.OnTiggerHp(pUser);
			g_ItemAutoConsume.OnTiggerCp(pUser);
			g_ItemAutoConsume.OnTiggerCpAdv(pUser);
			g_ItemAutoConsume.OnTiggerMp(pUser);

			g_TerritoryChecker.ValidateRideTerritory(pUser);

			//validate combat mode
			if(pUser->inCombatMode)
			{
				if(pUser->pED->broadcastCombatModeTimestamp < pUser->combatModeTimeStamp)
				{
					PlayerAction::OnBroadcastCombatModeStart(pUser);
				}
			}


			if(g_SpawnProtection.IsEnabled())
			{
				if(pUser->pED->spawnProtectionTimeout)
				{
					if(pUser->pED->spawnProtectionTimeout <= currentTime)
					{
						g_SpawnProtection.DeactivateProtection(pUser);
					}else
					{
						INT32 diffTime = pUser->pED->spawnProtectionTimeout - currentTime;
						if((diffTime % 5) == 0 || diffTime < 5)
						{
							if(pUser->pED->spawnProtectionTimeSent != currentTime)
							{
								pUser->pED->spawnProtectionTimeSent = currentTime;
								//2514	1	a,The effect of Spawn Protection will expire in $s1 second(s).\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
								CSystemMessage msg(2514);
								msg.AddNumber(diffTime);
								pUser->SendSystemMessage(&msg);
							}
						}
					}
				}
			}

			pUser->pSD->pExData->requestJoinParyRoom.Expired();
			g_OfflineShop.TimerExpired(pUser);
			if(g_PrivateStoreSystem.StoreInDB())
			{
				g_PrivateStoreSystem.TimerExpired(pUser);
			}
			if(g_VIPSystem.IsEnabled())
			{
				g_VIPSystem.TimerExpired(pUser);
			}

			if( g_AIOSystem.IsEnabled() )
			{
				g_AIOSystem.TimerExpired(pUser);
			}

			g_DailyPvP.TimerExpired(pUser);

			if((pUser->pSD->nStoreMode == 3 || pUser->pSD->nStoreMode == 5 || pUser->pSD->nStoreMode == 1) && (pUser->pSD->nTimerTick % 5) == 0)
			{
				if(!g_PrivateStoreSystem.GotEnoughAdenaToBuy(pUser))
				{
					pUser->QuitPrivateStore();
					if(pUser->pSocket)
					{
						pUser->pSocket->SendSystemMessage(L"You've got not enough adena to handle your private store!");
					}
				}
			}

			if((pUser->pSD->nTimerTick % 10) == 0 )
			{
				CBuilderCommand::ValidateBuilder(pUser);
			}

			if(CParty *pParty = pUser->GetParty())
			{
				if(pParty->GetMaster() == pUser)
				{
					INT32 partyCount = pUser->GetMPCCPartyAmount();
					if(partyCount > 0)
					{
						pUser->pSD->nIsCCOwner = 1;
						pUser->pSD->nCCPartyAmount = partyCount;
						if((pUser->pSD->nTimerTick % 5) == 0)
						{
							pUser->pSD->nCCMembersDistance = pUser->GetMPCCMemberDistance();
						}
					}
				}

				if(pUser->pSD->nCursedOwner)
				{
					pParty->LeaveParty(pUser);
					pUser->SendSystemMessage(L"You cannot be a party member while equipping cursed weapon!");
				}
			}else if(pUser->pSD->nIsCCOwner)
			{
				pUser->pSD->nIsCCOwner = 0;
				pUser->pSD->nCCPartyAmount = 0;
			}

			if(!pUser->IsNowTrade())
			{
				if((pUser->pSD->nTimerTick % 10) == 0 )	//every 10 sec
				{
					DWORD color = 0;
					for(map<INT32, DWORD>::iterator it = g_ItemNicknameColor.begin(); it != g_ItemNicknameColor.end(); it++)
					{
						if(pUser->inventory.HaveItemByClassID(it->first, 1))
						{
							color = it->second;
						}
					}
					if(color != pUser->pED->nicknameColor)
					{
						pUser->pED->nicknameColor = color;
						pUser->SetUserInfoChanged();
						pUser->SetCharInfoChanged();
					}

					color = 0;
					for(map<INT32, DWORD>::iterator it = g_ItemTitleColor.begin(); it != g_ItemTitleColor.end(); it++)
					{
						if(pUser->inventory.HaveItemByClassID(it->first, 1))
						{
							color = it->second;
						}
					}
					if(color != pUser->pED->titleColor)
					{
						pUser->pED->titleColor = color;
						pUser->SetUserInfoChanged();
						pUser->SetCharInfoChanged();
					}
				}

				if((pUser->pSD->nTimerTick % 5) == 0)
				{
					pUser->ValidateItemExSkill();
				}

				if(g_ValidateStackableItems && (pUser->pSD->nTimerTick % 3) == 0)
				{
					if(!pUser->ValidateStackableItems())
					{
						pUser->pSD->pExData->kickTick = 2;
					}
				}
				if((pUser->pSD->nTimerTick % 30) == 0)
				{
					g_HeroSystem.TimerExpired(pUser);
				}
				if((pUser->pSD->nTimerTick % 25) == 0)
				{
					pUser->ValidateLifeTimeItems();
				}
			}
		}
	}

	typedef void(*f)(User*, int);
	f(0x00845090L)(pUser, nTimerID);	//User::TimerExpired

	if(pUser->ValidUser() && nTimerID == 0)
	{
		pUser->ValidateTerritory();
		g_TvT.TimerExpired(pUser);
		g_ItemDelivery.OnTimerExpired(pUser);
		if(g_Auction.IsEnabled())
		{
			if(pUser->pED->auctionUser.paymentAskTimestamp < time(0))
			{
				pUser->pED->auctionUser.paymentAskTimestamp = time(0) + 60;
				g_DB.RequestAuctionPaymentAsk(pUser->nDBID);
			}
		}

		if(pUser->pED->voteRewardMsgRequest)
		{
			pUser->pED->voteRewardMsgRequest = FALSE;
			g_VoteReward.OnLoad(pUser);
			g_DB.RequestSaveVotePoints(pUser->pSocket->accountId, pUser->nDBID, pUser->pED->voteRewardPoints);
		}
		
		LONG sendUserInfo = InterlockedExchange(&pUser->pED->userInfoChanged, FALSE);
		if(sendUserInfo)
		{
			pUser->SendUserInfo();
		}
		if(InterlockedExchange(&pUser->pED->charInfoChanged, FALSE))
		{
			pUser->SendCharInfo();
		}
		if(InterlockedExchange(&pUser->pED->itemListChanged, FALSE))
		{
			pUser->SendItemList(false);
		}
		if(InterlockedExchange(&pUser->pED->abnormalStatusChanged, FALSE))
		{
			pUser->SendAbnormalStatusInfo();
		}
		if(InterlockedExchange(&pUser->pED->validateParametersRequest, FALSE))
		{
			pUser->ValidateParameters();
		}

		if(pUser->pSD->pExData->kickTick > 0)
		{
			pUser->pSD->pExData->kickTick--;
			if(pUser->pSD->pExData->kickTick == 0)
			{
				pUser->Kick();
			}
		}
	}
	unguard;
}

bool PlayerAction::UnequipItem(User *pUser, int nSlotType)
{
	if(pUser->ValidUser())
	{
		if(pUser->pSD->nCursedOwner && nSlotType == CItem::LRHand)
				return false;
		
		return pUser->UnEquipItem(nSlotType);
	}
	return false;
}

CParty *PlayerAction::OnUpdateHP(User *pUser)
{
	if(g_DuelSystem.IsEnabled())
	{
		if(pUser->ValidUser() && pUser->pSD->DuelInfo.DuelID && pUser->pSD->DuelInfo.Fighting)
		{
			g_DuelSystem.UpdateDuelUserInfo(pUser);
		}
	}
	return pUser->GetParty();
}

bool PlayerAction::UseItem(User *pUser, CItem *pItem, int nForce)
{
	if(pItem->IsValidItem() && pUser->ValidUser())
	{
		if(pUser->IsNowTrade())
		{
			pUser->TradeCancel();
			return false;
		}
		if(pUser->pSD->nStoreMode > 0)
		{
			pUser->SendSystemMessage(L"You cannot use any item while operating a private store!");
			return false;
		}

		if(pUser->pSD->nCursedOwner)
		{
			int nSlotType = pItem->pSID->nSlotType;
			if(nSlotType == 256 || nSlotType == 128 || nSlotType == 16384) //BLock equip any weapon/shield with CursedWeapon
			{
				return false;
			}else 
			if(pItem->pSID->nItemID == 6408) //Formal Wear
				return false;

			//block all soulshots and spiritshots
			if(pItem->pII->defaultAction == 3 || pItem->pII->defaultAction == 4)
			{
				return false;
			}

		}

		if(pUser->olympiadUser.IsInside())
		{
			for(list<int>::iterator Iter = g_OlympiadForbiddenItemList.begin(); Iter!= g_OlympiadForbiddenItemList.end();Iter++)
			{
				if((*Iter) == pItem->pSID->nItemID)
				{
					pUser->pSocket->SendSystemMessage(1508);
					return false;
				}
			}

			if(g_OlympiadDB.IsForbiddenItem(pItem->pSID->nItemID))
			{
				pUser->pSocket->SendSystemMessage(1508);
				return false;
			}

			for(UINT m=0;m<g_OlympiadForbiddenItemGrade.size();m++)
			{
				if(g_OlympiadForbiddenItemGrade[m] == pItem->pII->nCrystalType)
				{
					pUser->pSocket->SendSystemMessage(1508);
					return false;
				}
			}

			if(g_OlympiadMaxAllowedWeaponEnchant > 0 && pItem->pSID->nItemType == 0)
			{
				if(g_OlympiadMaxAllowedWeaponEnchant < pItem->pSID->nEnchantLevel)
				{
					pUser->pSocket->SendSystemMessage(1508);
					return false;
				}
			}
			if(g_OlympiadMaxAllowedArmorEnchant > 0 && (pItem->pSID->nItemType == 1 || pItem->pSID->nItemType == 2))
			{
				if(g_OlympiadMaxAllowedArmorEnchant < (int)pItem->pSID->nEnchantLevel)
				{
					pUser->pSocket->SendSystemMessage(1508);
					return false;
				}
			}

		}

		if(pUser->pED->tvtUser.status == TvT::UserFighting || pUser->pED->tvtUser.status == TvT::UserPreparing)
		{
			if(!g_TvT.CanUse(pItem))
			{
				pUser->pSocket->SendSystemMessage(L"You cannot use this item during TvT event.");
				return false;
			}
		}

		int TerritoryID = pUser->pSD->nInBlockItemsZone;
		if(TerritoryID)
		{
			CBlockItemsTerritory *pTerritory = g_TerritoryChecker.GetTerritory(TerritoryID)->SafeCastBlockItems();
			if(pTerritory)
			{
				if(pTerritory->IsBlocked(pItem->pSID->nItemID))
				{
					pUser->GetSocket()->SendSystemMessage(61);
					return false;
				}
			}
		}
		
		return pUser->UseItem(pItem, nForce);
	}
	return false;
}

bool PlayerAction::DeleteShipTicket(User *pUser, INT32 ticketId, INT32 count)
{
	if(pUser->ValidUser())
	{
		if(pUser->IsNowTrade())
			pUser->TradeCancel();

		if(CSummon *pSummon = pUser->GetSummon())
		{
			pUser->DespawnSummon();
		}
	}

	typedef bool (*f)(User*, INT32, INT32);
	UINT64 addr = (*reinterpret_cast<PUINT64>(pUser));
	addr += 0x578;
	addr = (*reinterpret_cast<PUINT64>(addr));

	return f(reinterpret_cast<f>(addr))(pUser, ticketId, count);
}

bool PlayerAction::OnHear(User* pUser, User* pTalker, PWCHAR wTalkerName, const WCHAR* wMessage, int nType)
{
	typedef bool (*f)(User*, User*, PWCHAR, const WCHAR*, int);
	if(pUser->ValidUser())
	{
		if(nType == 9) //alliance
		{
			if(pUser->IsAcademyMember())
				return false;
		}
	}
	return f(0x82D810L)(pUser, pTalker, wTalkerName, wMessage, nType);
}

bool PlayerAction::OnClassChange(User *pUser, int nClass)
{
	bool bRet = pUser->ChangeClass(nClass);
	if(pUser->ValidUser())
	{
		ClanSys::OnChangeClass(pUser, nClass);
	}
	if(bRet)
	{
		pUser->requestSaveCharacterInfoTick = 195;
	}
	return bRet;
}

void PlayerAction::OnSendXPnSP(CUserSocket *pSocket, const char* format, BYTE opCode, DWORD MsgID, DWORD NumOfVars, DWORD Type1, int Exp, DWORD Type2, int SP)
{
	if(User *pUser = pSocket->GetUser())
	{

		UINT currentThreadIdx = GetThreadIndex();
		if(currentThreadIdx < 32)
		{
			if(g_IsDistributingExpSp[currentThreadIdx])
			{
				if(pUser->CanGetPartyBonus(g_DistributingCreature[currentThreadIdx]))
				{
					double modExp = static_cast<double>(Exp);
					modExp *= g_PartyExpRate;
					Exp = static_cast<INT32>(modExp);

					modExp = static_cast<double>(SP);
					modExp *= g_PartyExpRate;
					SP = static_cast<INT32>(modExp);
				}
			}
		}

		if(pUser->pSD->vipLevel)
		{
			Exp = g_VIPSystem.GetExp(Exp, pUser->pSD->vipLevel);
			SP = g_VIPSystem.GetSP(SP, pUser->pSD->vipLevel);
		}

		if(pUser->pSD->pExData->stopIncExp)
		{
			return;
		}

		//exp curve
		{
			double exp = static_cast<double>(Exp);
			exp *= g_ExpCurve[pUser->pSD->nLevel];
			Exp = static_cast<INT32>(exp);
		}
		
		double exp = static_cast<double>(Exp);
		exp *= pUser->pSD->pExData->SkillMod.bonusExp;
		exp *= g_DnNSystem.GetRate(DnN::RateExp);
		Exp = static_cast<INT32>(exp);

		double sp = static_cast<double>(SP);
		sp *= pUser->pSD->pExData->SkillMod.bonusSp;
		sp *= g_DnNSystem.GetRate(DnN::RateSp);
		SP = static_cast<INT32>(sp);

		//exp overflow
		INT64 n4Percent = pUser->pSD->nExp;
		n4Percent *= 5;
		n4Percent /= 100;
		n4Percent *= -1;

		if(Exp < n4Percent)
		{
			n4Percent *= -1;
			Exp = n4Percent;
		}

		//sp overflow
		if(SP == -2147483648)
		{
			INT64 sp = pUser->pSD->nSp;
			sp *= 5;
			sp /= 100;
			SP = sp;
		}
	}

	pSocket->Send(format, opCode, MsgID, NumOfVars, Type1, Exp, Type2, SP);
}

bool PlayerAction::OnDeleteItemInInventoryBeforeCommit(User *pUser, int itemId, int amount)
{
	if(pUser->ValidUser())
	{
		if(pUser->IsNowTrade())
		{
			pUser->TradeCancel();
		}
	}

	for(UINT n=0;n<g_InfinityArrow.size();n++)
	{
		if(itemId == g_InfinityArrow[n])
		{
			return true;
		}
	}

	typedef bool(*f)(User*, int, int);
	return f(0x827150L)(pUser, itemId, amount);
}

void PlayerAction::OnSendETCBuffStatusInfo(CSocket *pSocket, const char *format, ...)
{
	va_list tag;
	va_start(tag, format);
/* c */ BYTE cHeader = va_arg(tag, BYTE); 
/* d */ DWORD dCharges = va_arg(tag, DWORD); 
/* d */ DWORD dWeightPenalty = va_arg(tag, DWORD); 
/* d */ DWORD dBlockChats = va_arg(tag, DWORD); 
/* d */ DWORD dDangerArea = va_arg(tag, DWORD); 
/* d */ DWORD dGradePenalty = va_arg(tag, DWORD); 
	va_end(tag);
	//New C5 Things
	int nCharmOfCourage = 0;
	User *pUser = pSocket->CastUserSocket()->GetUser();
	int nDeathPenalty = 0;
	if(pUser->ValidUser())
	{
		nDeathPenalty = pUser->pSD->nDeathPenalty;
		if(dGradePenalty == 0 && pUser->pSD->pExData->ArmorPenalty)
			dGradePenalty = 1;
		if(pUser->pED->blockChatZoneId)
			dBlockChats = 1;

		if(g_GradePenalty.Enabled())
		{
			g_GradePenalty.CheckState(pUser, dGradePenalty > 0 ? true : false);
		}

		nCharmOfCourage = pUser->pED->charmOfCourageLevel;
	}

	pSocket->Send("cddddddd", cHeader, dCharges, dWeightPenalty, dBlockChats, dDangerArea, dGradePenalty, nCharmOfCourage, nDeathPenalty);
}

void PlayerAction::SaveCharacterInfo(INT64 nCDB, const char *format, ...)
{
	CPacket pck;
	va_list list;
	va_start(list, format);
/*c*/	BYTE opCode = va_arg(list, BYTE); pck.WriteC(opCode);
/*d*/	DWORD nCharID = va_arg(list, DWORD); pck.WriteD(nCharID);
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*f*/	pck.WriteF(va_arg(list, double));
/*f*/	pck.WriteF(va_arg(list, double));
/*f*/	pck.WriteF(va_arg(list, double));
/*f*/	pck.WriteF(va_arg(list, double));
/*f*/	pck.WriteF(va_arg(list, double));
/*f*/	pck.WriteF(va_arg(list, double));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
/*d*/	pck.WriteD(va_arg(list, DWORD));
	va_end(list);
	User *pUser = g_UserDB.GetUserByDBID(nCharID);
	if(pUser->ValidUser())
	{
		pck.WriteD(pUser->GetAugmentationID());
		if(g_Config.IsSet(CConfig::SPIRIT_SYSTEM))
		{
			pck.WriteD(pUser->pSD->nSpiritCounter);
		}
		else
		{
			pck.WriteD(0);
		}

			pck.WriteD(0); //hairdeco
			pck.WriteD(0); //hairall
			pck.WriteD(0); //EffectiveMana0
			pck.WriteD(0); //EffectiveMana1
			pck.WriteD(0); //EffectiveMana2
			pck.WriteD(0); //EffectiveMana3

	}else
	{
		pck.WriteD(NULL); //augmentation
		pck.WriteD(NULL); //spirit_count
		pck.WriteD(NULL); //hairdeco
		pck.WriteD(NULL); //hairall
		pck.WriteD(NULL); //EffectiveMana0
		pck.WriteD(NULL); //EffectiveMana1
		pck.WriteD(NULL); //EffectiveMana2
		pck.WriteD(NULL); //EffectiveMana3
	}

	 //ddddddddffffffddddddddddddddddddddddddddddddddd CachedPacket
	//cddddddddffffffddddddddddddddddddddddddddddddddd L2Server
	typedef void (*f)(INT64, const char *, size_t, PCHAR);
	f(0x0052ABE0L)(nCDB, "b", pck.GetSize(), pck.GetBuff());
}

extern BOOL g_OlympiadCheckHWID;

bool PlayerAction::RegisterInOlympiad(COlympiadUser *pOU)
{
	User *pUser = (User*)((UINT64)pOU - (UINT64)0x2CB0);
	if(pUser->ValidUser() && !pUser->pSD->nCursedOwner && pUser->pSD->activateOfflineShopTime == 0)
	{
		if( pUser->olympiadUser.GetState() != 0 )
		{
			pUser->GetSocket()->SendSystemMessage(1502);
			return false;
		}

		if(g_OlympiadCheckHWID)
		{
			if(g_UserDB.GetOlympiadUserAmountByIPnHWID(pUser->pSocket->addr.S_un.S_addr, pUser->pSocket->pED->longHWID.hash) > 0)
			{
				//send sys message
				pUser->SendSystemMessage(L"You already are registered in olympiad with other character!");
				return false;
			}
		}

		if(pUser->pED->aioId > 0)
		{
			pUser->SendSystemMessage(L"The AIO characters cannot participe in Olympiad Games!");
			return false;
		}
		tm ti;
		GetTimeInfo(ti);
		int endHour = 24;
		//check for 10 minutes before the end
		if(SSQPart::none == SSQSystem::GetInstance()->GetWinner())
		{
			//event period or none
			endHour = g_OlympiadPeriod[ti.tm_wday][0].endHour;
		}else
		{
			endHour = g_OlympiadPeriod[ti.tm_wday][1].endHour;
		}

		if(endHour == 0)
			endHour = 24;

		if( ( ti.tm_hour + 1 ) == endHour )
		{
			if(ti.tm_min > 50)
			{
				pUser->SendSystemMessage(L"There's only 10 minutes left till olympiad fights ends, it's too late to register.");
				return false;
			}
		}

		return pOU->RegisterOlympiadEntry();
	}
	return false;
}

int PlayerAction::OnDieLooseExp(User *pUser, int nLostExp)
{
	if(pUser->ValidUser() && nLostExp)
	{
		//g_Log.Add(CLog::Blue, "[%s] LostExp[%d] - 1", __FUNCTION__, nLostExp);

		if(pUser->pSD->nCursedVictim)
		{
			pUser->pSD->nCursedVictim = 0;
			//g_Log.Add(CLog::Blue, "[Ext] User[%S] killed by cursed weapon owner.", pUser->pSD->wszName);
			pUser->SetExpDown(-1);
			return pUser->ExpDown(-1);
		}

		if(pUser->pED->charmOfCourageLevel > 0 && (pUser->pSD->nInBattleField || pUser->inPvpZone))
		{
			pUser->SetExpDown(-1);
			return pUser->ExpDown(-1);
		}

		if(pUser->pED->tvtUser.status == TvT::UserFighting)
		{
			pUser->SetExpDown(-1);
			return pUser->ExpDown(-1);
		}

		if(pUser->pSD->nLevel < 9 && pUser->pSD->nKarma == 0)
		{
			if(pUser->pSD->nExp > 1)
			{
				pUser->SetExpDown(-1);
				return pUser->ExpDown(-1);
			}else
			{
				pUser->SetExpDown(0);
				return pUser->ExpDown(0);
			}
		}
		if(pUser->pSD->nLevel == 76)
		{
			nLostExp /= 2;
			pUser->SetExpDown(nLostExp);
		}else if(pUser->pSD->nLevel == 77)
		{
			nLostExp/=2;
			pUser->SetExpDown(nLostExp);
		}else if(pUser->pSD->nLevel == 78)
		{
			nLostExp/=3;
			pUser->SetExpDown(nLostExp);
		}else if(pUser->pSD->nLevel == 79)
		{
			nLostExp/=3;
			pUser->SetExpDown(nLostExp);
		}else if(pUser->pSD->nLevel == 80)
		{
			nLostExp/=4;
			pUser->SetExpDown(nLostExp);
		}
		
		
		if(pUser->pSD->pExData && pUser->pSD->pExData->SkillMod.nDecreaseLostExp)
		{
			double dPercent = 100 - pUser->pSD->pExData->SkillMod.nDecreaseLostExp;
			dPercent /= 100;
			dPercent *= nLostExp;
			nLostExp = (int)dPercent;
			pUser->SetExpDown(nLostExp);
			//g_Log.Add(CLog::Blue, "[%s] Called", __FUNCTION__);
		}
	}
	//g_Log.Add(CLog::Blue, "[%s] LostExp[%d] - 2", __FUNCTION__, nLostExp);
	return pUser->ExpDown(nLostExp);
}

void PlayerAction::OnDieDropItem(User *pUser, bool bOnBattlefield)
{
	typedef void (*f)(User*, bool);
	if(g_DeathPenalty.IsEnabled())
	{
		if(pUser->ValidUser() && pUser->pSD->nKarma > 0 && pUser->pSD->nPkCount >= 5)
		{
			f(0x81C310)(pUser, bOnBattlefield);
		}
	}else
	{
		f(0x81C310)(pUser, bOnBattlefield);
	}
}

bool PlayerAction::OnOlympiadStartMatch(COlympiadField *pField)
{
	bool bRet = pField->CheckGameExit();
	if(!bRet)
	{
		CObjectSP ObjectSP;
		CObjectSP::GetUserByDBID(&ObjectSP, pField->nPlayer1DBID);
		User *pUser = ObjectSP.pObject->GetUser();
		if(pUser->ValidUser())
		{
			if(!pUser->pSD->nAlive)
			{
				g_Log.Add(CLog::Error, "[%s] Death user[%S] participed to olympiad.", __FUNCTION__, pUser->pSD->wszName);
				pUser->Revive();
			}
			if(pUser->pED->spawnProtection)
			{
				g_SpawnProtection.DeactivateProtection(pUser);
			}
			
			pUser->pSD->fHP = pUser->pSD->MaxHP;
			pUser->pSD->fCP = pUser->pSD->MaxCP;
			pUser->pSD->fMP = pUser->pSD->MaxMP;
			pUser->SendHpInfo();
			pUser->SendMpInfo();
			if(g_OlympiadStopActionOnFightStart)
			{
				pUser->pCreatureController->AsyncDoNothing();
			}
		}
		CObjectSP ObjectSP2;
		CObjectSP::GetUserByDBID(&ObjectSP2, pField->nPlayer2DBID);
		pUser = ObjectSP2.pObject->GetUser();
		if(pUser->ValidUser())
		{
			if(!pUser->pSD->nAlive)
			{
				g_Log.Add(CLog::Error, "[%s] Death user[%S] participed to olympiad.", __FUNCTION__, pUser->pSD->wszName);
				pUser->Revive();
			}
			if(pUser->pED->spawnProtection)
			{
				g_SpawnProtection.DeactivateProtection(pUser);
			}
			pUser->pSD->fHP = pUser->pSD->MaxHP;
			pUser->pSD->fCP = pUser->pSD->MaxCP;
			pUser->pSD->fMP = pUser->pSD->MaxMP;
			pUser->SendHpInfo();
			pUser->SendMpInfo();
			if(g_OlympiadStopActionOnFightStart)
			{
				pUser->pCreatureController->AsyncDoNothing();
			}
		}

		User *pU1 = ObjectSP.pObject->SafeCastUser();
		User *pU2 = ObjectSP2.pObject->SafeCastUser();

		if(pU1 && pU2)
		{
			//Olympiad buff support
			vector<SkillPchId>& b1 = g_OlympiadDB.GetBuffSupport(pU1->pSD->nClass, pU2->pSD->nClass);
			vector<SkillPchId>& b2 = g_OlympiadDB.GetBuffSupport(pU2->pSD->nClass, pU1->pSD->nClass);
			for(UINT n=0;n<b1.size();n++)
			{
				if(CSkillInfo *pSI = g_SkillDB.GetSkillInfo(b1[n].skillId, b1[n].level))
				{
					pSI->ActivateSkill(pU1, pU1);
				}
			}
			for(UINT n=0;n<b2.size();n++)
			{
				if(CSkillInfo *pSI = g_SkillDB.GetSkillInfo(b2[n].skillId, b2[n].level))
				{
					pSI->ActivateSkill(pU2, pU2);
				}
			}
		}
	}
	return bRet;
}

void PlayerAction::OnOlympiadFinishMatch(COlympiadField *pOF)
{
	if(pOF)
	{
		CObjectSP ObjectSP1;
		CObjectSP::GetUserByDBID(&ObjectSP1, pOF->nPlayer1DBID);
		if(ObjectSP1.pObject->GetUser()->ValidUser())
		{
			User *pUser = ObjectSP1.pObject->GetUser();
			if(pUser->pSD->nAlive)
			{
				pUser->pCreatureController->AsyncDoNothing();
			}
		}
		CObjectSP ObjectSP2;
		CObjectSP::GetUserByDBID(&ObjectSP2, pOF->nPlayer2DBID);
		if(ObjectSP2.pObject->GetUser()->ValidUser())
		{
			User *pUser = ObjectSP2.pObject->GetUser();
			if(pUser->pSD->nAlive)
			{
				pUser->pCreatureController->AsyncDoNothing();
			}
		}
	}
	pOF->StatusFinishMatch();
}

void PlayerAction::OnOlympiadEscaping(User *pUser)
{
	if(pUser->ValidUser())
	{
		pUser->DispelAllByGM();
		pUser->ResetSkillReuse(); //Resetting skill reuse after match.
		//Adding Clan Skills
		CPledge *pPledge = pUser->GetPledge();
		if(pPledge)
		{
			pPledge->AcquirePledgeSkills(pUser);
		}
	}
}

bool PlayerAction::OnEnteringOlympiadStadion(COlympiadField *pOlympiadField)
{
	CObjectSP user1Sp;
	CObjectSP user2Sp;

	CObjectSP::GetUserByDBID(&user1Sp, pOlympiadField->nPlayer1DBID);
	CObjectSP::GetUserByDBID(&user2Sp, pOlympiadField->nPlayer2DBID);

	//Anty soe and some other exploit fix
	if(User *pUser = user1Sp.pObject->SafeCastUser())
	{
		pUser->pCreatureController->AsyncDoNothing();
		pUser->pSD->nWaitingForMatch = 0;
		if(g_SchemeBuffer.IsEnabled())
		{
			pUser->ClearQueuedBuffs();
		}
		pUser->TurnOffToggleSkills();
		if(g_InfinityShot.IsEnabled())
		{
			g_InfinityShot.AutoOff(pUser);
		}
		if(pUser->pSD->nAlive == 0)
				pUser->Revive();
	}

	if(User *pUser = user2Sp.pObject->SafeCastUser())
	{
		pUser->pCreatureController->AsyncDoNothing();
		pUser->pSD->nWaitingForMatch = 0;
		if(g_SchemeBuffer.IsEnabled())
		{
			pUser->ClearQueuedBuffs();
		}
		pUser->TurnOffToggleSkills();
		if(g_InfinityShot.IsEnabled())
		{
			g_InfinityShot.AutoOff(pUser);
		}
		if(pUser->pSD->nAlive == 0)
				pUser->Revive();
	}

	bool bReturn = pOlympiadField->SummonUser();
	if(bReturn)
	{
		if(User *pUser = user1Sp.pObject->SafeCastUser())
		{
			//deleting clan skills
			for(UINT n=370; n<392; n++)
			{
				pUser->DeleteSkill(n);
			}
			pUser->ValidateParameters();
			pUser->ClearHiddenAbnormals();
			pUser->pSD->pExData->HerbAbnormalMgr.Clear(-1);
			if(g_DuelSystem.IsEnabled())
			{
				if(pUser->pSD->DuelInfo.DuelID)
					g_DuelSystem.Interrupt(pUser->pSD->DuelInfo.DuelID);
			}
			
			COlympiadSystem::ValidateEquippedItems(pUser);
			pUser->DispelAllByGM();
		}else
		{
			g_Log.Add(CLog::Blue, "[%s] Cannont find First User[%d].", __FUNCTION__, pOlympiadField->nPlayer1DBID);
		}

		if(User *pUser = user2Sp.pObject->SafeCastUser())
		{
			//deleting clan skills
			for(UINT n=370; n<392; n++)
			{
				pUser->DeleteSkill(n);
			}
			pUser->ValidateParameters();
			pUser->ClearHiddenAbnormals();
			pUser->pSD->pExData->HerbAbnormalMgr.Clear(-1);
			if(g_DuelSystem.IsEnabled())
			{
				if(pUser->pSD->DuelInfo.DuelID)
					g_DuelSystem.Interrupt(pUser->pSD->DuelInfo.DuelID);
			}
			
			COlympiadSystem::ValidateEquippedItems(pUser);
			pUser->DispelAllByGM();
		}else
		{
			g_Log.Add(CLog::Blue, "[%s] Cannont find Second User[%d].", __FUNCTION__, pOlympiadField->nPlayer1DBID);
		}
	}
	return bReturn;
}

void PlayerAction::ObserverStart(User *pUser, int x, int y, int z, int yaw, int pitch, int period)
{
	typedef void (*f)(User*, int, int, int, int, int, int);
	
	if(pUser->ValidUser())
	{
		if(pUser->pSD->nStoreMode != 0)
		{
			pUser->SendSystemMessage(L"You cannot use observer mode when operating private store!");
			return;
		}
		if(pUser->olympiadUser.GetState() != 0)
		{
			pUser->SendSystemMessage(L"You cannot use observer mode when participing in Olympiad Fights!");
			return;
		}

		if(pUser->IsNowTrade())
			pUser->TradeCancel();

		pUser->pSD->nIsObserver = 1;
	}
	
	f(0x833880L)(pUser, x, y, z, yaw, pitch, period);
}

void PlayerAction::ObserverEnd(User *pUser)
{
	typedef void (*f)(User*);

	if(pUser->ValidUser())
	{
		if(pUser->IsNowTrade())
			pUser->TradeCancel();
		
		pUser->pSD->nIsObserver = 0;
	}

	f(0x813390L)(pUser);
}


void PlayerAction::OnPickUpItem(CInventory *pInventory, CItem *pItem, int nChangedType)
{
	//Normal Execution
	pInventory->SetInventoryChanged(pItem, nChangedType);
	User *pUser = pInventory->GetOwner()->GetUser();
	if(pUser->ValidUser())
	{
		g_CursedWeaponSystem.OnPickUp(pUser, pItem);
	}
}

void PlayerAction::OnAddItemToInventory(CInventory *pInventory, CItem *pItem, int nType)
{
	User *pUser = pInventory->GetOwner()->GetUser();
	if(nType == CInventory::ADD)
	{
		if(pItem->IsValidItem())
		{
			if(pUser->ValidUser())
			{
				g_Augmentation.MultiSellAddItem(pUser, pItem);
			}
		}
	}
	pInventory->SetInventoryChanged(pItem, nType);
	if(nType == CInventory::ADD)
	{

	}else if(nType == CInventory::UPDATE)
	{
		if(pItem->IsValidItem())
		{
			if(UINT lifeTime = g_ItemDBEx.GetLifeTime(pItem->pSID->nItemID))
			{
			//	g_Log.Add(CLog::Blue, "[%s] Update item stat [%d] item id [%d] count old life time[%d] new [%d]", __FUNCTION__, pItem->pSID->nItemID, pItem->pSID->nItemAmount, (pItem->nLifeTime - time(NULL)), (time(NULL) + LifeTime));
				pItem->nLifeTime = (time(0) + lifeTime);
				if(pUser->ValidUser())
				{
					//Send sys message
					//2411	1	$s1's life time has been refreshed.	0	79	9B	B0	FF			0	0	0	0	0		none
					CSystemMessage msg(2411);
					msg.AddItem(pItem->pSID->nItemID);
					pUser->SendSystemMessage(&msg);
				}		
			}
		}
	}
}

void PlayerAction::LoadItemsPacket(LPVOID pCDB, User *pUser)
{
	typedef void (*f)(LPVOID, User*);
	f(0x5314B0)(pCDB, pUser); //g_DB.RequestGetMacro(User *pUser)
}

void PlayerAction::OnLeaveWorld(User *pUser)
{
	if(pUser->ValidUser())
	{
		if(g_UserDB.GetUserByServerId(pUser->nObjectID))
		{
			try
			{
				if(g_DuelSystem.IsEnabled())
				{
					if(pUser->pSD->DuelInfo.DuelID)
						g_DuelSystem.LeaveWorld(pUser);
				}

				if(g_SchemeBuffer.IsEnabled())
				{
					g_DB.RequestSaveBuffList(pUser);
				}

				g_CursedWeaponSystem.OnLeaveWorld(pUser);
				ClanSys::LeaveWorld(pUser);
				COlympiadSystem::OlympiadFieldOnGameExit(pUser);
				if(g_PrivateStoreSystem.StoreInDB())
				{
					if(pUser->pSD->nStoreMode == 3 || pUser->pSD->nStoreMode == 5 || pUser->pSD->nStoreMode == 1)
					{
						g_PrivateStoreSystem.OnDeleteStore(pUser);
					}
				}
				if(pUser->pED->tvtUser.status != TvT::UserNone)
				{
					g_TvT.OnExit(pUser);
				}

				g_PartyWaitingList.DelMember(pUser);
				
				pUser->SaveInstances();

				g_UserCache.OnLeaveWorld(pUser);
				g_SocketDB.SetStatus(pUser->GetSocket(), CSocketData::LEAVING_WORLD);
				
				if(pUser->pSD->nActiveUser == 0)
				{
					pUser->SaveInfectedSkills();
				}

				g_DB.RequestSaveSubjob(pUser->nDBID, pUser->pSD->nSubClassIndex, pUser->pSD->fHP, pUser->pSD->fMP, pUser->pSD->nExp, pUser->pSD->nSp, pUser->pSD->nLevel, pUser->pSD->nHenna1, pUser->pSD->nHenna2, pUser->pSD->nHenna3);
				pUser->pED->leftWorld = TRUE;

			}catch(...)
			{
				EXCEPTION_LOG;
			}
		}
	}else
		g_Log.Add(CLog::Error, "[%s] Invalid User !!!", __FUNCTION__);

	//Normal Execution
	typedef void(*f)(User*);
	f(0x4D98F0L)(pUser);
}

void PlayerAction::IncreaseKarma(User *pUser, bool bWithPk)
{
	typedef void(*f)(User*, bool);

	if(pUser->pSD->nInCustomPvpZone)
	{
		return;
	}
	if(pUser->pSD->nCursedOwner)
	{
		bWithPk = false;
		return;
	}

	f(0x8146A0)(pUser, bWithPk);
}

int PlayerAction::OnGetRelationTo(User *pUser, User *pAttacker)
{
	typedef UINT (*_grt)(User*,User*);
	_grt grt = (_grt) 0x008008B0;
	//pUser - Target pCreature - Caster

	if(pUser->ValidUser() && pAttacker->ValidUser())
	{
		if(pUser->pSD->DuelInfo.DuelID && pUser->pSD->DuelInfo.DuelID == pAttacker->pSD->DuelInfo.DuelID && pUser->pSD->DuelInfo.Fighting)
			if(pAttacker->pSD->DuelInfo.DuelTeam > 0 && pAttacker->pSD->DuelInfo.DuelTeam!=pUser->pSD->DuelInfo.DuelTeam)
				return 0x6C4;

		if(pUser->pED->tvtUser.status == TvT::UserFighting && pAttacker->pED->tvtUser.status == TvT::UserFighting)
		{
			if(pUser->pED->tvtUser.team != pAttacker->pED->tvtUser.team)
			{
				return 0x6C4;
			}else
			{
				UINT Relation = grt(pUser, pAttacker);
				Relation |= User::IN_MY_PARTY;
				return Relation;
			}
		}

		if(pUser->IsSameAbnormalTeam(pAttacker))
		{
			UINT Relation = grt(pUser, pAttacker);
			Relation |= User::IN_MY_PARTY;
			return Relation;
		}

		if(g_AttackWarWithoutCTRL)
		{
			if(!pUser->IsMyParty(pAttacker) && pUser->pSD->nInPeaceZone == 0 && pUser->pSD->nPledgeID > 0 && pAttacker->pSD->nPledgeID > 0 && pUser->pSD->nPledgeID != pAttacker->pSD->nPledgeID)
			{
				if(CPledge *pPledge = pUser->GetPledge())
				{
					if(!pUser->IsAcademyMember() && pUser->olympiadUser.IsInside() == FALSE)
					{
						if(pUser->pSD->bGuilty == false)
						{
							if(pPledge->IsInWarWithEx(pAttacker->pledgeServerId))
							{
								UINT relation = grt(pUser, pAttacker);
								relation |= User::PVP_FLAG;
								return relation;
							}
						}
					}
				}
			}
		}

		//Custom PvP Zone
		if(pUser->pSD->nInCustomPvpZone || pAttacker->pSD->nInCustomPvpZone)
		{
			if(!pUser->IsInAlliance(pAttacker) && !pUser->IsMyParty(pAttacker))
				return 0x6C4;
		}
		//SSQ_ZONE fix
		if(pUser->pSD->nInSSQ_ZONE && pAttacker->pSD->nInSSQ_ZONE)
		{
			if(pUser->IsInAlliance(pAttacker))
			{
				UINT Relation = grt(pUser, pAttacker);
				Relation |= User::IN_MY_PARTY;

				return Relation;
			}

		}
	}
		
	return grt(pUser, pAttacker);
}

bool PlayerAction::OnEquipItem(User *pUser, CItem *pItem, int nForce)
{
	typedef bool (__fastcall *_EQI) (User *, CItem *, int);
	_EQI EquipItem = (_EQI) 0x008257C0;

	bool bRet = EquipItem(pUser, pItem, nForce);
	CShadowItem::EquipItem(pUser, pItem);

	return bRet;
}

bool PlayerAction::OnEnterWorld(User *pUser)
{
	if(pUser->ValidUser())
	{
		g_UserDB.AddUser(pUser);
		g_UserCache.OnEnterWorld(pUser);
		g_SocketDB.SetStatus(pUser->pSocket, CSocketData::IN_WORLD);

		g_CursedWeaponSystem.OnEnterWorld(pUser);
		g_DeathPenalty.ValidatePenalty(pUser);
		
		pUser->SendQuestList();
		ClanSys::EnterWorld(pUser);
		CShadowItem::EnterWorld(pUser);

		SSQSystem::ValidateCpSkill(pUser);

		if(g_FractionSystem.IsEnabled())
		{
			g_DB.RequestLoadUserFraction(pUser->nDBID);
		}
		if(g_VIPSystem.IsEnabled())
		{
			g_DB.RequestLoadVipStatus(pUser->nDBID);
		}

		if(g_VoteReward.IsEnabled())
		{
			g_DB.RequestLoadVotePoints(pUser->pSocket->accountId, pUser->nDBID);
		}

		if(g_RebirthSystem.Enabled())
		{
			g_DB.RequestLoadRebirth(pUser->nDBID, pUser->pSD->nSubClassIndex);
		}
		if(g_VisualArmor.IsEnabled())
		{
			g_DB.RequestLoadVisualArmor(pUser->nDBID);
		}

		if(g_GradePenalty.Enabled())
		{
			g_GradePenalty.Validate(pUser);
		}

		if(g_L2NetworkVote.Enabled())
		{
			UINT type = g_L2NetworkVote.GetIdentityType();
			wstringstream identity;
			switch(type)
			{
			case 0:
				identity << pUser->nDBID;
				break;
			case 1:
				identity << pUser->pSD->wszName;
				break;
			case 2:
				identity << pUser->pSocket->accountId;
				break;
			case 3:
				identity << pUser->pSocket->accountName;
				break;
			}
			if(identity.str().size() > 0)
			{
				g_DB.RequestLoadL2NetworkVote(pUser->nDBID, identity.str().c_str());
			}
		}

		if(g_Config.IsSet(CConfig::SKILL_REUSE_MANAGER))
		{
			g_SkillReuseManager.OnEnterWorld(pUser);
		}
		if(g_SchemeBuffer.IsEnabled())
		{
			g_DB.RequestLoadBuffList(pUser->nDBID, pUser->pSD->nSubClassIndex);
		}

		g_DB.RequestLoadInstanceTime(pUser->nDBID);

		pUser->SendETCBuffStatus();

		if(g_ShowInventoryOnEnterWorld)
		{
			pUser->pSD->pExData->TimerHandler.AddTimer(CCreatureTimerHandler::USER_SEND_ITEM_LIST, 4);
		}

		if(g_DailyPvP.IsEnabled())
		{
			g_DB.RequestLoadDailyPvP(pUser->nDBID);
		}

		if(g_SubStack.IsEnabled())
		{
			g_DB.RequestLoadSubStack(pUser->nDBID);
		}

		if(g_Config.IsSet(CConfig::SPIRIT_SYSTEM))
		{
			g_SpiritSystem.ValidateSkill(pUser);
			g_DB.RequestLoadSpiritCount(pUser->nDBID);
		}
		if(g_Config.IsSet(CConfig::ERA_SYSTEM))
			pUser->pSD->pExData->TimerHandler.AddTimer(CCreatureTimerHandler::USER_SEND_ERA_MOON, 2);
		
		if(g_AIOSystem.IsEnabled())
			g_DB.RequestLoadAIO(pUser->nDBID);
		if(g_ValidateAcquiredSkills)
			pUser->pSD->pExData->TimerHandler.AddTimer(CCreatureTimerHandler::USER_VALIDATE_ACQUIRED_SKILLS, 2);
		
		if(g_Config.IsSet(CConfig::ITEM_REUSE_MANAGER))
		{
			pUser->pSD->pExData->TimerHandler.AddTimer(CCreatureTimerHandler::VALIDATE_ITEM_REUSE, 3);
		}

		if(g_AutoLoot.IsEnabled() && g_AutoLoot.IsActiveByDefault())
		{
			pUser->pED->autoLoot = true;
		}

		g_Wedding.OnLoad(pUser);
		g_DB.RequestLoadNameTitleColor(pUser->nDBID);
		if(g_CharacterLock.IsEnabled())
		{
			g_DB.RequestLoadCharacterLock(pUser->nDBID);
		}

		g_AutoAnnounce.OnEnterWorld(pUser);
		pUser->SendLifeTimeItemsInfo();

		if(g_CliExt.IsEnabled())
		{
			char sHwid[33];
			memset(sHwid, 0, 33);
			memcpy(sHwid, pUser->pSocket->pED->longHWID.hash, 32);
			g_Logger.Add(L"Enter world user[%s][%d] account[%s][%d] ip[%d.%d.%d.%d] HWID[%S]", pUser->pSD->wszName, pUser->nDBID, pUser->pSocket->accountName, pUser->pSocket->accountId, pUser->pSocket->addr.S_un.S_un_b.s_b1, pUser->pSocket->addr.S_un.S_un_b.s_b2, pUser->pSocket->addr.S_un.S_un_b.s_b3, pUser->pSocket->addr.S_un.S_un_b.s_b4, sHwid);
			g_DB.RequestSaveHardwareId(pUser->nDBID, pUser->pSocket->pED->longHWID.hash);

			if(g_CliExt.GetVersion() >= 7)
			{
				//Job packet
				//                        \Device\NamedPipe\CF3614CF075AA0CB
				pUser->pSocket->Send("cddS", 0x87, 11, 1, L"CF3614CF075AA0CB");
			}
		}else
		{
			g_Logger.Add(L"Enter world user[%s][%d] account[%s][%d] ip[%d.%d.%d.%d]", pUser->pSD->wszName, pUser->nDBID, pUser->pSocket->accountName, pUser->pSocket->accountId, pUser->pSocket->addr.S_un.S_un_b.s_b1, pUser->pSocket->addr.S_un.S_un_b.s_b2, pUser->pSocket->addr.S_un.S_un_b.s_b3, pUser->pSocket->addr.S_un.S_un_b.s_b4);
		}

		pUser->ChangeTarget(NULL, true);
		if(g_ShowEnterWorldHtml)
		{
			const WCHAR* pFile = g_HtmlCache.Get(L"enterworld.htm");
			
			if(pFile)
			{
				PlayerAction::ShowHTML(pUser, L"enterworld.htm", pFile, 40000);
			}
		}

		if(int territoryId = g_TerritoryChecker.IsInTerritory(pUser, NO_RESTART_ZONE))
		{
			if(CNoRestartTerritory* pTerritory = g_TerritoryChecker.GetTerritory(territoryId)->SafeCastNoRestart())
			{
				IVector kickLoc = pTerritory->GetKickOutLocation();
				pUser->SendSystemMessage(L"You cannot login in here!");
				PlayerAction::OnTeleportToLocation(pUser, kickLoc.x, kickLoc.y, kickLoc.z, 0);
			}
		}
	}else
		g_Log.Add(CLog::Error, "[%s] NULL User!!!", __FUNCTION__);
	//Normal Execution
	return pUser->SendCoolTime();
}

bool PlayerAction::OnDropItem(User *pUser, CItem *pItem, int nAmount, FVector *pPos)
{
	if(pItem->IsValidItem())
	{
		if(pItem->nAugmentationID)
			return false;

		if(pItem->nProtectionTimeout > time(NULL))
		{
			if(pUser->ValidUser())
			{
				pUser->SendSystemMessage(L"This item is protected and cannot be dropped!");
			}
			return false;
		}
	}

	typedef bool ( * f)(User*, CItem*, int, FVector*);
	return f(0x800350L)(pUser, pItem, nAmount, pPos);
}

bool PlayerAction::OnDie(User *pUser, CCreature *pEnemy)
{
	typedef bool (__fastcall *_UD)(User*, CCreature*);
	_UD UD = (_UD) 0x0081DF00;

	if(pUser->pSD->pExData)
	{
		pUser->TimerExpiredForHiddenAbnormals(true);
		pUser->DispelByAbnormal(181, true);
	}

	if(g_DuelSystem.Die(pUser))
	{
		return false;
	}

	g_CursedWeaponSystem.OnDie(pUser, pEnemy);
	
	bool selfResurrection = false;
	if(pUser->pSD->pExData && pUser->pSD->pExData->SkillMod.nSelfResurrection)
	{
		selfResurrection = true;
	}

	
	g_TvT.OnDie(pUser, pEnemy);
	
	//PvP Reward zone
	int territoryId = pUser->pSD->pExData->territoryData.territoryId[PVP_REWARD_ZONE];
	if(territoryId != 0)
	{
		User *pEnemyUser = 0;
		if(pEnemy->ValidUser())
		{
			pEnemyUser = pEnemy->GetUser();
		}
		if(pEnemy->IsSummon())
		{
			pEnemyUser = pEnemy->CastSummon()->GetMaster();
		}
		
		if(pEnemyUser->ValidUser())
		{
			bool inWar = false;
			if(pUser->pSD->nPledgeID > 0 && pEnemyUser->pSD->nPledgeID > 0 && pUser->pSD->nPledgeID != pEnemyUser->pSD->nPledgeID)
			{
				if(CPledge *pPledge = pUser->GetPledge())
				{
					if(pPledge->IsInWarWithEx(pEnemyUser->pledgeServerId))
					{
						inWar = true;
					}
				}
			}
			if(pUser->pSD->bGuilty || pUser->pSD->nInCustomPvpZone || inWar)
			{
				CPvPRewardTerritory *pTerritory = g_TerritoryChecker.GetTerritory(territoryId)->SafeCastPvPReward();
				if(pTerritory)
				{
					pTerritory->Handle(pUser, pEnemyUser);
				}
			}
		}
	}



	if(g_Config.IsSet(CConfig::KILL_DEATH_STAT))
		g_KillDeathStat.OnUserDie(pUser, pEnemy);

	if(g_Config.IsSet(CConfig::SPIRIT_SYSTEM))
		g_SpiritSystem.OnHunt(pEnemy, pUser);

	if(g_Config.IsSet(CConfig::SIEGE_REPORT))
	{
		if(pUser->pSD->nPledgeID)
		{
			FVector Pos = pUser->pSD->Pos;
			int Killer = 0;
			int PledgeK = 0;
			int Owned = pUser->nDBID;
			int PledgeO = pUser->pSD->nPledgeID;
			
			if(pEnemy->ValidUser())
			{
				Killer = pEnemy->nDBID;
				PledgeK = pEnemy->pSD->nPledgeID;
			}
			else if(pEnemy->IsSummon())
			{
				User *pMaster = pEnemy->CastSummon()->GetMaster();
				if(pMaster->ValidUser())
				{
					Killer = pMaster->nDBID;
					PledgeK = pMaster->pSD->nPledgeID;
				}
			}
			if(Killer && PledgeO)
			{
				CSiegeStat::GetInstance().ReportKillDeathCount(Pos, Killer, PledgeK, Owned, PledgeO);
			}
		}
	}

	if(UD(pUser, pEnemy))
	{
		if(pUser->pED->charmOfCourageLevel > 0)
		{
			pUser->pED->charmOfCourageLevel = 0;
			pUser->SendETCBuffStatus();
		}
		ClanSys::Die(pUser, pEnemy);

		if(User *pEnemyUser = pEnemy->GetResponsibleUser())
		{
			PvP::CPvPAnnounce& pvpAnn = PvP::CPvPAnnounce::GetInstance();
			if(pvpAnn.Enabled())
			{
				pvpAnn.OnKill(pEnemyUser, pUser, pEnemyUser->pSD->nKarma ? true : false, (pUser->pSD->nInBattleField || pUser->pSD->nInCustomPvpZone) );
			}

			if(pUser->pSD->nInCustomPvpZone && pUser->IsInAlliance(pEnemyUser))
			{
				pEnemyUser->IncreaseDuelCount();
			}
			pEnemyUser->SetUserInfoChanged();
		}

		if(g_DailyPvP.IsEnabled())
		{
			if(pEnemy->ValidUser() || pEnemy->IsSummon())
			{
				if(pEnemy->IsSummon())
				{
					g_DailyPvP.GetPoint(pEnemy->CastSummon()->GetMaster(), pUser);
				}else
				{
					g_DailyPvP.GetPoint(pEnemy->SafeCastUser(), pUser);
				}
			}
		}
	
		if(pEnemy->IsMob())
		{
			g_DeathPenalty.OnDieByMob(pUser);
		}

		g_FractionSystem.OnDie(pUser, pEnemy);

		if(selfResurrection)
		{
			pUser->DispelByAbnormal(188, true);
			CSkillInfo *pSI = g_SkillDB.GetSkillInfo(2049, 1); //Bres
			if(pSI)
			{
				pSI->ActivateSkill(pUser, pUser, 0, 0);
			}
		}

		return true;
	}

	return false;
}


void PlayerAction::OnChangeSubjobEnd(User *pUser, bool bCreate)
{
	guard;

	typedef void (*f)(User*, bool);
	f(0x854050L)(pUser, bCreate);

	if(pUser->ValidUser())
	{
		g_DeathPenalty.ValidatePenalty(pUser);
	
		if(g_ArmorPenalty.IsEnabled())
		{
			pUser->pSD->pExData->ArmorPenalty = false;
			g_ArmorPenalty.Validate(pUser);
		}
		
		if(g_SchemeBuffer.IsEnabled())
		{
			g_DB.RequestLoadBuffList(pUser->nDBID, pUser->pSD->nSubClassIndex);
		}

		if(pUser->pSD->nCursedOwner)
		{
			//Acquire skills
			g_CursedWeaponSystem.OnChangeSubjob(pUser);
		}
		if(pUser->pSD->pExData)
		{
			pUser->ClearHiddenAbnormals();
			pUser->pSD->pExData->HerbAbnormalMgr.Clear(-1);
		}
		if(pUser->pED)
		{
			pUser->pED->lock.Enter();
			pUser->pED->itemSkills.clear();
			pUser->pED->lock.Leave();
		}

		if(g_RebirthSystem.Enabled())
		{
			RebirthUser& ru = pUser->pED->rebirthUser;
			ru.availablePoints = ru.bonusCon = ru.bonusDex = ru.bonusInt = ru.bonusMen = ru.bonusStr = ru.bonusWit = ru.rebirthCount = 0;
			g_DB.RequestLoadRebirth(pUser->nDBID, pUser->pSD->nSubClassIndex);
		}

		if(g_Config.IsSet(CConfig::SPIRIT_SYSTEM))
		{
			g_SpiritSystem.ValidateSkill(pUser);
		}

		
		if(g_GradePenalty.Enabled())
		{
			g_GradePenalty.Validate(pUser);
		}

		pUser->ValidateEquippedItems();

		SSQSystem::ValidateCpSkill(pUser);

		if(g_ValidateAcquiredSkills)
			pUser->ValidateAcquiredSkills();

		if(CPledge *pPledge = pUser->GetPledge())
		{
			pPledge->AcquirePledgeSkills(pUser);
		}

		pUser->ValidateAllOnWeapon();
		pUser->requestSaveCharacterInfoTick = 195;
		g_DB.RequestSaveSubjob(pUser->nDBID, pUser->pSD->nSubClassIndex, pUser->pSD->fHP, pUser->pSD->fMP, pUser->pSD->nExp, pUser->pSD->nSp, pUser->pSD->nLevel, pUser->pSD->nHenna1, pUser->pSD->nHenna2, pUser->pSD->nHenna3);
	}

	unguard;
}

void PlayerAction::OnChangeSubjobBegin(User *pUser)
{
	guard;

	if(g_SchemeBuffer.IsEnabled())
	{
		g_DB.RequestSaveBuffList(pUser);
		pUser->ClearQueuedBuffs();
	}

	pUser->pSD->pExData->SummonFriend.Clear();

	if(pUser->pSD->DuelInfo.DuelID)
	{
		g_DuelSystem.Interrupt(pUser->pSD->DuelInfo.DuelID);
	}

	if(g_InfinityShot.IsEnabled())
	{
		g_InfinityShot.AutoOff(pUser);
	}

	g_DB.RequestSaveCharacterInfo(pUser, true, false);

	typedef void (*f)(User*);
	f(0x851230)(pUser);

	unguard;
}

bool PlayerAction::COlympiadEntry_RemoveEntry(LPVOID pInstance, UINT userId)
{
	typedef bool (*f)(LPVOID, UINT);
	return f(0x708160L)(pInstance, userId);
}

bool PlayerAction::COlympiadEntry_AddEntry(LPVOID pInstance, UINT userId, bool bClassFree)
{
	typedef bool (*f)(LPVOID, UINT, bool);

	if(!COlympiadSystem::IsFightEnabled(bClassFree))
	{
		if(User *pUser = User::GetUserBySID(&userId))
		{
			if(bClassFree)
			{
				pUser->SendSystemMessage(L"The class free fights are disabled today.");
			}else
			{
				pUser->SendSystemMessage(L"The class fights are disabled today.");
			}
		}
		return false;
	}

	if(f(0x70CAA0L)(pInstance, userId, bClassFree))
	{
		return true;
	}
	return false;
}
