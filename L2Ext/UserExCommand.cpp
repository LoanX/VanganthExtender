#include "stdafx.h"
#include "UserExCommand.h"
#include "OfflineShop.h"
#include "VIPSystem.h"
#include "VoteReward.h"
#include "HTMLCacheManager.h"
#include "PlayerAction.h"
#include "HtmlCache.h"
#include "AutoLoot.h"
#include "VisualArmor.h"
#include "Wedding.h"
#include "TvT.h"
#include "L2NetworkVote.h"
#include "Rebirth.h"

BOOL g_GoldBar = FALSE;
INT32 g_GoldBarItemId = 0;
INT32 g_GoldBarAdenaAmount = 0;
BOOL g_EnableOnlineCommand = 0;
BOOL g_EnableExpCommand = 0;

extern BOOL g_MenuHtml;

CUserExCommand g_UserExCommand;

CUserExCommand::CUserExCommand()
{
}

CUserExCommand::~CUserExCommand()
{

}

bool CUserExCommand::IsUserExCommand(WCHAR* wCommand)
{
	if(wCommand[0] == L'.')
	{
		return true;
	}
	return false;
}

void CUserExCommand::Bind(std::wstring name, UserExCommandCallback func)
{
	m_commands.insert(pair<wstring, UserExCommandCallback>(name, func));
}

void CUserExCommand::Init()
{
	Bind(L".offline_shop", UserExCommandActivateOfflineShop);
	Bind(L".vip", UserExCommandVip);
	Bind(L".vote", UserExCommandVote);
	Bind(L".exp", UserExCommandExp);
	Bind(L".autoloot", UserExCommandAutoLoot);
	Bind(L".menu", UserExCommandMenu);
	Bind(L".rune", UserExCommandRune);
	Bind(L".timeleft", UserExCommandLifeTimeItemsInfo);
	Bind(L".restart", UserExCommandRestart);
	Bind(L".goldbar", UserExCommandGoldBar);
	Bind(L".varmor", UserExCommandVisualArmor);
	Bind(L".engage", UserExCommandEngage);
	Bind(L".divorce", UserExCommandDivorce);
	Bind(L".gotolove", UserExCommandGoToLove);
	Bind(L".ping", UserExCommandPing);
	Bind(L".online", UserExCommandOnline);
	Bind(L".jointvt", UserExCommandJoinTvT);
	Bind(L".checkvote", UserExCommandCheckVote);
	Bind(L".showvote", UserExCommandShowVote);
	Bind(L".rebirth", UserExCommandRebirth);

	const TCHAR* sectionName = _T("GoldBar");
	g_GoldBar = GetPrivateProfileInt(sectionName, _T("Enabled"), 0, g_ConfigFile);
	g_GoldBarItemId = GetPrivateProfileInt(sectionName, _T("GoldBarItemId"), 0, g_ConfigFile);
	g_GoldBarAdenaAmount = GetPrivateProfileInt(sectionName, _T("AdenaAmount"), 0, g_ConfigFile);
	g_EnableOnlineCommand = GetPrivateProfileInt(_T("Features"), _T("EnableOnlineCommand"), 0, g_ConfigFile);
	g_EnableExpCommand = GetPrivateProfileInt(_T("Features"), _T("EnableExpCommand"), 0, g_ConfigFile);
}

bool UserExCommandRebirth(User *pUser, wstring params)
{
	guard;

	if(g_RebirthSystem.Enabled())
	{
		if(g_RebirthSystem.EnableCommand())
		{
			g_RebirthSystem.ShowMenu(pUser);
		}
	}

	unguard;
	return false;
}

bool UserExCommandShowVote(User *pUser, wstring params)
{
	guard;

	if(g_L2NetworkVote.Enabled())
	{
		if(g_L2NetworkVote.EnableCommand())
		{
			g_L2NetworkVote.OnShowVote(pUser);
		}
	}

	unguard;
	return false;
}

bool UserExCommandCheckVote(User *pUser, wstring params)
{
	guard;

	if(g_L2NetworkVote.Enabled())
	{
		if(g_L2NetworkVote.EnableCommand())
		{
			g_L2NetworkVote.OnCheck(pUser);
		}
	}

	unguard;
	return false;
}

bool UserExCommandJoinTvT(User *pUser, wstring params)
{
	guard;

	g_TvT.Register(pUser, 0);

	unguard;
	return false;
}


bool UserExCommandOnline(User *pUser, wstring params)
{
	guard;

	if(g_EnableOnlineCommand)
	{
		LPINT lpPlayingUsers = (LPINT)0x1557E10;
		LPINT lpMaxUsers = (LPINT)0x1557E1C;

		wstringstream onlineStream;
		onlineStream << L"There are " << (*lpPlayingUsers) << L" online players.";
		pUser->SendSystemMessage(onlineStream.str().c_str());
	}
	unguard;
	return false;
}

