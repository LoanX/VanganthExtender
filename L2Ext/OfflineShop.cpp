#include "StdAfx.h"
#include "SocketDB.h"
#include "OfflineShop.h"
#include "OfflineSocketDB.h"
#include "TerritoryData.h"

extern DWORD g_ItemPacketReuse;

/*
[OfflineShop]
Enabled=1
;Auto activate - when player exits game it checks if offline shop can be activated if it can then it activates it
AutoActivate=1
;Max offline shop time in seconds Default: 43200 (12 hours)
ShopTimeLimit=43200
;Shop Cost item_id per hour Default 10000 adena
ShopCost=10000
;Shop Cost Item ID Default: adena (57)
ShopCostItemID=57
;Stay in game after loosing all requied items or endig private store (Default: off)
StayInGame=0
;Ip n Mac limit for users to prevent abuse
IpMacLimit=1
*/

COfflineShop g_OfflineShop;

void COfflineShop::Initialize()
{
	const TCHAR* section = _T("OfflineShop");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), FALSE, g_ConfigFile);
	
	m_AutoActivate = GetPrivateProfileInt(section, _T("AutoActivate"), FALSE, g_ConfigFile);
	m_ShopTimeLimit = GetPrivateProfileInt(section, _T("ShopTimeLimit"), 43200, g_ConfigFile);
	m_RequiredItemId = GetPrivateProfileInt(section, _T("RequiredItemId"), 0, g_ConfigFile);
	m_RequiredItemCount = GetPrivateProfileInt(section, _T("RequiredItemCount"), 0, g_ConfigFile);
	m_ConsumeRequiredItem = GetPrivateProfileInt(section, _T("ConsumeRequiredItem"), 0, g_ConfigFile);
	m_IpMacLimit = GetPrivateProfileInt(section, _T("IpMacLimit"), 1, g_ConfigFile);
	m_StayInGame = GetPrivateProfileInt(section, _T("StayInGame"), FALSE, g_ConfigFile);
	m_RequiredItemPriceId = GetPrivateProfileInt(section, _T("RequiredItemPriceId"), 0, g_ConfigFile);
	m_RequiredItemPriceCount = GetPrivateProfileInt(section, _T("RequiredItemPriceCount"), 0, g_ConfigFile);

	if(m_Enabled)
	{
		g_Log.Add(CLog::Blue, "[%s] ShopTimeLimit [%d] seconds. Required item count [%d] id[%d] AutoActivate[%d]", __FUNCTION__, m_ShopTimeLimit, m_RequiredItemCount, m_RequiredItemId, m_AutoActivate);
		
		//Func inside CIOSocket::Close
		BYTE OrgBytes1[5] = {0xE8, 0xCB, 0x57, 0xD9, 0xFF};
		g_HookManager.WriteCall(0x890A00, ManageCloseTimer, 0, OrgBytes1);
		//AuthRequestKickAccount
		BYTE OrgBytes2[9] = {0x48,  0x8B, 0x80, 0x78, 0x07, 0x00, 0x00, 0xFF, 0xD0};
		g_HookManager.WriteCall(0x588627, AuthKick, 4, OrgBytes2);
		//BuilderCmd_kick
		BYTE OrgBytes3[5] = {0xE8, 0x0C, 0xD4, 0x19, 0x00};
		g_HookManager.WriteCall(0x48112F, BuilderCmdKick, 0, OrgBytes3);
		//KickCharPacket
		g_HookManager.WriteCall(0x576290, KickCharPacket, 0);
		//User::Pause kick
		g_HookManager.WriteCall(0x80D4E2, KickCharPacket, 0);
		//ReplyPrivateStore Commit
		g_HookManager.WriteCall(0x57E690, KickCharPacket, 0);
		g_HookManager.WriteCall(0x57E6F7, KickCharPacket, 0);
		//ReplyMakeItem
		g_HookManager.WriteCall(0x561242, KickCharPacket, 0);
		
		//Func inside CIOSocket::Close
		BYTE OrgBytes4[10] = {0x41, 0x80, 0xBB, 0xA5, 0x0C, 0x00, 0x00, 0x00, 0x74, 0x4C};
		g_HookManager.WriteJump( 0x8909B5, CheckOfflineShopWhenExit, 5, OrgBytes4);
		//CL2StatusWnd::OnRefresh
		BYTE OrgBytes5[5] = {0xE8, 0x10, 0xC8, 0x19, 0x00};
		g_HookManager.WriteCall(0x6451BB, AddToStatusWindow, 0, OrgBytes5);
		//User::Tell
		BYTE OrgBytes6[5] = {0xE8, 0xF3, 0xD7, 0xFF, 0xFF};
		g_HookManager.WriteCall(0x82D6B8, PrivateMessageA, 0, OrgBytes6);
		BYTE OrgBytes7[5] = {0xE8, 0x70, 0xC2, 0xC0, 0xFF};
		g_HookManager.WriteCall(0x82D6DB, PrivateMessageB, 0, OrgBytes7);
	}
}

