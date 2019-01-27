#include "StdAfx.h"
#include "ChatManager.h"
#include "UserDB.h"
#include "License.h"

using namespace License;

CChatManager g_ChatManager;

CChatManager::CChatManager() : m_Enabled(FALSE), m_GlobalChat(FALSE), m_BackupInstance(0), m_Reloading(0)
{

}

CChatManager::~CChatManager()
{

}

#pragma optimize("", off)

void CChatManager::Init(bool backup)
{
	VIRTUALIZER_TIGER_BLACK_START;
	if(CLicense::GetInstance().CanUse(LicenseChatManager))
	{
		m_ReloadTimestamp = 0;
		m_Enabled = FALSE;
		m_PrivateStoreCharset.clear();
		for(UINT n=0;n<18;n++)
		{
			m_ChatSetting[n].charset.clear();
			m_ChatSetting[n].delay = 0;
			m_ChatSetting[n].enabled = 0;
			m_ChatSetting[n].invalidCharMsg = L"";
			m_ChatSetting[n].minLevel = 0;
			m_ChatSetting[n].wordToReplace.clear();
		}
		LoadINI();
		if(m_Enabled)
		{
			g_Log.Add(CLog::Blue, "[%s] feature is enabled.", __FUNCTION__);
		}
		if(!backup)
		{
			g_Logger.Add(L"[%s] Loaded [%d]][%d] Prohibited Phrase(s).", GetName(), m_ProhibitedPhrase.size(), m_ProhibitedPhraseMask.size());
			
			if(m_BackupInstance == 0)
			{
				m_BackupInstance = new CChatManager();
				m_BackupInstance->Init(true);
			}
		}else
		{
			g_Logger.Add(L"[%s] Backup - Loaded [%d]][%d] Prohibited Phrase(s).", GetName(), m_ProhibitedPhrase.size(), m_ProhibitedPhraseMask.size());
		}
	}
	
	VIRTUALIZER_TIGER_BLACK_END;
}


