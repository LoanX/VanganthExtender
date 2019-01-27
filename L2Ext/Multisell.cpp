#include "StdAfx.h"
#include "MultiSell.h"
#include "ObjectDB.h"

CMultiSellDB g_MultiSellDB;

CMultiSellEntry::CMultiSellEntry(wstring wLine)
{
	if(wLine.find(L"{{{") == 0)
	{
		//{{{[flamberge];1}};{{[crystal_c];573};{[crystal_d];2865}}};
		PARSE_STATE PState = MS_BEGIN;
		wstring wName;
		wstring wCount;
		for(int n=0;n<wLine.size();n++)
		{
			switch(PState)
			{
			case MS_BEGIN:
				if(wLine[n] == '{')
				{
					PState = MS_REWARD;
				}
				break;
			case MS_REWARD:
				if(wLine[n] == '[')
				{
					PState = MS_REWARD_NAME;
					break;
				}
				if(wLine[n] == '}')
				{
					PState = MS_PRICE;
					break;
				}
				break;
			case MS_REWARD_NAME:
				if(wLine[n] == ';')
				{
					PState = MS_REWARD_COUNT;
					break;
				}
				if(wLine[n] == ']')
					break;
				wName += wLine[n];
				break;
			case MS_REWARD_COUNT:
				if(wLine[n] == '}')
				{
					CMultiSellItem Item(g_ObjectDB.GetClassIdFromName(wName.c_str()), _wtoi(wCount.c_str()));
					wName = L"";
					wCount = L"";
					vReward.push_back(Item);
					PState = MS_REWARD;
					break;
				}
				wCount += wLine[n];
				break;
			case MS_PRICE:
				if(wLine[n] == '[')
				{
					PState = MS_PRICE_NAME;
					break;
				}
				if(wLine[n] == '}')
				{
					PState = MS_END;
					break;
				}
				break;
			case MS_PRICE_NAME:
				if(wLine[n] == ';')
				{
					PState = MS_PRICE_COUNT;
					break;
				}
				if(wLine[n] == ']')
					break;
				wName += wLine[n];
				break;
			case MS_PRICE_COUNT:
				if(wLine[n] == '}')
				{
					CMultiSellItem Item(g_ObjectDB.GetClassIdFromName(wName.c_str()), _wtoi(wCount.c_str()));
					wName = L"";
					wCount = L"";
					vPrice.push_back(Item);
					PState = MS_PRICE;
					break;
				}
				wCount += wLine[n];
				break;
			case MS_END:
				break;
			}
		}
	}
}
size_t CMultiSellEntry::GetPriceItemCount() 
{ 
	return vPrice.size(); 
};
size_t CMultiSellEntry::GetRewardItemCount() 
{ 
	return vReward.size(); 
};

CMultiSell::CMultiSell() : ID(0), is_dutyfree(false), is_show_all(false), keep_enchanted(false)
{

};

void CMultiSell::Begin(wstring wLine)
{
	//MultiSell_begin	[blackmerchant_weapon]	1
	wstringstream sstr;
	sstr << wLine;
	sstr >> wName;
	sstr >> wName; //real name
	sstr >> ID;
}
int CMultiSell::GetID()
{
	return ID;
}
size_t CMultiSell::GetEntryCount()
{
	return vEntry.size();
}
void CMultiSell::Clear()
{
	is_dutyfree = false;
	is_show_all = false;
	keep_enchanted = false;
	wName = L"";
	ID = 0;
	vEntry.clear();
}
void CMultiSell::SetOption(wstring wLine)
{
	if(wLine.find(L"is_dutyfree")==0)
	{
		wLine = CParser::Trim(wLine);
		size_t pos = wLine.find('=');
		wstring wOpt = wLine.substr(pos+1, 1);
		if(wOpt == L"1")
			is_dutyfree = true;
		else
			is_dutyfree = false;
	}else if(wLine.find(L"is_show_all")==0)
	{
		wLine = CParser::Trim(wLine);
		size_t pos = wLine.find('=');
		wstring wOpt = wLine.substr(pos+1, 1);
		if(wOpt == L"1")
			is_show_all = true;
		else
			is_show_all = false;
	}else if(wLine.find(L"keep_enchanted")==0)
	{
		wLine = CParser::Trim(wLine);
		size_t pos = wLine.find('=');
		wstring wOpt = wLine.substr(pos+1, 1);
		if(wOpt == L"1")
			keep_enchanted = true;
		else
			keep_enchanted = false;
	}
}
void CMultiSell::AddEntry(wstring wLine)
{
	CMultiSellEntry Entry(wLine);
	if(Entry.vReward.size() > 0)
	{
		vEntry.push_back(Entry);
/*///////////////////////////////////////////////////////////////////////
		//validate for adena mistakes
		INT64 AdenaPrice = 0;
		INT64 AdenaReward = 0;

		//price:
		for(vector<CMultiSellItem>::iterator Iter = Entry.vPrice.begin();Iter!=Entry.vPrice.end();Iter++)
		{
			CItem *pItem = CObjectDB::GetObject(Iter->ItemID)->CastItem();
			if(pItem)
			{
				int DefaultPrice = pItem->pII->nItemPrice;
				if(DefaultPrice < 1)
					DefaultPrice = 1;
				AdenaPrice += ( DefaultPrice * Iter->Count);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find item[%d]", __FUNCTION__, Iter->ItemID);
			}
		}

		//reward
		for(vector<CMultiSellItem>::iterator Iter = Entry.vReward.begin();Iter!=Entry.vReward.end();Iter++)
		{
			CItem *pItem = CObjectDB::GetObject(Iter->ItemID)->CastItem();
			if(pItem)
			{
				int DefaultPrice = pItem->pII->nItemPrice;
				if(DefaultPrice < 1)
					DefaultPrice = 1;
				AdenaReward += ( DefaultPrice * Iter->Count);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find item[%d]", __FUNCTION__, Iter->ItemID);
			}
		}

		if(AdenaReward > (AdenaPrice * 2))
		{
			wLine = CParser::Replace(wLine, L'\n', L'');
			g_Log.Add(CLog::Error, "[%s] Reward bigger than price!!", __FUNCTION__);
			g_Log.Add(CLog::Error, "%S", wLine.c_str());
		}
///////////////////////////////////////////////////////////////////////////*/
	}
}