bool COfflineShop::IsOfflineShop(CUserSocket *pSocket)
{
	try
	{
		if(pSocket)
		{
			if(User *pUser = pSocket->GetUser())
			{
				if(pUser->pSD->activateOfflineShopTime)
				{
					return true;
				}
			}
		}
	}catch(...)
	{
		EXCEPTION_LOG;
	}
	return false;
}

void COfflineShop::RequestBuyRequiredItem(User *pUser)
{
	if(m_Enabled)
	{
		UINT currentTick = GetTickCount();
		if(pUser->pSocket->pED->itemActionTimestamp < currentTick || ((pUser->pSocket->pED->itemActionTimestamp - g_ItemPacketReuse) > currentTick))
		{
			pUser->pSocket->pED->itemActionTimestamp = currentTick + 2000;
			
			if(!pUser->IsNowTrade())
			{
				if(pUser->pSD->nStoreMode == 0)
				{
					if(m_RequiredItemPriceId && m_RequiredItemId)
					{
						if(m_RequiredItemPriceCount > 0)
						{
							if(!pUser->DeleteItemInInventory(m_RequiredItemPriceId, m_RequiredItemPriceCount))
							{
								pUser->pSocket->SendSystemMessage(L"You don't have enough required items to do this action!");
								return;
							}
						}

						pUser->AddItemToInventory(m_RequiredItemId, m_RequiredItemCount);
					}
				}else
				{
					pUser->pSocket->SendSystemMessage(L"You cannot do this action while having a private store!");
				}
			}else
			{
				pUser->pSocket->SendSystemMessage(L"You cannot do this action while trading!");
			}
		}
	}
}

void COfflineShop::RequestActivateOfflineShop(User *pUser)
{
	if(!m_Enabled)
	{
		pUser->SendSystemMessage(L"Offline Shops are not available at this time.");
		return;
	}
	if(pUser->pSD->nInPeaceZone)
	{
		if(pUser->pSD->nStoreMode == 3 || pUser->pSD->nStoreMode == 5 || pUser->pSD->nStoreMode == 1)
		{
			if(pUser->olympiadUser.GetState() == 0)
			{
				if( pUser->pSD->activateOfflineShopTime == 0 )
				{
					if(pUser->pSD->pExData->territoryData.territoryId[FORBIDDEN_OFFLINE_SHOP_ZONE] == 0)
					{
						if( m_IpMacLimit == 0 || g_OfflineSocketDB.GetHwidCount(pUser->pSocket) < m_IpMacLimit )
						{
							if(m_RequiredItemId > 0 && m_RequiredItemCount > 0)
							{
								if(!pUser->inventory.HaveItemByClassID(m_RequiredItemId, m_RequiredItemCount))
								{
									pUser->pSocket->SendSystemMessage(L"You don't have enough required item(s) to start Offline Shop.");
									return;
								}else if(m_ConsumeRequiredItem)
								{
									pUser->DeleteItemInInventory(m_RequiredItemId, m_RequiredItemCount);
								}
							}

							pUser->pSD->activateOfflineShopTime = _time32(NULL);
							CParty *pParty = pUser->GetParty();
							if(pParty)
							{
								pParty->LeaveParty(pUser);
							}
							pUser->olympiadUser.OnLeaveWorld();
							
							wstring wMsg = L"Your private store has succesfully been flagged as an offline shop and will remain active for ";
							int nHours = m_ShopTimeLimit; //seconds
							nHours /= 3600;
							WCHAR wStr[8];
							wMsg += _itow(nHours, wStr, 10);
							wMsg += L" hours.";
							pUser->SendSystemMessage((PWCHAR)wMsg.c_str());
							pUser->SetUserInfoChanged();
							pUser->SetCharInfoChanged();
						}else
							pUser->SendSystemMessage(L"You've reached offline shops limit.");
					}else
						pUser->SendSystemMessage(L"Using the offline shop is prohibited in this area.");
				}else
					pUser->SendSystemMessage(L"Your private store has already been flagged as an offline shop.");
			}else
				pUser->SendSystemMessage(L"Using the offline shop is prohibited when participing in Olympiad Games.");
		}else
			pUser->SendSystemMessage(L"You are not running a private store or private work shop.");
	}else
		pUser->SendSystemMessage(L"Using the offline shop is prohibited outside of town.");
}