bool UserExCommandVisualArmor(User *pUser, wstring params)
{
	guard;
	if(g_VisualArmor.IsEnabled())
	{
		g_VisualArmor.ShowMenu(pUser);
	}
	unguard;
	return false;
}

bool UserExCommandGoldBar(User *pUser, wstring params)
{
	guard;
	if(g_GoldBar && g_GoldBarItemId)
	{
		if(params.find(L"buy") == 0)
		{
			if(pUser->inventory.GetAdenaAmount() >= g_GoldBarAdenaAmount)
			{
				if(pUser->inventory.CheckAddable(g_GoldBarItemId, 1))
				{
					pUser->DeleteItemInInventory(57, g_GoldBarAdenaAmount);
					pUser->AddItemToInventory(g_GoldBarItemId, 1);
					g_Logger.Add(L"User[%s] exchange adena[%d] into gold bar.", pUser->pSD->wszName, g_GoldBarAdenaAmount);
					pUser->SendSystemMessage(L"You have bought 1 Gold Bar.");
				}
			}else
			{
				pUser->SendSystemMessage(L"You don't have enough adena.");
			}
		}else if(params.find(L"sell") == 0)
		{
			if(pUser->inventory.HaveItemByClassID(g_GoldBarItemId, 1))
			{
				if(pUser->inventory.CheckAddable(57, g_GoldBarAdenaAmount))
				{
					pUser->DeleteItemInInventory(g_GoldBarItemId, 1);
					pUser->AddItemToInventory(57, g_GoldBarAdenaAmount);
					g_Logger.Add(L"User[%s] exchange gold bar into adena[%d]", pUser->pSD->wszName, g_GoldBarAdenaAmount);
				}else
				{
					pUser->SendSystemMessage(L"Cannot add more adena to your inventory!");
				}
			}else
			{
				pUser->SendSystemMessage(L"Cannot find a gold bar in your inventory.");
			}
		}else
		{
			pUser->SendSystemMessage(L"Gold Bar System:");
			wstringstream message;
			message << L".goldbar buy - converts " << g_GoldBarAdenaAmount << L" adena into 1 gold bar.";
			pUser->SendSystemMessage(message.str().c_str());
			wstringstream message2;
			message2 << L".goldbar sell - converts 1 gold bar into " << g_GoldBarAdenaAmount << L" adena.";
			pUser->SendSystemMessage(message2.str().c_str());
		}
	}

	unguard;
	return false;
}

bool CUserExCommand::Handle(User *pUser, WCHAR *wCommand)
{
	guard;
	bool ret = false;
	wstringstream data;
	wstringstream params;
	wstring command;
	wstring param;

	data << wCommand;
	data >> command;
	while(data >> param)
	{
		params << param << L" ";
	}

	map<wstring, UserExCommandCallback>::iterator it = m_commands.find(command);
	if(it!=m_commands.end())
	{
		UserExCommandCallback func = it->second;
		ret = func(pUser, params.str());
	}else
	{
		pUser->SendSystemMessage(L"Unknown user ex command!");
	}
	
	unguard;
	return ret;
}

bool UserExCommandPing(User *pUser, wstring params)
{
	guard;
	
	pUser->pSocket->Send("cdd", 0x87, 10, GetTickCount());

	unguard;
	return false;
}

bool UserExCommandGoToLove(User *pUser, wstring params)
{
	guard;
	g_Wedding.GoToLove(pUser);
	unguard;
	return false;
}

bool UserExCommandDivorce(User *pUser, wstring params)
{
	guard;
	g_Wedding.Divorce(pUser);
	unguard;
	return false;
}

bool UserExCommandEngage(User *pUser, wstring params)
{
	guard;
	wstringstream paramStream(params);
	wstring name;
	if(paramStream >> name)
	{
		if(User *pTarget = g_UserDB.GetUserByName(name))
		{
			g_Wedding.Engage(pUser, pTarget->pSD->wszName);
		}else
		{
			pUser->SendSystemMessage(L"Cannot find player with specified name!");
		}
	}
	unguard;
	return false;
}