void CMultiSellDB::Init()
{
	ReadData();
	g_HookManager.WriteCall(0x69293F, CMultiSellDB::SendList, 0);
	g_HookManager.WriteCall(0x692A37, CMultiSellDB::SendList, 0);
	g_HookManager.WriteCall(0x692B04, CMultiSellDB::SendList, 0);
	g_HookManager.WriteCall(0x692CA6, CMultiSellDB::SendList, 0);

	g_HookManager.WriteCall(0x868620, CMultiSellDB::MultiSellChoose, 0);
}

void CMultiSellDB::ReadData()
{
	wstring str = ReadFileW(L"..\\Script\\MultiSell.txt");
	wstringstream sstr;
	sstr << str;
	wstring wLine;
	CMultiSell MultiSell;
	while(getline(sstr, wLine))
	{
		if(wLine.find(L"//") == 0)
			continue;
		else if(wLine.find(L"MultiSell_begin") == 0)
		{
			MultiSell.Begin(wLine);
		}else if(wLine.find(L"is_dutyfree") == 0)
		{
			MultiSell.SetOption(wLine);
		}else if(wLine.find(L"is_show_all") == 0)
		{
			MultiSell.SetOption(wLine);
		}else if(wLine.find(L"keep_enchanted") == 0)
		{
			MultiSell.SetOption(wLine);
		}else if(wLine.find(L"{{{") == 0 )
		{
			MultiSell.AddEntry(wLine);
		}else if(wLine.find(L"MultiSell_end") == 0)
		{
			mData.insert(pair<int, CMultiSell>(MultiSell.GetID(), MultiSell));
			MultiSell.Clear();
		}
	}
	g_Log.Add(CLog::Blue, "[%s] Loaded [%d] MultiSell lists.", __FUNCTION__, mData.size());
}

int CMultiSellDB::ConvertEntry(UINT nEntryID, int nItemCount)
{
	UINT nConv = 0;
	nItemCount*=2;
	nConv = (((nEntryID % nItemCount)+1)/2)-1;
	
	return nConv;
}

