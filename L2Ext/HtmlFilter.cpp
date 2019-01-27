#include "stdafx.h"
#include "HtmlFilter.h"

CHtmlFilter g_HtmlFilter;

CHtmlFilter::CHtmlFilter()
{
}

CHtmlFilter::~CHtmlFilter()
{
}

void CHtmlFilter::LoadINI()
{
	const TCHAR* section = _T("HtmlFilter");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), FALSE, g_ConfigFile);
}

void CHtmlFilter::Init()
{
	LoadINI();
	if(m_Enabled)
	{
		g_Log.Add(CLog::Blue, "[%s] Feature is enabled.", __FUNCTION__);
		BYTE lpMovEax1[5] = { 0xB8, 0x01, 0x00, 0x00, 0x00 };
		g_HookManager.WriteMemory(0x894424, lpMovEax1, 5);
		g_HookManager.WriteMemory(0x89497C, lpMovEax1, 5);
		UINT64 ampedAddr = (UINT64)GetModuleHandle(_T("AmpeDx64.dll"));
		if(ampedAddr)
		{
			//restore amped html check hooks
			BYTE lpData1[7] = {0x44, 0x8B, 0x8B, 0xC8, 0x0C, 0x00, 0x00};
			g_HookManager.WriteMemory(0x80A116, lpData1, 7);
			BYTE lpData2[7] = {0x45, 0x33, 0xC9, 0x45, 0x8D, 0x41, 0x0F};
			g_HookManager.WriteMemory(0x60DFAE, lpData2, 7);
			BYTE lpData3[7] = {0x48, 0x8D, 0x0D, 0x1C, 0x7B, 0x6F, 0x06};
			g_HookManager.WriteMemory(0x89441D, lpData3, 7);
			BYTE lpData4[7] = {0x48, 0x8D, 0x0D, 0xC4, 0x75, 0x6F, 0x06};
			g_HookManager.WriteMemory(0x894975, lpData4, 7);
			BYTE lpData5[15] = {0xC7, 0x44, 0x24, 0x30, 0x4A, 0x00, 0x00, 0x00, 0x48, 0x8D, 0x05, 0xEE, 0x38, 0x20, 0x00};
			g_HookManager.WriteMemory(0x89444B, lpData5, 15);
			BYTE lpData6[13] = { 0xC7, 0x44, 0x24, 0x30, 0xB1, 0x00, 0x00, 0x00, 0x48, 0x89, 0x6C, 0x24, 0x28 };
			g_HookManager.WriteMemory(0x8949A7, lpData6, 13);
		}
	}
}

void CHtmlFilter::OnSendHtml(User *pUser, const WCHAR *wFileName, const WCHAR *wHtml, UINT itemClassId)
{
	guard;

	if(m_Enabled)
	{
		bool done = false;
		pUser->pED->sentHtmlLock.Enter();
		try
		{
			//check if exists
			for(UINT n=0;n<pUser->pED->sentHtml.size();n++)
			{
				SentHtml& sh = pUser->pED->sentHtml[n];
				if(sh.classId == itemClassId)
				{
					//copy name and html data
					try
					{
						if(wFileName)
						{
							wcscpy_s(sh.wName, 260, wFileName);
						}else
						{
							memset(sh.wName, 0, sizeof(sh.wName));
						}
					}catch(...)
					{
						g_Log.Add(CLog::Error, "[%s] Handled exception while copying html name - user[%S]!", __FUNCTION__, pUser->pSD->wszName);
					}
					try
					{
						if(wHtml)
						{
							wcscpy_s(sh.wHtml, 16382, wHtml);
						}else
						{
							memset(sh.wHtml, 0, sizeof(sh.wHtml));
						}
					}catch(...)
					{
						g_Log.Add(CLog::Error, "[%s] Handled exception while copying html data - user[%S]!", __FUNCTION__, pUser->pSD->wszName);
					}
					done = true;
					break;
				}
			}
			if(!done)
			{
				SentHtml sh;
				sh.classId = itemClassId;
				//copy name and html data
				try
				{
					if(wFileName)
					{
						wcscpy_s(sh.wName, 260, wFileName);
					}else
					{
						memset(sh.wName, 0, sizeof(sh.wName));
					}
				}catch(...)
				{
					g_Log.Add(CLog::Error, "[%s] Handled exception while copying html name - user[%S]!", __FUNCTION__, pUser->pSD->wszName);
				}
				try
				{
					if(wHtml)
					{
						wcscpy_s(sh.wHtml, 16382, wHtml);
					}else
					{
						memset(sh.wHtml, 0, sizeof(sh.wHtml));
					}
				}catch(...)
				{
					g_Log.Add(CLog::Error, "[%s] Handled exception while copying html data - user[%S]!", __FUNCTION__, pUser->pSD->wszName);
				}
				pUser->pED->sentHtml.push_back(sh);
			}
		}catch(...)
		{
			EXCEPTION_LOG;
		}
		pUser->pED->sentHtmlLock.Leave();
	}

	unguard;
}

