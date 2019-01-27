#include "stdafx.h"
#include "Auction.h"
#include "HtmlCache.h"
#include "Augmentation.h"
#include "DB.h"

CAuction g_Auction;

CAuction::CAuction() : m_UpdatePageRequest(FALSE), m_Enabled(FALSE), m_ExpireTimestamp(0)
{

}

CAuction::~CAuction()
{
}

void CAuction::Init()
{
	LoadINI();
	if(m_Enabled)
	{
		g_DB.RequestLoadAuctionItems();
	}
}

UINT CAuction::GetCategoryByName(wstring name)
{
	for(map<UINT, wstring>::iterator it = m_Categories.begin(); it!= m_Categories.end(); it++)
	{
		if(it->second == name)
		{
			return it->first;
		}
	}
	return 0;
}

/*
[Auction]
Enabled=1
;Duration of an auction in seconds (default: 172800 - 2 days)
AuctionTime=172800
AuctionFee=5000
;Max amount of non stackable items sold by 1 player at the same time
AuctionLimit=10
Categories=Weapons;0 Armors;1 Jawelry;2 Scrolls;3 Potions;4 Materials;5 Recipes;6 Etc;7
;List of items that can't be sold
ForbiddenItemList=57
*/

void CAuction::LoadINI()
{
	const TCHAR* section = _T("Auction");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), FALSE, g_ConfigFile);
	m_AuctionTime = GetPrivateProfileInt(section, _T("AuctionTime"), 172800, g_ConfigFile);
	m_AuctionFee = GetPrivateProfileInt(section, _T("AuctionFee"), 10000, g_ConfigFile);
	m_AuctionLimit = GetPrivateProfileInt(section, _T("AuctionLimit"), 10, g_ConfigFile);
	TCHAR temp[8190];
	if(GetPrivateProfileString(section, _T("Categories"), 0, temp, 8190, g_ConfigFile))
	{
		tstringstream tStream(Utils::ReplaceString(temp, L";", L" ", true));
		wstring name;
		UINT id = 0;
		while(tStream >> name >> id)
		{
			if(m_Categories.find(id) == m_Categories.end())
			{
				m_Categories.insert(pair<UINT, wstring>(id, name));
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Duplicated category[%S]!", __FUNCTION__, name.c_str());
			}
		}
	}
	wstringstream categoryList;
	for(map<UINT, wstring>::iterator it = m_Categories.begin();it != m_Categories.end(); it++)
	{
		if(it == m_Categories.begin())
		{
			categoryList << it->second;
		}else
		{
			categoryList << L";" << it->second;
		}
	}
	m_CategoryList = categoryList.str();
	if(GetPrivateProfileString(section, _T("ForbiddenItemList"), 0, temp, 8190, g_ConfigFile))
	{
		tstringstream tStream(temp);
		int itemId = 0;
		while(tStream >> itemId)
		{
			m_ForbiddenItems.push_back(itemId);
		}
	}

}

void CAuction::TimerExpired()
{
	guard;

	if(m_Enabled)
	{
		if(m_ExpireTimestamp < time(0))
		{
			m_ExpireTimestamp = time(0) + 10;
			m_UpdatePageRequest = TRUE;
		}
		if(m_UpdatePageRequest)
		{
			UpdatePages();
		}
	}

	unguard;
}