void COfflineShop::AutoActivate(User *pUser)
{
	if(m_Enabled)
	{
		if(m_AutoActivate && pUser->pSD->nInPeaceZone)
		{
			if(pUser->pSD->nStoreMode == 3 || pUser->pSD->nStoreMode == 5 || pUser->pSD->nStoreMode == 1)
			{
				if(pUser->olympiadUser.GetState() == 0)
				{
					if( pUser->pSD->activateOfflineShopTime == 0 )
					{
						if(pUser->pSD->pExData->territoryData.territoryId[FORBIDDEN_OFFLINE_SHOP_ZONE] == 0)
						{
							if(g_OfflineSocketDB.GetHwidCount(pUser->pSocket) < m_IpMacLimit)
							{
								if(m_RequiredItemId > 0 && m_RequiredItemCount > 0)
								{
									if(!pUser->inventory.HaveItemByClassID(m_RequiredItemId, m_RequiredItemCount))
									{
										return;
									}else if(m_ConsumeRequiredItem)
									{
										pUser->DeleteItemInInventory(m_RequiredItemId, m_RequiredItemCount);
									}
								}

								pUser->pSD->activateOfflineShopTime = _time32(0);
								CParty *pParty = pUser->GetParty();
								if(pParty)
								{
									pParty->LeaveParty(pUser);
								}
								pUser->olympiadUser.OnLeaveWorld();
								pUser->SetCharInfoChanged();
							}
						}
					}
				}
			}
		}
	}
}


void COfflineShop::TimerExpired(User *pUser)
{
	guard;
	if(m_Enabled && pUser->ValidUser())
	{
		if(pUser->pSD->activateOfflineShopTime)
		{
			if(pUser->pSD->nInPeaceZone)
			{
				if(pUser->pSD->nStoreMode == 3 || pUser->pSD->nStoreMode == 5 || pUser->pSD->nStoreMode == 1)
				{
					UINT CurrentTime = _time32(0);
					if( (pUser->pSD->activateOfflineShopTime + m_ShopTimeLimit) < CurrentTime )
					{
						g_Log.Add(CLog::Blue, "[%s] User[%S] Shop Expired.", __FUNCTION__, pUser->pSD->wszName);
						pUser->pSD->activateOfflineShopTime = 0;
						g_OfflineSocketDB.StartCloseTimer(pUser->GetSocket());
					}
				}else
				{
					g_Log.Add(CLog::Blue, "[%s] User[%S] not meeting requirements - not in private store mode.", __FUNCTION__, pUser->pSD->wszName);
					pUser->pSD->activateOfflineShopTime = 0;
					pUser->SetUserInfoChanged();
					pUser->SetCharInfoChanged();
					g_OfflineSocketDB.StartCloseTimer(pUser->GetSocket());
				}
			}else
			{
				g_Log.Add(CLog::Blue, "[%s] User[%S] not meeting requirements - not in peace zone.", __FUNCTION__, pUser->pSD->wszName);
				pUser->pSD->activateOfflineShopTime = 0;
				g_OfflineSocketDB.StartCloseTimer(pUser->GetSocket());
			}
		}
	}
	unguard;
}