UINT CHtmlFilter::GetClassId(const WCHAR* wBypass)
{
	try
	{
		if(wBypass)
		{
			//find # at the end of the bypass
			size_t len = wcslen(wBypass);
			UINT classId = 0;
			bool numberStart = false;
			for(UINT n=0;n<len;n++)
			{
				if(numberStart)
				{
					classId = _wtoi(&wBypass[n]);
					break;
				}
				if(wBypass[n] == L'#')
				{
					numberStart = true;
				}
			}
			return classId;
		}
	}catch(...)
	{
		EXCEPTION_LOG;
	}
	return 0;
}

bool CHtmlFilter::IsValid(User *pUser, const WCHAR *wBypass, bool link, bool tutorialHtml)
{
	guard;
	bool valid = false;
	if(m_Enabled && wBypass)
	{
		//bypass -h auction_set_item?amount= $item_amount &price= $item_price
		UINT classId = GetClassId(wBypass);

		if(classId > 0 && !link)
		{
			if(pUser->inventory.HaveItemByClassID(classId, 1) == false)
			{
				g_Log.Add(CLog::Error, "[%s] User[%S] has no item[%d] in inventory - tried to use[%S]!", __FUNCTION__, pUser->pSD->wszName, classId, wBypass);
				unguard;
				return false;
			}
		}

		if(tutorialHtml)
		{
			classId = -1;
		}



		pUser->pED->sentHtmlLock.Enter();
		for(UINT n=0;n<pUser->pED->sentHtml.size();n++)
		{
			SentHtml& sh = pUser->pED->sentHtml[n];
			if(sh.classId == classId || (link && sh.classId == 0))
			{
				if(const WCHAR* wFound = wcsstr(sh.wHtml, wBypass))
				{
					valid = true;
					break;
				}else
				{
					HtmlBypass hb;
					ParseBypass(wBypass, hb, link);

					if(hb.name == L"manor_menu_select")
					{
						valid = true;
					}else if(hb.name.find(L"_diary") == 0)
					{
						valid = true;
					}else if(hb.name.find(L"_friend") == 0)
					{
						valid = true;
					}else if(hb.name.find(L"_mail") == 0)
					{
						valid = true;
					}else if(hb.name.find(L"_bbs") == 0)
					{
						valid = true;
					}else if(hb.name.find(L"_mm") == 0)
					{
						valid = true;
					}else if(hb.name.find(L"_clbbs") == 0)
					{
						valid = true;
					}else if(hb.name.find(L"_announce") == 0)
					{
						valid = true;
					}else if(hb.name.find(L"_mr") == 0)
					{
						valid = true;
					}else if(hb.name.find(L"_agit") == 0)
					{
						valid = true;
					}

					if(!valid)
					{
						vector<HtmlBypass> bypasses;
						ParseAction(sh.wHtml, bypasses);
						for(UINT m=0;m<bypasses.size();m++)
						{
							HtmlBypass& ohb = bypasses[m];
							if(ohb.name == hb.name)
							{
								if(ohb.params.size() == hb.params.size())
								{
									bool params = true;
									bool names = true;
									for(UINT i = 0; i < ohb.params.size() ;i++)
									{
										if(ohb.params[i].paramName != hb.params[i].paramName)
										{
											params = false;
											break;
										}
										if(ohb.params[i].paramValue != hb.params[i].paramValue)
										{
											if(ohb.params[i].paramValue.size() > 0)
											{
												if(ohb.params[i].paramValue[0] != L'$')
												{
													names = false;
													break;
												}
											}else
											{
												names = false;
												break;
											}
										}
									}
									if(names && params)
									{
										valid = true;
										break;
									}
								}
							}
						}
					}
				}
				if(!valid)
				{
				//	g_Log.Add(CLog::Error, "[%s] Invalid bypass[%S] for html[%S] User[%S]", __FUNCTION__, wBypass, sh.wName, pUser->pSD->wszName);
				}
			}
		}
		pUser->pED->sentHtmlLock.Leave();
	}else
	{
		valid = true;
	}
	unguard;
	return valid;
};