bool CAuction::RequestShowAuction(User *pUser, wstring wCategory, UINT page)
{
	guard;
	if(m_Enabled)
	{
		const WCHAR *wHtml = g_HtmlCache.Get(L"auction_show.htm");
		if(wHtml)
		{
			wstring html(wHtml);
			if(wCategory.size() > 0)
			{
				html = Utils::ReplaceString(html, L"<?category?>", wCategory, true);
				wstringstream categoryList;
				categoryList << wCategory;
				bool validCategory = false;
				for(map<UINT, wstring>::iterator it = m_Categories.begin();it != m_Categories.end(); it++)
				{
					if(it->second != wCategory)
					{
						categoryList << L";" << it->second;
					}else
					{
						validCategory = true;
					}
				}
				if(validCategory)
				{
					html = Utils::ReplaceString(html, L"<?category_list?>", categoryList.str(), true);
				}else
				{
					html = Utils::ReplaceString(html, L"<?category_list?>", m_CategoryList, true);
					g_Log.Add(CLog::Error, "[%s] Invalid Category[%S] - User[%S]!", __FUNCTION__, wCategory.c_str(), pUser->pSD->wszName);
				}
			}else
			{
				html = Utils::ReplaceString(html, L"<?category?>", L"", true);
				html = Utils::ReplaceString(html, L"<?category_list?>", m_CategoryList, true);
			}
			

			wstring itemTemplate;
			size_t startPos = html.find(L"<template_item>");
			if(startPos != wstring::npos)
			{
				size_t endPos = html.find(L"</template_item>", startPos);
				if(endPos != wstring::npos)
				{
					itemTemplate = html.substr(startPos + 15, (endPos - startPos - 15));
					
					//fill with items
					wstring lines;
					UINT totalPages = 0;
					m_Lock.Enter();
					totalPages = static_cast<UINT>(m_Pages.size());
					if(page < totalPages)
					{
						lines = m_Pages[page];
					}
					m_Lock.Leave();
					
					html = html.replace(startPos, (endPos + 16 - startPos), lines);

					TCHAR temp[16];
					_itow_s(page+1, temp, 16, 10);
					html = Utils::ReplaceString(html, L"<?current_page?>", temp, true);
					if(page > 0)
					{
						_itow_s(page-1, temp, 16, 10);
					}else
					{
						_itow_s(0, temp, 16, 10);
					}
					html = Utils::ReplaceString(html, L"<?previous_page?>", temp, true);
					_itow_s(page+1, temp, 16, 10);
					if(page < (totalPages - 1))
					{
						_itow_s(page+1, temp, 16, 10);
					}else
					{
						_itow_s(totalPages-1, temp, 16, 10);
					}
					html = Utils::ReplaceString(html, L"<?next_page?>", temp, true);
					_itow_s(totalPages, temp, 16, 10);
					html = Utils::ReplaceString(html, L"<?total_pages?>", temp, true);
					
				}
			}
			g_Log.Add(CLog::Blue, "[%s] size[%d]", __FUNCTION__, html.size());
		//	pUser->ShowBoard(html.c_str());
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find auction_show.htm !", __FUNCTION__);
		}
	}
	unguard;
	return false;
}

bool CAuction::RequestSetAuction(User *pUser, int price, int amount)
{
	guard;
	if(m_Enabled)
	{
		if(pUser->pED->auctionUser.pendingAuction == FALSE)
		{
			if(CItem *pItem = pUser->inventory.GetItemBySID(pUser->pED->auctionUser.itemServerId))
			{
				if(pItem->pSID->nItemID == pUser->pED->auctionUser.itemClassId)
				{
					if(amount > 0 && amount <= pItem->pSID->nItemAmount)
					{
						if(price > 0 && price < INT_MAX)
						{
							INT64 totalPrice = price * amount;
							if(totalPrice < INT_MAX)
							{
								if(m_AuctionFee > 0)
								{
									if(pUser->inventory.GetAdenaAmount() < m_AuctionFee)
									{
										pUser->SendSystemMessage(L"You don't have enough adena to pay auction fee.");
										RequestCancelAuction(pUser);
										unguard;
										return false;
									}
									pUser->DeleteItemInInventory(57, m_AuctionFee);
								}

								pUser->pED->auctionUser.pendingAuction = TRUE;
								g_DB.RequestDestroyItem(pItem, amount, pUser);
								g_DB.RequestCreateAuction(pUser->nDBID, pUser->pSD->wszName, pItem->pSID->nItemID, amount, pItem->pSID->nEnchantLevel, pItem->nAugmentationID, price, (time(0) + m_AuctionTime));
								g_Logger.Add(L"User[%s] - request create auction item[%d] amount[%d] enchant[%d] augmentation[%d] price[%d]", pUser->pSD->wszName, pItem->pSID->nItemID, amount, pItem->pSID->nEnchantLevel, pItem->nAugmentationID, price);
								unguard;
								return true;
							}else
							{
								//348	1	a,Incorrect item price.\0	5	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,error\0
								pUser->pSocket->SendSystemMessage(348);
								RequestCancelAuction(pUser);
							}
						}else
						{
							//348	1	a,Incorrect item price.\0	5	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,error\0
							pUser->pSocket->SendSystemMessage(348);
							RequestCancelAuction(pUser);
						}
					}else
					{
						//347	1	a,Incorrect item count.\0	5	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,error\0
						pUser->pSocket->SendSystemMessage(347);
						RequestCancelAuction(pUser);
					}
				}else
				{
					g_Log.Add(CLog::Error, "[%s] Item class id doesn't match[%d][%d] User[%S]!", __FUNCTION__, pUser->pED->auctionUser.itemClassId, pItem->pSID->nItemID, pUser->pSD->wszName);
					RequestCancelAuction(pUser);
				}
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find item[%d] in inventory[%S]", __FUNCTION__, pUser->pED->auctionUser.itemServerId, pUser->pSD->wszName);
			}
		}else
		{
			pUser->SendSystemMessage(L"You have to wait for pending operation to finish.");
		}
	}
	unguard;
	return false;
};