int COfflineShop::AuthKick(User *pUser)
{
	if(g_OfflineShop.IsOfflineShop(pUser->pSocket))
	{
		g_OfflineSocketDB.StartCloseTimer(pUser->pSocket);
	}

	typedef int (*f)(User*);
	return f(0x84C650L)(pUser);
}

void COfflineShop::KickCharPacket(CUserSocket *pSocket)
{
	if(g_OfflineShop.IsOfflineShop(pSocket))
	{
		g_OfflineSocketDB.StartCloseTimer(pSocket);
	}

	typedef void (*f)(CUserSocket*);
	f(0x85DEF0L)(pSocket);
}

void COfflineShop::BuilderCmdKick(CUserSocket *pSocket)
{
	if(g_OfflineShop.IsOfflineShop(pSocket))
	{
		if(g_OfflineSocketDB.StartCloseTimer(pSocket))
		{
			return;
		}
	}

	typedef int (*f)(CUserSocket*);
	f(0x61E540L)(pSocket);
}

bool COfflineShop::ManageCloseTimer(CIOObject *pObject, int nTimeSpan, int nTimerID)
{
	CUserSocket *pSocket = (CUserSocket*)pObject->pSocket;	
	if(User *pUser = pSocket->GetUser()->GetUser())
	{
		if(pUser->inCombatMode)
		{
			//Normal Execution
			typedef bool (*f)(CIOObject*, int, int);
			return f(0x6261D0L)(pObject, nTimeSpan, nTimerID);
		}else if(pUser->pSD->activateOfflineShopTime > 0)
		{
			if(pUser->pSD->nInPeaceZone && (pUser->pSD->nStoreMode == 3 || pUser->pSD->nStoreMode == 5 || pUser->pSD->nStoreMode == 1))
			{
				if(g_OfflineSocketDB.Add(pSocket, pObject))
				{
					g_Log.Add(CLog::Blue, "[%s] User[%S] is offline shopping.", __FUNCTION__, pUser->pSD->wszName);
					g_SocketDB.SetStatus(pSocket, CSocketData::OFFLINE_SHOPPING);
					return false;
				}
			}
		}
	}

	nTimeSpan = 100;
	typedef bool (*f)(CIOObject*, int, int);
	return f(0x6261D0L)(pObject, nTimeSpan, nTimerID);
}

void COfflineShop::AddToStatusWindow(LPVOID pWindow, int nLine, PWCHAR format, int nCount)
{
	typedef void (*f)(LPVOID, int, PWCHAR, int);
	//Our new info
	static PWCHAR newFormat = L"Offline Shops : %d";
	f(0x7E19D0L)(pWindow, nLine, newFormat, g_OfflineSocketDB.GetSize());
}

bool COfflineShop::PrivateMessageA(User *pUser, int nSenderDBID)
{
	typedef bool (*f)(User*, int); //User::IsInBlockList(User *pUser, int nSenderDBID);
	if(pUser->ValidUser())
	{
		if(pUser->pSD->activateOfflineShopTime)
		{
			User *pSender = g_UserDB.GetUserByDBID(nSenderDBID);
			if(pSender->ValidUser())
			{
				pSender->SendSystemMessage(L"Selected player is in offline shop mode and cannot receive any private messages.");
				pSender->pSD->nPrivMsgFlag = 1;
			}
			return true;
		}
	}
	return f(0x82AEB0L)(pUser, nSenderDBID);
}

void COfflineShop::PrivateMessageB(CUserSocket *pSocket, int nMsgID, PWCHAR wsName)
{
	if(pSocket->ValidUserSocket())
	{
		if(User *pUser = pSocket->GetUser())
		{
			if(pUser->pSD->nPrivMsgFlag)
			{
				pUser->pSD->nPrivMsgFlag = 0;
				return;
			}else
			{
				pSocket->SendSystemMessage_S(nMsgID, wsName);
			}
		}
	}
}