bool UserExCommandRestart(User *pUser, wstring params)
{
	guard;

	if(pUser->pSD->nAlive == 0)
	{
		if(pUser->pSD->nYongmaType == 2)
		{
			pUser->GetOff(true);
		}
		
		if(params.find(L"village") == 0)
		{
			typedef void (*f)(User*, UINT);
			f(0x824120L)(pUser, RestartPointVillage);
		}else if(params.find(L"clanhall") == 0)
		{
			if(CPledge *pPledge = pUser->GetPledge())
			{
				if(pPledge->pSD->nAgitID > 0)
				{
					typedef void (*f)(User*, UINT);
					f(0x824120L)(pUser, RestartPointClanHall);
				}else
				{
					pUser->SendSystemMessage(L"Your clan doesn't have a clan hall!");
				}
			}else
			{
				pUser->SendSystemMessage(L"You are not in a clan!");
			}
		}else if(params.find(L"castle") == 0)
		{
			if(CPledge *pPledge = pUser->GetPledge())
			{
				if(pPledge->pSD->nCastleID > 0)
				{
					typedef void (*f)(User*, UINT);
					f(0x824120L)(pUser, RestartPointCastle);
				}else
				{
					pUser->SendSystemMessage(L"Your clan doesn't have a castle!");
				}
			}else
			{
				pUser->SendSystemMessage(L"You are not in a clan!");
			}
		}else
		{
			pUser->SendSystemMessage(L"Restart command parameters:");
			pUser->SendSystemMessage(L"village - To Village");
			pUser->SendSystemMessage(L"clanhall - To Clan Hall");
			pUser->SendSystemMessage(L"castle - To Castle");
			pUser->SendSystemMessage(L"Ex: .restart clanhall");
		}
	}else
	{
		pUser->SendSystemMessage(L"You have to be dead to be affected by this command.");
	}
	
	unguard;
	return false;
}

bool UserExCommandLifeTimeItemsInfo(User *pUser, wstring params)
{
	guard;
	
	pUser->SendLifeTimeItemsInfo();
	
	unguard;
	return false;
}

bool UserExCommandRune(User *pUser, wstring params)
{
	guard;
	
	pUser->SendRuneInfo();
	
	unguard;
	return false;
}

bool UserExCommandMenu(User *pUser, wstring params)
{
	guard;
	
	if(g_MenuHtml)
	{
		const WCHAR* pFile = 0;
		if(g_HtmlCache.IsEnabled())
		{
			pFile = g_HtmlCache.Get(L"menu.htm");
		}else
		{
			pFile = g_HTMLCacheManager.GetHTMLFile( L"menu.htm", pUser->pSD->bLang);
		}
		if(pFile)
		{
			PlayerAction::ShowHTML(pUser, L"menu.htm", pFile, 0);
		}
	}
	
	unguard;
	return false;
}

bool UserExCommandAutoLoot(User *pUser, wstring params)
{
	guard;
	
	if(g_AutoLoot.IsEnabledUserExCommand())
	{
		if(params.find(L"on") == 0)
		{
			if(!pUser->pED->autoLoot)
			{
				pUser->pED->autoLoot = TRUE;
				pUser->SendSystemMessage(L"Auto Loot mode have been enabled.");
			}
		}else if(params.find(L"off") == 0)
		{
			if(pUser->pED->autoLoot)
			{
				pUser->pED->autoLoot = FALSE;
				pUser->SendSystemMessage(L"Auto Loot mode have been disabled.");
			}
		}
	}

	unguard;
	return false;
}

bool UserExCommandExp(User *pUser, wstring params)
{
	guard;
	if(g_EnableExpCommand)
	{
		if(params.find(L"on") == 0)
		{
			if(pUser->pSD->pExData->stopIncExp)
			{
				pUser->pSD->pExData->stopIncExp = FALSE;
				pUser->SendSystemMessage(L"Stop Exp mode have been disabled.");
			}
		}else if(params.find(L"off") == 0)
		{
			if(!pUser->pSD->pExData->stopIncExp)
			{
				pUser->pSD->pExData->stopIncExp = TRUE;
				pUser->SendSystemMessage(L"Stop Exp mode have been enabled.");
			}
		}
	}

	unguard;
	return false;
}

bool UserExCommandVote(User *pUser, wstring params)
{
	guard;
	if(g_VoteReward.IsEnabled())
	{
		g_VoteReward.HandleCommand(pUser, params);
	}else
	{
		//2449	1	The Vote Reward System is currently disabled.	0	79	9B	B0	FF			0	0	0	0	0		none
		pUser->pSocket->SendSystemMessage(2449);
	}
	unguard;
	return false;
}

bool UserExCommandVip(User *pUser, wstring params)
{
	guard;
	if(g_VIPSystem.IsEnabled())
	{
		g_VIPSystem.HandleCommand(pUser, params);
	}else
	{
		//2446	1	The VIP System is currently disabled.	0	79	9B	B0	FF			0	0	0	0	0		none
		pUser->pSocket->SendSystemMessage(2446);
	}
	unguard;
	return false;
}

bool UserExCommandActivateOfflineShop(User *pUser, wstring params)
{
	guard;
	g_OfflineShop.RequestActivateOfflineShop(pUser);
	unguard;
	return false;
}