bool CAuction::RequestCancelAuction(User *pUser)
{
	guard;
	pUser->pED->auctionUser.createTimeout = 0;
	pUser->pED->auctionUser.itemClassId = 0;
	pUser->pED->auctionUser.itemServerId = 0;
	const WCHAR *wHtml = g_HtmlCache.Get(L"auction_cancel.htm");
	if(wHtml)
	{
		pUser->ShowBoard(wHtml);
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Cannot find auction_cancel.htm!", __FUNCTION__);
	}
	unguard;
	return false;
}

bool CAuction::RequestSelectAuctionItem(User *pUser)
{
	guard;
	if(m_Enabled)
	{
		if(m_AuctionFee > 0)
		{
			if(pUser->inventory.GetAdenaAmount() < m_AuctionFee)
			{
				pUser->SendSystemMessage(L"You don't have enough adena to pay auction fee.");
				unguard;
				return false;
			}
		}
		const WCHAR *wHtml = g_HtmlCache.Get(L"auction_create.htm");
		if(wHtml)
		{
			pUser->pED->auctionUser.createTimeout = time(0) + 60;
			pUser->pED->auctionUser.itemServerId = 0;
			pUser->ShowBoard(wHtml);
			pUser->SendItemList(true);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find auction_create.htm!", __FUNCTION__);
		}
	}
	unguard;
	return false;
}

bool CAuction::HandleSelectedItem(User *pUser, UINT itemServerId)
{
	guard;
	if(m_Enabled)
	{
		if(pUser->pED->auctionUser.createTimeout > time(0))
		{
			if(CItem *pItem = pUser->inventory.GetItemBySID(itemServerId))
			{
				//check if item isn't augmented etc
				if(pItem->nAugmentationID != 0 || pItem->nManaLeft != 0 || pItem->nProtectionTimeout > time(0))
				{
					if(pItem->nAugmentationID)
					{
						pUser->SendSystemMessage(L"You cannot trade augmented items!");
					}else if(pItem->nManaLeft)
					{
						pUser->SendSystemMessage(L"You cannot trade shadow items!");
					}else if(pItem->nProtectionTimeout)
					{
						pUser->SendSystemMessage(L"You cannot trade protected items!");
					}
					const WCHAR *wHtml = g_HtmlCache.Get(L"auction_select_invalid_item.htm");
					if(wHtml)
					{
						pUser->pED->auctionUser.createTimeout = time(0) + 60;
						pUser->pED->auctionUser.itemServerId = 0;
						pUser->ShowBoard(wHtml);
						pUser->SendItemList(true);
					}else
					{
						g_Log.Add(CLog::Error, "[%s] Cannot find auction_select_invalid_item.htm!", __FUNCTION__);
					}
					unguard;
					return false;
				}
				//check if item isnt blocked
				for(UINT n=0;n<m_ForbiddenItems.size();n++)
				{
					if(pItem->pSID->nItemID == m_ForbiddenItems[n])
					{
						const WCHAR *wHtml = g_HtmlCache.Get(L"auction_select_invalid_item.htm");
						if(wHtml)
						{
							pUser->pED->auctionUser.createTimeout = time(0) + 60;
							pUser->pED->auctionUser.itemServerId = 0;
							pUser->ShowBoard(wHtml);
							pUser->SendItemList(true);
						}else
						{
							g_Log.Add(CLog::Error, "[%s] Cannot find auction_select_invalid_item.htm!", __FUNCTION__);
						}
						unguard;
						return false;
					}
				}

				wstring wHtml = g_HtmlCache.Get(L"auction_set_item.htm");
				if(wHtml.size() > 0)
				{
					pUser->pED->auctionUser.itemServerId = itemServerId;
					pUser->pED->auctionUser.itemClassId = pItem->pSID->nItemID;
					Augmentation augment(pItem->nAugmentationID);
					wHtml = Utils::ReplaceString(wHtml, L"<?item_name?>", g_ItemDBEx.GetItemName(pItem->pSID->nItemID), true);
					wHtml = Utils::ReplaceString(wHtml, L"<?item_sa?>", g_ItemDBEx.GetItemSA(pItem->pSID->nItemID), true);
					wHtml = Utils::ReplaceString(wHtml, L"<?item_icon?>", g_ItemDBEx.GetItemIcon(pItem->pSID->nItemID), true);
					if(pItem->pSID->nEnchantLevel > 0)
					{
						wstringstream enchantStream;
						enchantStream << L"+" << pItem->pSID->nEnchantLevel;
						wHtml = Utils::ReplaceString(wHtml, L"<?item_enchant?>", enchantStream.str(), true);
					}else
					{
						wHtml = Utils::ReplaceString(wHtml, L"<?item_enchant?>", L"", true);
					}
					WCHAR wItemAmount[16];
					_itow_s(pItem->pSID->nItemAmount, wItemAmount, 16, 10);
					wHtml = Utils::ReplaceString(wHtml, L"<?item_max_amount?>", wItemAmount, true);
					wHtml = Utils::ReplaceString(wHtml, L"<?item_augmentation1?>", g_Augmentation.GetName(augment.Part.effectA), true);
					wHtml = Utils::ReplaceString(wHtml, L"<?item_augmentation2?>", g_Augmentation.GetName(augment.Part.effectB), true);

					pUser->ShowBoard(wHtml.c_str());
				}else
				{
					g_Log.Add(CLog::Error, "[%s] Cannot find auction_select_item.htm!", __FUNCTION__);
				}
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Invalid item[%d] User[%S]", __FUNCTION__, itemServerId, pUser->pSD->wszName);
			}
		}else
		{
			pUser->SendSystemMessage(L"You have to select an item in less than 60 seconds.");
		}
		pUser->pED->auctionUser.createTimeout = 0;
	}
	unguard;
	return false;
}