void CChatManager::LoadINI()
{
	VIRTUALIZER_TIGER_BLACK_START;

	m_AllowedClass.clear();
	m_ProhibitedPhrase.clear();
	m_ProhibitedPhraseMask.clear();

	TCHAR configFile[260] = { 0 };
	GetCurrentDirectory(MAX_PATH, configFile);
	lstrcat(configFile, _T("\\ChatManager.ini"));

	const WCHAR* sectionName = L"ChatManager";
	m_MinLevel = GetPrivateProfileInt(sectionName, L"MinLevel", 0, configFile);
	m_MaxLevel = GetPrivateProfileInt(sectionName, L"MaxLevel", 85, configFile);
	m_MinUseTime = GetPrivateProfileInt(sectionName, L"MinUseTime", 0, configFile);
	m_MinUseTimeHWID = GetPrivateProfileInt(sectionName, L"MinUseTimeHWID", 0, configFile);
	m_AllClassAllowed = 0;
	TCHAR temp[16384];
	if(GetPrivateProfileString(sectionName, _T("AllowedClasses"), 0, temp, 8190, configFile))
	{
		wstringstream paramStream(temp);
		wstring param;
		while(paramStream >> param)
		{
			if(param == L"all")
			{
				m_AllClassAllowed = TRUE;
			}else if(param.find(L"-") != wstring::npos)
			{
				param = Parser::Replace(param, L'-', L' ');
				wstringstream nrStream(param);
				UINT fromClass = 0, toClass = 0;
				if(nrStream >> fromClass >> toClass)
				{
					if(fromClass < toClass)
					{
						for(UINT n=fromClass;n<=toClass;n++)
						{
							m_AllowedClass.push_back(n);
						}
					}
				}
			}else
			{
				UINT classId = 0;
				classId = _wtoi(param.c_str());
				m_AllowedClass.push_back(classId);
			}
		}
	}

	m_GlobalChat = GetPrivateProfileInt(sectionName, L"GlobalChat", 0, configFile);
	m_SendMessage = GetPrivateProfileInt(sectionName, L"SendMessage", 0, configFile);
	m_MessageId = GetPrivateProfileInt(sectionName, L"MessageId", 0, configFile);

	if(GetPrivateProfileString(sectionName, _T("PrivateStoreCharset"), 0, temp, 8190, configFile))
	{
		wstringstream patternStream(Utils::ReplaceString(temp, L"-", L" ", true));
		wstring low, high;
		while(patternStream >> low >> high)
		{
			m_PrivateStoreCharset.push_back(pair<WORD, WORD>(low[0], high[0]));
		}
	}

	if(GetPrivateProfileString(sectionName, L"ProhibitedPhrase", 0, temp, 16384, configFile))
	{
		//format {phrase one};{phrase two};{phrase three}
		vector<wstring> phrases = Parser::Split(temp, L";");
		for(UINT n=0;n<phrases.size();n++)
		{
			wstring phrase = phrases[n];
			wstring str;
			for(UINT i=0;i<phrase.size();i++)
			{
				if(phrase[i] != L'{' && phrase[i] != L'}')
				{
					str.push_back(Parser::ToWLower(phrase[i]));
				}
			}
			if(str.size() > 0)
			{
				m_ProhibitedPhrase.push_back(str);
			}
		}
	}
	if(GetPrivateProfileString(sectionName, L"ProhibitedPhraseMask", 0, temp, 16384, configFile))
	{
		//format {phrase one};{phrase two};{phrase three}
		vector<wstring> phrases = Parser::Split(temp, L";");
		for(UINT n=0;n<phrases.size();n++)
		{
			wstring phrase = phrases[n];
			wstring str;
			for(UINT i=0;i<phrase.size();i++)
			{
				if(phrase[i] != L'{' && phrase[i] != L'}')
				{
					str.push_back(Parser::ToWLower(phrase[i]));
				}
			}
			if(str.size() > 0)
			{
				m_ProhibitedPhraseMask.push_back(str);
			}
		}
	}

	m_ChatSetting[SAY_ALL].enabled = GetPrivateProfileInt(sectionName, L"AllEnabled", 1, configFile);
	m_ChatSetting[SAY_ALL].minLevel = GetPrivateProfileInt(sectionName, L"AllRequiredLevel", 1, configFile);
	m_ChatSetting[SAY_ALL].delay = GetPrivateProfileInt(sectionName, L"AllDelay", 0, configFile);
	
	if(GetPrivateProfileString(sectionName, _T("AllCharset"), 0, temp, 8190, configFile))
	{
		wstringstream patternStream(Utils::ReplaceString(temp, L"-", L" ", true));
		wstring low, high;
		while(patternStream >> low >> high)
		{
			m_ChatSetting[SAY_ALL].charset.push_back(pair<WORD, WORD>(low[0], high[0]));
		}
	}
	if(GetPrivateProfileString(sectionName, _T("AllInvalidCharMessage"), 0, temp, 8190, configFile))
	{
		m_ChatSetting[SAY_ALL].invalidCharMsg = temp;
	}

	if(GetPrivateProfileString(sectionName, _T("AllReplaceWord"), 0, temp, 8190, configFile))
	{
		wstringstream wordStream(Utils::ReplaceString(temp, L";", L" ", true));
		wstring wordR, wordN;
		while(wordStream >> wordR >> wordN)
		{
			m_ChatSetting[SAY_ALL].wordToReplace.push_back(pair<wstring, wstring>(wordR, wordN));
		}
	}

	m_ChatSetting[SAY_HERO_VOICE].enabled = GetPrivateProfileInt(sectionName, L"HeroEnabled", 1, configFile);
	m_ChatSetting[SAY_HERO_VOICE].minLevel = GetPrivateProfileInt(sectionName, L"HeroRequiredLevel", 1, configFile);
	m_ChatSetting[SAY_HERO_VOICE].delay = GetPrivateProfileInt(sectionName, L"HeroDelay", 2000, configFile);
	if(GetPrivateProfileString(sectionName, _T("HeroCharset"), 0, temp, 8190, configFile))
	{
		wstringstream patternStream(Utils::ReplaceString(temp, L"-", L" ", true));
		wstring low, high;
		while(patternStream >> low >> high)
		{
			m_ChatSetting[SAY_HERO_VOICE].charset.push_back(pair<WORD, WORD>(low[0], high[0]));
		}
	}
	if(GetPrivateProfileString(sectionName, _T("HeroInvalidCharMessage"), 0, temp, 8190, configFile))
	{
		m_ChatSetting[SAY_HERO_VOICE].invalidCharMsg = temp;
	}
	if(GetPrivateProfileString(sectionName, _T("HeroReplaceWord"), 0, temp, 8190, configFile))
	{
		wstringstream wordStream(Utils::ReplaceString(temp, L";", L" ", true));
		wstring wordR, wordN;
		while(wordStream >> wordR >> wordN)
		{
			m_ChatSetting[SAY_HERO_VOICE].wordToReplace.push_back(pair<wstring, wstring>(wordR, wordN));
		}
	}

	m_ChatSetting[SAY_TRADE].enabled = GetPrivateProfileInt(sectionName, L"TradeEnabled", 1, configFile);
	m_ChatSetting[SAY_TRADE].minLevel = GetPrivateProfileInt(sectionName, L"TradeRequiredLevel", 1, configFile);
	m_ChatSetting[SAY_TRADE].delay = GetPrivateProfileInt(sectionName, L"TradeDelay", 0, configFile);
	if(GetPrivateProfileString(sectionName, _T("TradeCharset"), 0, temp, 8190, configFile))
	{
		wstringstream patternStream(Utils::ReplaceString(temp, L"-", L" ", true));
		wstring low, high;
		while(patternStream >> low >> high)
		{
			m_ChatSetting[SAY_TRADE].charset.push_back(pair<WORD, WORD>(low[0], high[0]));
		}
	}
	if(GetPrivateProfileString(sectionName, _T("TradeInvalidCharMessage"), 0, temp, 8190, configFile))
	{
		m_ChatSetting[SAY_TRADE].invalidCharMsg = temp;
	}
	if(GetPrivateProfileString(sectionName, _T("TradeReplaceWord"), 0, temp, 8190, configFile))
	{
		wstringstream wordStream(Utils::ReplaceString(temp, L";", L" ", true));
		wstring wordR, wordN;
		while(wordStream >> wordR >> wordN)
		{
			m_ChatSetting[SAY_TRADE].wordToReplace.push_back(pair<wstring, wstring>(wordR, wordN));
		}
	}

	m_ChatSetting[SAY_SHOUT].enabled = GetPrivateProfileInt(sectionName, L"ShoutEnabled", 1, configFile);
	m_ChatSetting[SAY_SHOUT].minLevel = GetPrivateProfileInt(sectionName, L"ShoutRequiredLevel", 1, configFile);
	m_ChatSetting[SAY_SHOUT].delay = GetPrivateProfileInt(sectionName, L"ShoutDelay", 0, configFile);
	if(GetPrivateProfileString(sectionName, _T("ShoutCharset"), 0, temp, 8190, configFile))
	{
		wstringstream patternStream(Utils::ReplaceString(temp, L"-", L" ", true));
		wstring low, high;
		while(patternStream >> low >> high)
		{
			m_ChatSetting[SAY_SHOUT].charset.push_back(pair<WORD, WORD>(low[0], high[0]));
		}
	}
	if(GetPrivateProfileString(sectionName, _T("ShoutInvalidCharMessage"), 0, temp, 8190, configFile))
	{
		m_ChatSetting[SAY_SHOUT].invalidCharMsg = temp;
	}
	if(GetPrivateProfileString(sectionName, _T("ShoutReplaceWord"), 0, temp, 8190, configFile))
	{
		wstringstream wordStream(Utils::ReplaceString(temp, L";", L" ", true));
		wstring wordR, wordN;
		while(wordStream >> wordR >> wordN)
		{
			m_ChatSetting[SAY_SHOUT].wordToReplace.push_back(pair<wstring, wstring>(wordR, wordN));
		}
	}

	m_ChatSetting[SAY_TELL].enabled = GetPrivateProfileInt(sectionName, L"TellEnabled", 1, configFile);
	m_ChatSetting[SAY_TELL].minLevel = GetPrivateProfileInt(sectionName, L"TellRequiredLevel", 1, configFile);
	m_ChatSetting[SAY_TELL].delay = GetPrivateProfileInt(sectionName, L"TellDelay", 0, configFile);
	if(GetPrivateProfileString(sectionName, _T("TellCharset"), 0, temp, 8190, configFile))
	{
		wstringstream patternStream(Utils::ReplaceString(temp, L"-", L" ", true));
		wstring low, high;
		while(patternStream >> low >> high)
		{
			m_ChatSetting[SAY_TELL].charset.push_back(pair<WORD, WORD>(low[0], high[0]));
		}
	}
	if(GetPrivateProfileString(sectionName, _T("TellInvalidCharMessage"), 0, temp, 8190, configFile))
	{
		m_ChatSetting[SAY_TELL].invalidCharMsg = temp;
	}
	if(GetPrivateProfileString(sectionName, _T("TellReplaceWord"), 0, temp, 8190, configFile))
	{
		wstringstream wordStream(Utils::ReplaceString(temp, L";", L" ", true));
		wstring wordR, wordN;
		while(wordStream >> wordR >> wordN)
		{
			m_ChatSetting[SAY_TELL].wordToReplace.push_back(pair<wstring, wstring>(wordR, wordN));
		}
	}

	m_ChatSetting[SAY_CLAN].enabled = GetPrivateProfileInt(sectionName, L"ClanEnabled", 1, configFile);
	m_ChatSetting[SAY_CLAN].minLevel = GetPrivateProfileInt(sectionName, L"ClanRequiredLevel", 1, configFile);
	m_ChatSetting[SAY_CLAN].delay = GetPrivateProfileInt(sectionName, L"ClanDelay", 0, configFile);
	if(GetPrivateProfileString(sectionName, _T("ClanCharset"), 0, temp, 8190, configFile))
	{
		wstringstream patternStream(Utils::ReplaceString(temp, L"-", L" ", true));
		wstring low, high;
		while(patternStream >> low >> high)
		{
			m_ChatSetting[SAY_CLAN].charset.push_back(pair<WORD, WORD>(low[0], high[0]));
		}
	}
	if(GetPrivateProfileString(sectionName, _T("ClanInvalidCharMessage"), 0, temp, 8190, configFile))
	{
		m_ChatSetting[SAY_CLAN].invalidCharMsg = temp;
	}
	if(GetPrivateProfileString(sectionName, _T("ClanReplaceWord"), 0, temp, 8190, configFile))
	{
		wstringstream wordStream(Utils::ReplaceString(temp, L";", L" ", true));
		wstring wordR, wordN;
		while(wordStream >> wordR >> wordN)
		{
			m_ChatSetting[SAY_CLAN].wordToReplace.push_back(pair<wstring, wstring>(wordR, wordN));
		}
	}

	m_ChatSetting[SAY_ALLIANCE].enabled = GetPrivateProfileInt(sectionName, L"AllianceEnabled", 1, configFile);
	m_ChatSetting[SAY_ALLIANCE].minLevel = GetPrivateProfileInt(sectionName, L"AllianceRequiredLevel", 1, configFile);
	m_ChatSetting[SAY_ALLIANCE].delay = GetPrivateProfileInt(sectionName, L"AllianceDelay", 0, configFile);
	if(GetPrivateProfileString(sectionName, _T("AllianceCharset"), 0, temp, 8190, configFile))
	{
		wstringstream patternStream(Utils::ReplaceString(temp, L"-", L" ", true));
		wstring low, high;
		while(patternStream >> low >> high)
		{
			m_ChatSetting[SAY_ALLIANCE].charset.push_back(pair<WORD, WORD>(low[0], high[0]));
		}
	}
	if(GetPrivateProfileString(sectionName, _T("AllianceInvalidCharMessage"), 0, temp, 8190, configFile))
	{
		m_ChatSetting[SAY_ALLIANCE].invalidCharMsg = temp;
	}
	if(GetPrivateProfileString(sectionName, _T("AllianceReplaceWord"), 0, temp, 8190, configFile))
	{
		wstringstream wordStream(Utils::ReplaceString(temp, L";", L" ", true));
		wstring wordR, wordN;
		while(wordStream >> wordR >> wordN)
		{
			m_ChatSetting[SAY_ALLIANCE].wordToReplace.push_back(pair<wstring, wstring>(wordR, wordN));
		}
	}

	m_ChatSetting[SAY_PARTY].enabled = GetPrivateProfileInt(sectionName, L"PartyEnabled", 1, configFile);
	m_ChatSetting[SAY_PARTY].minLevel = GetPrivateProfileInt(sectionName, L"PartyRequiredLevel", 1, configFile);
	m_ChatSetting[SAY_PARTY].delay = GetPrivateProfileInt(sectionName, L"PartyDelay", 0, configFile);
	if(GetPrivateProfileString(sectionName, _T("PartyCharset"), 0, temp, 8190, configFile))
	{
		wstringstream patternStream(Utils::ReplaceString(temp, L"-", L" ", true));
		wstring low, high;
		while(patternStream >> low >> high)
		{
			m_ChatSetting[SAY_PARTY].charset.push_back(pair<WORD, WORD>(low[0], high[0]));
		}
	}
	if(GetPrivateProfileString(sectionName, _T("PartyInvalidCharMessage"), 0, temp, 8190, configFile))
	{
		m_ChatSetting[SAY_PARTY].invalidCharMsg = temp;
	}
	if(GetPrivateProfileString(sectionName, _T("PartyReplaceWord"), 0, temp, 8190, configFile))
	{
		wstringstream wordStream(Utils::ReplaceString(temp, L";", L" ", true));
		wstring wordR, wordN;
		while(wordStream >> wordR >> wordN)
		{
			m_ChatSetting[SAY_PARTY].wordToReplace.push_back(pair<wstring, wstring>(wordR, wordN));
		}
	}

	m_ChatSetting[SAY_PARTYROOM_ALL].enabled = GetPrivateProfileInt(sectionName, L"PartyRoomAllEnabled", 1, configFile);
	m_ChatSetting[SAY_PARTYROOM_ALL].minLevel = GetPrivateProfileInt(sectionName, L"PartyRoomAllRequiredLevel", 1, configFile);
	m_ChatSetting[SAY_PARTYROOM_ALL].delay = GetPrivateProfileInt(sectionName, L"PartyRoomAllDelay", 0, configFile);
	if(GetPrivateProfileString(sectionName, _T("PartyRoomAllCharset"), 0, temp, 8190, configFile))
	{
		wstringstream patternStream(Utils::ReplaceString(temp, L"-", L" ", true));
		wstring low, high;
		while(patternStream >> low >> high)
		{
			m_ChatSetting[SAY_PARTYROOM_ALL].charset.push_back(pair<WORD, WORD>(low[0], high[0]));
		}
	}
	if(GetPrivateProfileString(sectionName, _T("PartyRoomAllInvalidCharMessage"), 0, temp, 8190, configFile))
	{
		m_ChatSetting[SAY_PARTYROOM_ALL].invalidCharMsg = temp;
	}
	if(GetPrivateProfileString(sectionName, _T("PartyRoomAllReplaceWord"), 0, temp, 8190, configFile))
	{
		wstringstream wordStream(Utils::ReplaceString(temp, L";", L" ", true));
		wstring wordR, wordN;
		while(wordStream >> wordR >> wordN)
		{
			m_ChatSetting[SAY_PARTYROOM_ALL].wordToReplace.push_back(pair<wstring, wstring>(wordR, wordN));
		}
	}

	m_ChatSetting[SAY_PARTYROOM_COMMANDER].enabled = GetPrivateProfileInt(sectionName, L"PartyRoomCommanderEnabled", 1, configFile);
	m_ChatSetting[SAY_PARTYROOM_COMMANDER].minLevel = GetPrivateProfileInt(sectionName, L"PartyRoomCommanderRequiredLevel", 1, configFile);
	m_ChatSetting[SAY_PARTYROOM_COMMANDER].delay = GetPrivateProfileInt(sectionName, L"PartyRoomCommanderDelay", 0, configFile);
	if(GetPrivateProfileString(sectionName, _T("PartyRoomCommanderCharset"), 0, temp, 8190, configFile))
	{
		wstringstream patternStream(Utils::ReplaceString(temp, L"-", L" ", true));
		wstring low, high;
		while(patternStream >> low >> high)
		{
			m_ChatSetting[SAY_PARTYROOM_COMMANDER].charset.push_back(pair<WORD, WORD>(low[0], high[0]));
		}
	}
	if(GetPrivateProfileString(sectionName, _T("PartyRoomCommanderInvalidCharMessage"), 0, temp, 8190, configFile))
	{
		m_ChatSetting[SAY_PARTYROOM_COMMANDER].invalidCharMsg = temp;
	}
	if(GetPrivateProfileString(sectionName, _T("PartyRoomCommanderReplaceWord"), 0, temp, 8190, configFile))
	{
		wstringstream wordStream(Utils::ReplaceString(temp, L";", L" ", true));
		wstring wordR, wordN;
		while(wordStream >> wordR >> wordN)
		{
			m_ChatSetting[SAY_PARTYROOM_COMMANDER].wordToReplace.push_back(pair<wstring, wstring>(wordR, wordN));
		}
	}
	m_Enabled = GetPrivateProfileInt(sectionName, L"Enabled", 0, configFile);
	VIRTUALIZER_TIGER_BLACK_END;
}

