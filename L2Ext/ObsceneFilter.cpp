#include "StdAfx.h"
#include "ObsceneFilter.h"
#include "AccountDB.h"

CObsceneFilter g_ObsceneFilter;

CObsceneFilter::CObsceneFilter()
{
}

CObsceneFilter::~CObsceneFilter()
{
}

void CObsceneFilter::Init()
{
	guard;

	g_Log.Add(CLog::Blue, "[%S] Initializing", L"Obscene Filter");
	lWords.clear();
	LoadData();
	if(enabled)
	{
		g_Log.Add(CLog::Blue, "[%S] Feature is enabled, loaded %d word(s)", L"Obscene Filter", lWords.size());
	}else
	{
		g_Log.Add(CLog::Blue, "[%S] Feature is disabled", L"Obscene Filter");
	}

	unguard;
}

void CObsceneFilter::LoadData()
{
	guard;

	wstringstream file(ReadFileW(L"..//Script//ObsceneFilter.txt"));
	BOOL enabledTmp = FALSE;
	if(file)
	{
		wstring line;
		while(getline(file, line))
		{
			if(line.size() > 4)
			{
				if( line[0] == L'/' || line[0] == L';' )
					continue;

				if( line.find(L"setting_begin") == 0 )
				{
					//setting_begin	enabled=1 show_animation=1	to_low_level_msg=2344	to_high_level_msg=2345	not_enough_adena_msg=2346	setting_end
					enabledTmp = Parser::ParseOptionInt(line, L"enabled", 0);
					messageId = Parser::ParseOptionInt(line, L"messageId", 0);
					continue;
				}
				if( line.find(L"word_begin") == 0 )
				{
					//skill_begin	skill_id=1204	skill_level=2	price=0	min_level=1	max_level=90	custom_time=3600	skill_end
					ObsceneWordData data;
					memset(&data, 0, sizeof(ObsceneWordData));

					data.actionType = Parser::ParseOptionInt(line, L"action_type", 0);
					data.chatBanTime = Parser::ParseOptionInt(line, L"chat_ban_time", 0);
					data.kick = Parser::ParseOptionInt(line, L"kick", 0);
					data.partialSearch = Parser::ParseOptionInt(line, L"partial_search", 0);
					data.sendWarning = Parser::ParseOptionInt(line, L"send_warning", 0);
					data.banAccountTime = Parser::ParseOptionInt(line, L"ban_account_time", 0);
					wstring word = Parser::ParseOptionString(line, L"word");
					if(word.size() > 0)
					{
						transform(word.begin(), word.end(), word.begin(), towlower);
						data.word = word;
						lWords.push_back(data);
					}					
				}
			}
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%S] Cannot find ..//Script//ObsceneFilter.txt !", L"Obscene Filter");
	}
	enabled = enabledTmp;
	unguard;
}

void CObsceneFilter::CheckString(User *pUser, WCHAR* wStr, bool &drop, bool &kick)
{
	CTL;

	if(enabled && pUser->ValidUser())
	{
		wstring wMessageL(wStr);
		WCHAR wTemp[4096];
		memset(wTemp, 0, sizeof(wTemp));
		size_t len = wcslen(wStr);

		if(len > 4000)
		{
			drop = true;
			g_Log.Add(CLog::Error, "[%s] Too long string[%d] User[%S]", __FUNCTION__, len, pUser->pSD->wszName);
		
			return;
		}

		for( size_t n = 0; n < len ; n++ )
		{
			wTemp[n] = Parser::ToWLower(wStr[n]);
		}	

		
		for(vector<ObsceneWordData>::iterator Iter = lWords.begin(); Iter!=lWords.end();Iter++)
		{
			WCHAR* wFound = wcsstr(wTemp, Iter->word.c_str());
			while(wFound)
			{
				if(Iter->partialSearch)
				{
					if(Iter->sendWarning && messageId)
					{
						pUser->GetSocket()->SendSystemMessage(messageId);
					}
					if(Iter->chatBanTime > 0)
					{
						pUser->SetStopSay((Iter->chatBanTime * 1000), true);
					}
					if(Iter->banAccountTime > 0)
					{
						g_AccountDB.Ban(pUser->pSocket->accountId, pUser->pSocket->accountName, Iter->banAccountTime, L"Obscene Filter Ban");
					}
					if(Iter->kick)
					{
						kick = true;
					
						return;
					}
					if(Iter->actionType == 0)
					{
						drop = true;
					
						return;
					}
					if(Iter->actionType == 1)
					{
						for(int n=0;n<Iter->word.size();n++)
						{
							size_t pos = wFound - wTemp;
							wStr[pos+n] = L'*';
							wTemp[pos+n] = L'*';
						}
					}
				}else
				{
					WCHAR c1 = L' ';
					WCHAR c2 = L' ';
					if(wFound != wTemp)
					{
						c1 = wFound[-1];
					}
					
					c2 = wFound[Iter->word.size()];

					wstring separators( L" \n\t\r_-.,`'~<>!@#$%^&*()=+" );
					
					if(separators.find(c1) != wstring::npos && separators.find(c2) != wstring::npos)
					{
						if(Iter->sendWarning && messageId)
						{
							pUser->GetSocket()->SendSystemMessage(messageId);
						}
						if(Iter->chatBanTime > 0)
						{
							pUser->SetStopSay((Iter->chatBanTime * 1000), true);
						}
						
						if(Iter->banAccountTime > 0)
						{
							g_AccountDB.Ban(pUser->pSocket->accountId, pUser->pSocket->accountName, Iter->banAccountTime, L"Obscene Filter Ban");
						}
						if(Iter->kick)
						{
							kick = true;
						
							return;
						}
						if(Iter->actionType == 0)
						{
							drop = true;
						
							return;
						}
						if(Iter->actionType == 1)
						{
							for(int n=0; n < Iter->word.size(); n++)
							{
								size_t pos = wFound - wTemp;
								wStr[pos+n] = L'*';
								wTemp[pos+n] = L'*';
							}
						}
					}

					wFound = &wFound[1];
				}

				wFound = wcsstr(wFound, Iter->word.c_str());
			}
		}
	}

}

void CObsceneFilter::RequestReload()
{
	enabled = FALSE;
	reloadTimer = time(0) + 1;
}

void CObsceneFilter::TimerExpired()
{
	guard;
	if(enabled == FALSE && reloadTimer != 0)
	{
		if(reloadTimer < time(0))
		{
			reloadTimer = 0;
			Init();
		}
	}
	unguard;
}