bool CAuction::Create(UINT sellerId, const WCHAR *sellerName, UINT auctionId, INT32 itemId, INT32 amount, INT32 enchant, UINT augmentation, INT32 price, UINT expireTime, bool loadOnly)
{
	guard;

	if(m_Enabled)
	{
		AuctionItem ai;
		ai.auctionTimeoutTime = expireTime;
		ai.itemAmount = amount;
		ai.itemAugmentation = Augmentation(augmentation);
		ai.itemClassId = itemId;
		ai.itemEnchant = enchant;
		ai.itemPrice = price;
		ai.sellerDatabaseId = sellerId;
		wcscpy_s(ai.sellerName, 25, sellerName);

		m_Lock.Enter();
		m_Items.insert(pair<UINT, AuctionItem>(auctionId, ai));
		if(expireTime > time(0))
		{
			m_UpdatePageRequest = TRUE;
		}
		m_Lock.Leave();

		if(!loadOnly)
		{
			if(User *pUser = g_UserDB.GetUserByDBID(sellerId))
			{
				pUser->pED->auctionUser.pendingAuction = FALSE;
				pUser->SendSystemMessage(L"The auction has been created.");
				RequestShowAuctionConfirmation(pUser, ai);
			}
		}
	}


	unguard;
	return false;
}

void CAuction::UpdatePages()
{
	guard;

	//update pages
	const WCHAR *wHtml = g_HtmlCache.Get(L"auction_show.htm");
	if(wHtml)
	{
		wstring html(wHtml);
		
		wstring itemTemplate;
		size_t startPos = html.find(L"<template_item>");
		if(startPos != wstring::npos)
		{
			size_t endPos = html.find(L"</template_item>", startPos);
			if(endPos != wstring::npos)
			{
				itemTemplate = html.substr(startPos + 15, (endPos - startPos - 15));
				
				//fill with items
				wstringstream pageStream;
				UINT lineCounter = 0;
				m_Lock.Enter();
				m_Pages.clear();
				m_UpdatePageRequest = FALSE;
				bool pageFlag = false;
				for(map<UINT, AuctionItem>::iterator it = m_Items.begin(); it!= m_Items.end(); it++)
				{
					AuctionItem& ai = it->second;
					if(ai.auctionTimeoutTime > time(0))
					{
						lineCounter++;					
						wstring line = itemTemplate;
						line = Utils::ReplaceString(line, L"<?item_icon?>", g_ItemDBEx.GetItemIcon(ai.itemClassId), true);
						if(ai.itemEnchant > 0)
						{
							wstringstream enchantStream;
							enchantStream << L"+" << ai.itemEnchant;
							line = Utils::ReplaceString(line, L"<?item_enchant?>", enchantStream.str(), true);
						}else
						{
							line = Utils::ReplaceString(line, L"<?item_enchant?>", L"", true);
						}
						line = Utils::ReplaceString(line, L"<?item_name?>", g_ItemDBEx.GetItemName(ai.itemClassId), true);
						line = Utils::ReplaceString(line, L"<?item_sa?>", g_ItemDBEx.GetItemSA(ai.itemClassId), true);
						TCHAR temp[16];
						_itow_s(ai.itemAmount, temp, 16, 10);
						line = Utils::ReplaceString(line, L"<?item_amount?>", temp, true);
						_itow_s(ai.itemPrice, temp, 16, 10);
						line = Utils::ReplaceString(line, L"<?item_price?>", temp, true);
						line = Utils::ReplaceString(line, L"<?seller_name?>", ai.sellerName, true);
						_itow_s(it->first, temp, 16, 10);
						line = Utils::ReplaceString(line, L"<?auction_id?>", temp, true);

						UINT expireTime = ai.auctionTimeoutTime - time(0);
						UINT days = expireTime / 86400;
						expireTime = expireTime % 86400;
						UINT hours = expireTime / 3600;
						expireTime = expireTime % 3600;
						UINT minutes = expireTime / 60;
						wstringstream expireStream;
						// 6d2h3m
						if(days > 0)
						{
							expireStream << days << L"d";
						}
						if(hours > 0)
						{
							expireStream << hours << L"h";
						}
						if(minutes)
						{
							expireStream << minutes << L"m";
						}
						wstring expire = expireStream.str();
						if(expire.size() == 0)
						{
							expire = L"Expired";
						}

						line = Utils::ReplaceString(line, L"<?expire_time?>", expire, true);

						pageStream << line << endl;
						pageFlag = true;
						if((lineCounter % 10) == 0)
						{
							m_Pages.push_back(pageStream.str());
							pageStream.str(L"");
							pageFlag = false;
						}
					}
				}
				if(pageFlag)
				{
					m_Pages.push_back(pageStream.str());
				}
				m_Lock.Leave();
			}
		}
	}
	unguard;
}