#pragma optimize("", on)

void CChatManager::TimerExpired()
{
	if(m_ReloadTimestamp > 0 && m_ReloadTimestamp < time(0))
	{
		Init();
		m_Reloading = FALSE;
	}else if(m_ReloadTimestamp > 0 && (m_ReloadTimestamp - 5) < time(0) && !m_Reloading)
	{
		m_BackupInstance->Init(true);
		m_Reloading = TRUE;
	}
}

bool CChatManager::IsClassAllowed(UINT classId)
{
	bool ret = false;
	try
	{
		for(UINT n=0;n<m_AllowedClass.size();n++)
		{
			if(classId == m_AllowedClass[n])
			{
				ret = true;
				break;
			}
		}
	}catch(...)
	{

	}

	return false;
}

bool CChatManager::CanChat(User *pUser)
{
	guard;
	bool ret = false;

	if(m_Enabled)
	{
		if(pUser->pSD->nLevel >= m_MinLevel && pUser->pSD->nLevel <= m_MaxLevel)
		{
			if(/* m_MinUseTime < pUser->GetUseTime() || (m_MinUseTimeHWID > 0 && m_MinUseTimeHWID < g_UserDB.GetUseTimeByHWID(pUser->pSocket->pED->longHWID.hash)) */ true )
			{
				if(m_AllClassAllowed || IsClassAllowed(pUser->pSD->nClass))
				{
					ret = true;
				}else
				{
					pUser->SendSystemMessage(L"Your class cannot use chat.");
				}
			}else
			{
				pUser->SendSystemMessage(L"You need to play a bit longer to be able to use chat.");
			}
		}else
		{
			pUser->SendSystemMessage(L"You cannot use chat with your character level.");
		}
	}
	unguard;
	return ret;
}