void CHtmlFilter::Split(vector<wstring>& data, wstring bypass)
{
	transform(bypass.begin(), bypass.end(), bypass.begin(), tolower);
	size_t pos = bypass.find_first_of(L" \t\r\n");
	while(pos != wstring::npos)
	{
		bypass.erase(pos, 1);
		pos = bypass.find_first_of(L" \t\r\n");
	}
		
	wstring word = L"";
	wstring separators = L"?&=";
	for(UINT n=0;n<bypass.size();n++)
	{
		bool wordFlag = false;
		WCHAR c = bypass[n];
		if(c == L'#' || c == L'\"')
		{
			wstring tmp;
			tmp += c;
			data.push_back(tmp);
			word = L"";
			wordFlag = true;
		}else
		{
			for(UINT m=0;m<separators.size();m++)
			{
				if(separators[m] == c)
				{
					data.push_back(word);
					word = L"";
					wordFlag = true;
				}
			}
		}
		if(!wordFlag)
		{
			word += c;
		}
	}
	data.push_back(word);
}


void CHtmlFilter::ParseBypass(const WCHAR* wBypass, HtmlBypass& hb, bool link)
{
	guard;
	vector<wstring> words;
	Split(words, wBypass);

	bool waitingParam = false;
	bool waitingValue = false;
	bool waitingName = false;

	BypassParam bp;
	if(link)
	{
		hb.type = BypassTypeLink;
		waitingParam = true;
	}else
	{
		hb.type = BypassTypeBypass;
		waitingName = true;
	}
	for(UINT n=0;n<words.size();n++)
	{
		wstring& word = words[n];
		if(waitingValue)
		{
			bp.paramValue = word;
			hb.params.push_back(bp);
			bp.paramName = L"";
			bp.paramValue = L"";
			waitingValue = false;
			waitingParam = true;
		}else if(waitingParam)
		{
			if(hb.type == BypassTypeLink)
			{
				hb.linkHtml = word;
				waitingParam = false;
			}else if(hb.type == BypassTypeBypass)
			{
				bp.paramName = word;
				waitingParam = false;
				waitingValue = true;
			}
		}else if(waitingName)
		{
			hb.name = word;
			waitingName = false;
			waitingParam = true;
		}
	}
	unguard;
}


void CHtmlFilter::ParseAction(wstring html, std::vector<HtmlBypass> &bypasses)
{
	guard;

	transform(html.begin(), html.end(), html.begin(), tolower);
	size_t pos = html.find(L" =");
	while(pos != wstring::npos)
	{
		html.erase(pos, 1);
		pos = html.find(L" =", pos);
	}
	pos = html.find(L"= ");
	while(pos != wstring::npos)
	{
		html.erase(pos+1, 1);
		pos = html.find(L"= ", pos);
	}
	//change ' to "
	pos = html.find(L'\'');
	while(pos != wstring::npos)
	{
		html.replace(pos, 1, L"\"");
		pos = html.find(L'\'', pos);
	}
	//extract action
	pos = html.find(L"action=");
	while(pos!=wstring::npos)
	{
		pos+=8;

		size_t endPos = html.find(L"\"", pos);
		if(endPos != wstring::npos)
		{
			wstring action = html.substr(pos, endPos - pos);
			//remove -h
			size_t tmpPos = action.find(L"-h ");
			if(tmpPos != wstring::npos)
			{
				action.erase(tmpPos, 3);
				tmpPos = action.find(L"-h ");
			}

			tmpPos = action.find(L" ");
			if(tmpPos != wstring::npos)
			{
				wstring type = action.substr(0, tmpPos);
				action = action.substr(tmpPos+1);
				HtmlBypass hb;
				if(type == L"bypass")
				{
					ParseBypass(action.c_str(), hb, false);
				}else
				{
					ParseBypass(action.c_str(), hb, true);
				}
				if(hb.type != BypassTypeNone)
				{
					bypasses.push_back(hb);
				}
			}
		}

		pos = html.find(L"action=", pos);
	}
	unguard;
}