bool CAuction::RequestSetBuyItem(User *pUser, UINT auctionId)
{
	guard;
	if(m_Enabled)
	{
		if(auctionId > 0)
		{
			wstring wHtml = g_HtmlCache.Get(L"auction_buy_item.htm");
			if(wHtml.size() > 0)
			{
				AuctionItem ai;
				memset(&ai, 0, sizeof(AuctionItem));
				m_Lock.Enter();
				map<UINT, AuctionItem>::iterator it = m_Items.find(auctionId);
				if(it != m_Items.end())
				{
					memcpy(&ai, &it->second, sizeof(AuctionItem));
				}
				m_Lock.Leave();

				if(ai.itemClassId > 0 && ai.auctionTimeoutTime > time(0))
				{
					wHtml = Utils::ReplaceString(wHtml, L"<?item_name?>", g_ItemDBEx.GetItemName(ai.itemClassId), true);
					wHtml = Utils::ReplaceString(wHtml, L"<?item_sa?>", g_ItemDBEx.GetItemSA(ai.itemClassId), true);
					wHtml = Utils::ReplaceString(wHtml, L"<?item_icon?>", g_ItemDBEx.GetItemIcon(ai.itemClassId), true);
					if(ai.itemEnchant > 0)
					{
						wstringstream enchantStream;
						enchantStream << L"+" << ai.itemEnchant;
						wHtml = Utils::ReplaceString(wHtml, L"<?item_enchant?>", enchantStream.str(), true);
					}else
					{
						wHtml = Utils::ReplaceString(wHtml, L"<?item_enchant?>", L"", true);
					}
					WCHAR wItemAmount[16];
					_itow_s(ai.itemAmount, wItemAmount, 16, 10);
					wHtml = Utils::ReplaceString(wHtml, L"<?item_max_amount?>", wItemAmount, true);
					WCHAR wItemPrice[16];
					_itow_s(ai.itemPrice, wItemPrice, 16, 10);
					wHtml = Utils::ReplaceString(wHtml, L"<?item_price?>", wItemPrice, true);
					WCHAR wAuctionId[16];
					_itow_s(auctionId, wAuctionId, 16, 10);
					wHtml = Utils::ReplaceString(wHtml, L"<?auction_id?>", wAuctionId, true);

					pUser->ShowBoard(wHtml.c_str());
				}else
				{
					pUser->SendSystemMessage(L"The auction doesn't exist anymore.");
				}
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find auction_buy_item.htm!", __FUNCTION__);
			}
		}
	}
	unguard;
	return false;
}