bool CChatManager::ValidPrivateStoreMsg(wstring wMessage)
{
	guard;
	bool ret = true;

	if(m_PrivateStoreCharset.size() > 0)
	{
		//check for forbidden characters
		size_t len = wMessage.size();
		for(UINT n=0;n<len;n++)
		{
			bool valid = false;
			for(UINT m=0;m<m_PrivateStoreCharset.size();m++)
			{
				if(m_PrivateStoreCharset[m].first <= wMessage[n] && m_PrivateStoreCharset[m].second >= wMessage[n])
				{
					valid = true;
					break;
				}
			}
			if(!valid && wMessage[n] != L' ')
			{
				ret = false;
				break;
			}
		}
	}
	unguard;
	return ret;
}

bool CChatManager::ContainProhibitedPhrase(wstring str)
{
	guard;
	bool ret = false;
	if(m_Enabled)
	{
		//convert to lowercase
		for(UINT n = 0; n < str.size();n++)
		{
			str[n] = Parser::ToWLower(str[n]);
		}
		for(UINT n = 0; n < m_ProhibitedPhrase.size();n++)
		{
			if(str.find(m_ProhibitedPhrase[n]) != wstring::npos)
			{
			//	g_Log.Add(CLog::Blue, "[%s] containing prohibited phrase[%S] in str[%S]", __FUNCTION__, m_ProhibitedPhrase[n].c_str(), str.c_str());
				ret = true;
				break;
			}
		}

		if(!ret)
		{
			for(UINT n = 0; n < m_ProhibitedPhraseMask.size();n++)
			{
				wstring& phrase = m_ProhibitedPhraseMask[n];
				UINT indexFound = 0;
				for(UINT m=0;m<str.size();m++)
				{
					if(indexFound < phrase.size())
					{
						if(str[m] == phrase[indexFound])
						{
							indexFound++;
						}
					}
					if(indexFound == phrase.size())
					{
				//		g_Log.Add(CLog::Blue, "[%s] containing prohibited phrase masked[%S] in str[%S]", __FUNCTION__, phrase.c_str(), str.c_str());
						ret = true;
						break;
					}
				}
				if(ret == true)
				{
					break;
				}
			}
		}
	}
	unguard;
	return ret;
}