bool CMultiSellDB::GetPriceAndReward(UINT listId, UINT entryId, INT32 count, LPINT lpPriceId, LPINT lpPriceCount, LPINT lpRewardId, LPINT lpRewardCount)
{
	guard;
	
	map<INT32, CMultiSell>::iterator it = mData.find(listId);
	if(it!=mData.end())
	{
		UINT entryIndex = ConvertEntry(entryId, (INT32)it->second.GetEntryCount());
		if(entryIndex < it->second.GetEntryCount())
		{
			CMultiSellEntry& entry = it->second.vEntry[entryIndex];
			for(INT32 n=0;n<entry.vPrice.size();n++)
			{
				if(n < 5)
				{
					lpPriceId[n] = entry.vPrice[n].GetID();
					lpPriceCount[n] = entry.vPrice[n].GetCount() * count;
				}
			}
			for(INT32 n=0;n<entry.vReward.size();n++)
			{
				if(n < 5)
				{
					lpRewardId[n] = entry.vReward[n].GetID();
					lpRewardCount[n] = entry.vReward[n].GetCount() * count;
				}
			}
			unguard;
			return true;
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Invalid entry index[%d] - entry[%d][%d] list[%d]", __FUNCTION__, entryIndex, entryId, it->second.GetEntryCount(), listId);
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Cannot find MultiSell! ListId[%d]", __FUNCTION__, listId);
	}

	unguard;
	return false;
}

bool CMultiSellDB::Validate(User *pUser, int nListID, int nItemIndex, int nAmount)
{
	if(pUser->ValidUser() && nAmount && nItemIndex >= 0)
	{
		try
		{
			map<int, CMultiSell>::iterator Iter = mData.find(nListID);
			if(Iter!=mData.end())
			{
				nItemIndex = ConvertEntry(nItemIndex, (int)Iter->second.GetEntryCount());
				if(nItemIndex < Iter->second.GetEntryCount())
				{
					//check price int overflow
					int nPriceSize = (int)Iter->second.vEntry[nItemIndex].GetPriceItemCount();
					for(int n=0;n<nPriceSize;n++)
					{
						INT64 nFinalPrice = ((INT64)nAmount) * ((INT64)Iter->second.vEntry[nItemIndex].vPrice[n].Count);
						if( nFinalPrice > (INT64)2100000000 || nFinalPrice < 0)
						{
							g_Log.Add(CLog::Error, "[%s] int overflow - multisell price amount[%d] entry[%d] item[%d]", __FUNCTION__, nAmount, nItemIndex, n);
							return false;
						}
					}
					//check reward int overflow
					int nRewardSize = (int)Iter->second.vEntry[nItemIndex].GetRewardItemCount();
					for(int n=0;n<nRewardSize;n++)
					{
						INT64 nFinalPrice = ((INT64)nAmount) * ((INT64)Iter->second.vEntry[nItemIndex].vReward[n].Count);
						if( nFinalPrice > (INT64)2100000000 || nFinalPrice < 0)
						{
							g_Log.Add(CLog::Error, "[%s] int overflow - multisell reward amount[%d] entry[%d] item[%d]", __FUNCTION__, nAmount, nItemIndex, n);
							return false;
						}
						CItem *pItem = pUser->GetInventory()->GetFirstItemByClassID(Iter->second.vEntry[nItemIndex].vReward[n].ItemID);
						if(pItem->IsValidItem() && (pItem->pSID->nConsumeType == CItem::consume_type_asset || pItem->pSID->nConsumeType == CItem::consume_type_stackable))
						{
							INT64 nOwnedAmount = (INT64)pItem->pSID->nItemAmount;
							nOwnedAmount += nFinalPrice;
							if(nOwnedAmount > (INT64)2100000000 || nOwnedAmount < 0)
							{
								g_Log.Add(CLog::Error, "[%s] int overflow - multisell reward + owned - amount[%d] entry[%d] item[%d]", __FUNCTION__, nAmount, nItemIndex, n);
								return false;
							}
						}
					}
					return true;

				}else
					g_Log.Add(CLog::Error, "[%s] Invalid ItemIndex[%d] in ListID[%d]", __FUNCTION__, nItemIndex, nListID);
			}else
				g_Log.Add(CLog::Error, "[%s] Invalid ListID[%d]", __FUNCTION__, nListID);
		}catch(...)
		{
			g_Log.Add(CLog::Blue, "[%s] Exception Detected - List[%d] ItemIndex[%d] Count[%d]", __FUNCTION__, nListID, nItemIndex, nAmount);
		}
	}
	return false;
}

void CMultiSellDB::MultiSellChoose(User *pUser, UINT groupId, int itemChooseId, int itemAmount)
{
	guard;
	if(pUser->pED->multiSellGroupId == groupId)
	{
		typedef void (*f)(User*, UINT, int, int);
		f(0x8489D0L)(pUser, groupId, itemChooseId, itemAmount);
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Invalid groupId[%d] sent[%d] User[%S]!", __FUNCTION__, groupId, pUser->pED->multiSellGroupId, pUser->pSD->wszName);
	}
	unguard;
}

void CMultiSellDB::SendList(CUserSocket *pSocket, const char* format, BYTE opCode, UINT groupId, UINT page, UINT finished, UINT totalPages, UINT groupSize, DWORD buffLen, LPBYTE lpBuff)
{
	guard;
	if(User *pUser = pSocket->pUser->SafeCastUser())
	{
		pUser->pED->multiSellGroupId = groupId;
	//	g_Log.Add(CLog::Blue, "[%s] Player[%S] multisell group[%d]", __FUNCTION__, pUser->pSD->wszName, groupId);
		pSocket->Send(format, opCode, groupId, page, finished, totalPages, groupSize, buffLen, lpBuff);
	}
	unguard;
}