bool CAuction::RequestConfirmBuy(User *pUser, UINT auctionId, INT32 amount)
{
	guard;
	if(m_Enabled)
	{
		if(auctionId > 0)
		{
			if(amount > 0)
			{
				wstring wHtml = g_HtmlCache.Get(L"auction_buy_item_confirm.htm");
				if(wHtml.size() > 0)
				{
					AuctionItem ai;
					memset(&ai, 0, sizeof(AuctionItem));

					m_Lock.Enter();
					map<UINT, AuctionItem>::iterator it = m_Items.find(auctionId);
					if(it != m_Items.end())
					{
						ai = it->second;
					}
					m_Lock.Leave();

					if(ai.itemClassId > 0 && ai.auctionTimeoutTime > time(0))
					{
						if(amount <= ai.itemAmount)
						{
							INT64 totalPrice = amount * ai.itemPrice;
							if(totalPrice > 0 && totalPrice < INT_MAX)
							{
								WCHAR wTotalPrice[16];
								_itow_s(totalPrice, wTotalPrice, 16, 10);
								wHtml = Utils::ReplaceString(wHtml, L"<?total_price?>", wTotalPrice, true);
								WCHAR wBuyAmount[16];
								_itow_s(amount, wBuyAmount, 16, 10);
								wHtml = Utils::ReplaceString(wHtml, L"<?buy_amount?>", wBuyAmount, true);

								wHtml = Utils::ReplaceString(wHtml, L"<?item_name?>", g_ItemDBEx.GetItemName(ai.itemClassId), true);
								wHtml = Utils::ReplaceString(wHtml, L"<?item_sa?>", g_ItemDBEx.GetItemSA(ai.itemClassId), true);
								wHtml = Utils::ReplaceString(wHtml, L"<?item_icon?>", g_ItemDBEx.GetItemIcon(ai.itemClassId), true);
								if(ai.itemEnchant > 0)
								{
									wstringstream enchantStream;
									enchantStream << L"+" << ai.itemEnchant;
									wHtml = Utils::ReplaceString(wHtml, L"<?item_enchant?>", enchantStream.str(), true);
								}else
								{
									wHtml = Utils::ReplaceString(wHtml, L"<?item_enchant?>", L"", true);
								}
								WCHAR wItemAmount[16];
								_itow_s(ai.itemAmount, wItemAmount, 16, 10);
								wHtml = Utils::ReplaceString(wHtml, L"<?item_max_amount?>", wItemAmount, true);
								WCHAR wItemPrice[16];
								_itow_s(ai.itemPrice, wItemPrice, 16, 10);
								wHtml = Utils::ReplaceString(wHtml, L"<?item_price?>", wItemPrice, true);
								WCHAR wAuctionId[16];
								_itow_s(auctionId, wAuctionId, 16, 10);
								wHtml = Utils::ReplaceString(wHtml, L"<?auction_id?>", wAuctionId, true);

								pUser->ShowBoard(wHtml.c_str());
							}else
							{
								pUser->SendSystemMessage(L"You're trying to buy too many items!");
							}
						}else
						{
							//347	1	a,Incorrect item count.\0	5	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,error\0
							pUser->pSocket->SendSystemMessage(347);
							RequestSetBuyItem(pUser, auctionId);
						}
					}else
					{
						pUser->SendSystemMessage(L"The auction doesn't exist anymore.");
					}
				}else
				{
					g_Log.Add(CLog::Error, "[%s] Cannot find auction_buy_item_confirm.htm!", __FUNCTION__);
				}
			}else
			{
				//347	1	a,Incorrect item count.\0	5	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,error\0
				pUser->pSocket->SendSystemMessage(347);
			}
		}
	}
	unguard;
	return false;
}