bool CChatManager::Handle(CUserSocket *pSocket, const unsigned char *packet, int type)
{
	if(m_Enabled)
	{
		if(User *pUser = pSocket->pUser->SafeCastUser())
		{
			if(CanChat(pUser))
			{
				WCHAR wMessage[4096];
				memset(wMessage, 0, sizeof(wMessage));
				Disassemble(packet, "S", 2048, wMessage);
				if(ContainProhibitedPhrase(wMessage))
				{
					pUser->SendSystemMessage(L"Your message contains prohibited phrase!");

					return false;
				}
				DWORD currentTick = GetTickCount();
				switch(type)
				{
				case SAY_ALL:
				case SAY_HERO_VOICE:
				case SAY_TRADE:
				case SAY_SHOUT:
				case SAY_TELL:
				case SAY_CLAN:
				case SAY_ALLIANCE:
				case SAY_PARTY:
				case SAY_PARTYROOM_ALL:
				case SAY_PARTYROOM_COMMANDER:
					{
						if(m_ChatSetting[type].enabled)
						{
							if(m_ChatSetting[type].minLevel > pUser->pSD->nLevel)
							{
								if(m_SendMessage)
								{
									pUser->SendSystemMessage(L"Chat Manager", L"Level of your chatacter is too low, you cannot use this chat.");
								}
								return false;
							}
							if(m_ChatSetting[type].delay > 0)
							{
								if( pUser->pED->chatTimestamp[type] > currentTick )
								{
									if(m_SendMessage && m_MessageId)
									{
										pSocket->SendSystemMessage(m_MessageId);
									}
									return false;
								}

								pUser->pED->chatTimestamp[type] = currentTick + m_ChatSetting[type].delay;
							}
							if(m_ChatSetting[type].charset.size() > 0)
							{
								//check for forbidden characters
								vector<pair<WORD, WORD>>& charset = m_ChatSetting[type].charset;
								size_t len = wcslen(wMessage);
								for(UINT n=0;n<len;n++)
								{
									bool valid = false;
									for(UINT m=0;m<charset.size();m++)
									{
										if(charset[m].first <= wMessage[n] && charset[m].second >= wMessage[n])
										{
											valid = true;
											break;
										}
									}
									if(!valid && wMessage[n] != L' ')
									{
										pUser->SendSystemMessage(m_ChatSetting[type].invalidCharMsg.c_str());
										return false;
									}
								}
							}
							if( m_ChatSetting[type].wordToReplace.size() > 0 )
							{
								if(type != SAY_TELL)
								{
									wstring wBase((PWCHAR)packet);
									
									vector<pair<wstring, wstring>>& words = m_ChatSetting[type].wordToReplace;
									for(UINT n=0;n<words.size();n++)
									{
										size_t pos = wBase.find(words[n].first);
										if(pos != wstring::npos)
										{
											bool toReplace = false;
											if(pos > 0)
											{
												if(wBase[pos - 1] == L' ')
												{
													if( (pos + words[n].first.size()) < wBase.size())
													{
														if(wBase[pos+words[n].first.size()] == L' ')
														{
															toReplace = true;
														}
													}else
													{
														toReplace = true;
													}
												}
											}else
											{
												if( (pos + words[n].first.size()) < wBase.size())
												{
													if(wBase[pos+words[n].first.size()] == L' ')
													{
														toReplace = true;
													}
												}else
												{
													toReplace = true;
												}
											}
											if(toReplace)
											{
												wBase.replace(pos, words[n].first.size(), words[n].second);
												Assemble((PCHAR)packet, 4096, "Sd", wBase.c_str(), type);
											}
										}
									}
								}
							}
							if(type == SAY_SHOUT && m_GlobalChat)
							{
								if(pUser->stopSayTick == 0)
								{
									WCHAR wMessage[4096];
									memset(wMessage, 0, sizeof(wMessage));
									Disassemble(packet, "S", 2048, wMessage);
									CHAR buff[8190];
									int len = Assemble(buff, 8190, "cddSS", 0x4A, pUser->nObjectID, type, pUser->pSD->wszName, wMessage);
									L2Server::BroadcastToAllUser(len, buff);
								}else
								{
									//147	1	a,Chatting is currently prohibited.\0	4	79	9B	B0	FF	a,ItemSound3.sys_chat_prohibition\0	a,147\0	0	0	0	0	0	a,	a,popup\0
									pUser->pSocket->SendSystemMessage(147);
								}
								return false;
							}
						}else
						{
							if(m_SendMessage && m_MessageId)
							{
								pSocket->SendSystemMessage(m_MessageId);
							}
							return false;
						}
						break;
					}
				default:
					{
						break;
					}
				}
				return Callback(0x00886870L)(pSocket, packet);
			}
			return false;
		}
		return true;
	}
	return Callback(0x00886870L)(pSocket, packet);
}