bool CAuction::RequestBuy(User *pUser, UINT auctionId, INT32 amount)
{
	guard;
	if(m_Enabled)
	{
		if(auctionId > 0)
		{
			if(amount > 0)
			{
				wstring wHtml = g_HtmlCache.Get(L"auction_buy_item_done.htm");
				if(wHtml.size() > 0)
				{
					AuctionItem ai;
					memset(&ai, 0, sizeof(AuctionItem));

					bool updateAmount = false;
					bool noAuction = false;
					bool notEnoughAdena = false;
					bool notEnoughSpaceInInventory = false;
					bool notEnoughItemsLeft = false;
					INT64 totalPrice = 0;
					m_Lock.Enter();
					map<UINT, AuctionItem>::iterator it = m_Items.find(auctionId);
					if(it != m_Items.end())
					{
						memcpy(&ai, &it->second, sizeof(AuctionItem));
						if(ai.auctionTimeoutTime > time(0))
						{
							if(pUser->inventory.CheckAddable(ai.itemClassId, amount))
							{
								totalPrice = ai.itemPrice * amount;
								if(totalPrice <= pUser->inventory.GetAdenaAmount())
								{
									if(amount <= ai.itemAmount)
									{
										updateAmount = true;
										ai.itemAmount -= amount;
										it->second.itemAmount = ai.itemAmount;
										if(ai.itemAmount == 0)
										{
											m_Items.erase(it);
										}
									}else
									{
										notEnoughItemsLeft = true;
									}
								}else
								{
									notEnoughAdena = true;
								}
							}else
							{
								notEnoughSpaceInInventory = true;
							}
						}else
						{
							noAuction = true;
						}
					}else
					{
						noAuction = true;
					}
					m_Lock.Leave();

					if(noAuction)
					{
						pUser->SendSystemMessage(L"The auction doesn't exist anymore.");
						RequestShowAuction(pUser, L"", 0);
						unguard;
						return false;
					}
					if(notEnoughItemsLeft)
					{
						pUser->SendSystemMessage(L"We're sorry there are not enough items left in the auction.");
						RequestSetBuyItem(pUser, auctionId);
						unguard;
						return false;
					}
					if(notEnoughAdena)
					{
						pUser->SendSystemMessage(L"You've got not enough adena in your inventory.");
						if(amount > 1)
						{
							RequestSetBuyItem(pUser, auctionId);
						}
						unguard;
						return false;
					}
					if(notEnoughSpaceInInventory)
					{
						pUser->SendSystemMessage(L"You've got not enough space in your inventory.");
						unguard;
						return false;
					}

					if(updateAmount)
					{
						pUser->DeleteItemInInventory(57, totalPrice);
						pUser->AddItemToInventory(ai.itemClassId, amount, false, ai.itemEnchant, 0, 0);
						g_Logger.Add(L"User[%s] bought auction[%d] seller[%s][%d] item[%d] amount[%d]", pUser->pSD->wszName, auctionId, ai.sellerName, ai.sellerDatabaseId, ai.itemClassId, amount);
						if(ai.itemAmount > 0)
						{
							g_DB.RequestModdifyAuctionAmount(auctionId, ai.itemAmount);
						}else
						{
							g_DB.RequestDeleteAuction(auctionId);
						}
						m_UpdatePageRequest = TRUE;

						g_DB.RequestAuctionAddPaymentJob(ai.sellerDatabaseId, 57, totalPrice, ai.itemClassId, amount, pUser->pSD->wszName);
						if(User *pSeller = g_UserDB.GetUserByDBID(ai.sellerDatabaseId))
						{
							//2458	1	a,You've just sold $s1 $s2 for $s3 adena through Auction House.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							CSystemMessage msg(2458);
							msg.AddNumber(amount);
							msg.AddItem(ai.itemClassId);
							msg.AddNumber(totalPrice);
							pSeller->SendSystemMessage(&msg);
						}

						WCHAR wTotalPrice[16];
						_itow_s(totalPrice, wTotalPrice, 16, 10);
						wHtml = Utils::ReplaceString(wHtml, L"<?total_price?>", wTotalPrice, true);
						WCHAR wBuyAmount[16];
						_itow_s(amount, wBuyAmount, 16, 10);
						wHtml = Utils::ReplaceString(wHtml, L"<?buy_amount?>", wBuyAmount, true);

						wHtml = Utils::ReplaceString(wHtml, L"<?item_name?>", g_ItemDBEx.GetItemName(ai.itemClassId), true);
						wHtml = Utils::ReplaceString(wHtml, L"<?item_sa?>", g_ItemDBEx.GetItemSA(ai.itemClassId), true);
						wHtml = Utils::ReplaceString(wHtml, L"<?item_icon?>", g_ItemDBEx.GetItemIcon(ai.itemClassId), true);
						if(ai.itemEnchant > 0)
						{
							wstringstream enchantStream;
							enchantStream << L"+" << ai.itemEnchant;
							wHtml = Utils::ReplaceString(wHtml, L"<?item_enchant?>", enchantStream.str(), true);
						}else
						{
							wHtml = Utils::ReplaceString(wHtml, L"<?item_enchant?>", L"", true);
						}
						WCHAR wItemAmount[16];
						_itow_s(ai.itemAmount, wItemAmount, 16, 10);
						wHtml = Utils::ReplaceString(wHtml, L"<?item_max_amount?>", wItemAmount, true);
						WCHAR wItemPrice[16];
						_itow_s(ai.itemPrice, wItemPrice, 16, 10);
						wHtml = Utils::ReplaceString(wHtml, L"<?item_price?>", wItemPrice, true);
						WCHAR wAuctionId[16];
						_itow_s(auctionId, wAuctionId, 16, 10);
						wHtml = Utils::ReplaceString(wHtml, L"<?auction_id?>", wAuctionId, true);

						pUser->ShowBoard(wHtml.c_str());
					}
				}else
				{
					g_Log.Add(CLog::Error, "[%s] Cannot find auction_buy_item_done.htm!", __FUNCTION__);
				}
			}else
			{
				//347	1	a,Incorrect item count.\0	5	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,error\0
				pUser->pSocket->SendSystemMessage(347);
			}
		}
	}
	unguard;
	return false;
}

void CAuction::HandlePaymentRequest(User *pUser, UINT jobId, INT32 priceId, INT32 priceAmount, INT32 itemId, INT32 amount, const WCHAR* buyer)
{
	guard;

	if(pUser->ValidUser())
	{
		if(pUser->inventory.CheckAddable(priceId, priceAmount))
		{
			if(pUser->IsNowTrade() == false && pUser->pSD->nStoreMode == 0)
			{
				pUser->PlaySound(L"ItemSound.quest_finish", 1, pUser->nObjectID, pUser->pSD->Pos.x, pUser->pSD->Pos.y, pUser->pSD->Pos.z);
				//2459	1	a,Incomming payment $s1 $s2 from $s3 for $s4 $s5 sold through Auction House.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				CSystemMessage msg(2459);
				msg.AddNumber(priceAmount);
				msg.AddItem(priceId);
				msg.AddText(buyer);
				msg.AddNumber(amount);
				msg.AddItem(itemId);
				pUser->SendSystemMessage(&msg);
				pUser->AddItemToInventory(priceId, priceAmount);
				g_Logger.Add(L"Auction House payment[%d][%d] for user[%s] job[%d] item[%d][%d]", priceId, priceAmount, pUser->pSD->wszName, jobId, itemId, amount);
				g_DB.RequestAuctionPaymentDone(pUser->nDBID, jobId);
			}
		}else
		{
			pUser->SendSystemMessage(L"Auction House", L"Please make sure you've got enough space in inventory for the auction payment.");
		}
	}

	unguard;
}

void CAuction::RequestShowAuctionConfirmation(User *pUser, AuctionItem& ai)
{
	guard;
	if(m_Enabled)
	{
		wstring wHtml = g_HtmlCache.Get(L"auction_create_done.htm");
		if(wHtml.size() > 0)
		{
			UINT totalPrice = ai.itemPrice * ai.itemAmount;
			WCHAR wTotalPrice[16];
			_itow_s(totalPrice, wTotalPrice, 16, 10);
			wHtml = Utils::ReplaceString(wHtml, L"<?total_price?>", wTotalPrice, true);
			WCHAR wBuyAmount[16];
			_itow_s(ai.itemAmount, wBuyAmount, 16, 10);
			wHtml = Utils::ReplaceString(wHtml, L"<?buy_amount?>", wBuyAmount, true);

			wHtml = Utils::ReplaceString(wHtml, L"<?item_name?>", g_ItemDBEx.GetItemName(ai.itemClassId), true);
			wHtml = Utils::ReplaceString(wHtml, L"<?item_sa?>", g_ItemDBEx.GetItemSA(ai.itemClassId), true);
			wHtml = Utils::ReplaceString(wHtml, L"<?item_icon?>", g_ItemDBEx.GetItemIcon(ai.itemClassId), true);
			if(ai.itemEnchant > 0)
			{
				wstringstream enchantStream;
				enchantStream << L"+" << ai.itemEnchant;
				wHtml = Utils::ReplaceString(wHtml, L"<?item_enchant?>", enchantStream.str(), true);
			}else
			{
				wHtml = Utils::ReplaceString(wHtml, L"<?item_enchant?>", L"", true);
			}
			WCHAR wItemAmount[16];
			_itow_s(ai.itemAmount, wItemAmount, 16, 10);
			wHtml = Utils::ReplaceString(wHtml, L"<?item_max_amount?>", wItemAmount, true);
			WCHAR wItemPrice[16];
			_itow_s(ai.itemPrice, wItemPrice, 16, 10);
			wHtml = Utils::ReplaceString(wHtml, L"<?item_price?>", wItemPrice, true);

			UINT expireTime = ai.auctionTimeoutTime - time(0);
			UINT days = expireTime / 86400;
			expireTime = expireTime % 86400;
			UINT hours = expireTime / 3600;
			expireTime = expireTime % 3600;
			UINT minutes = expireTime / 60;
			wstringstream expireStream;
			// 1d23h55m
			if(days > 0)
			{
				expireStream << days << L"d";
			}
			if(hours > 0)
			{
				expireStream << hours << L"h";
			}
			if(minutes)
			{
				expireStream << minutes << L"m";
			}
			wstring expire = expireStream.str();
			if(expire.size() == 0)
			{
				expire = L"Expired";
			}

			wHtml = Utils::ReplaceString(wHtml, L"<?expire_time?>", expire, true);

			pUser->ShowBoard(wHtml.c_str());
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find auction_create_done.htm!", __FUNCTION__);
		}
	}
	unguard;
}