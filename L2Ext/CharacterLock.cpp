#include "stdafx.h"
#include "CharacterLock.h"
#include "MD5.h"
#include "DB.h"

CCharacterLock g_CharacterLock;

void CCharacterLock::Init()
{
	m_Enabled = FALSE;
	LoadINI();
	if(m_Enabled)
	{
		g_Log.Add(CLog::Blue, "[%s] Feature is enabled.", __FUNCTION__);
	}
}

/*
[CharacterLock]
Enabled=0
;Force player to create pin
Force=1
AttemptLimit=3
BanTimeout=300
;In db there will be up to 16 characters limit
MinCodeLength=4
MaxCodeLength=4
;pairs separated with space
AllowedCharacters=0-9
*/
void CCharacterLock::LoadINI()
{
	const TCHAR* section = _T("CharacterLock");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile);
	m_Force = GetPrivateProfileInt(section, _T("Force"), 0, g_ConfigFile);
	m_AttemptLimit = GetPrivateProfileInt(section, _T("AttemptLimit"), 3, g_ConfigFile);
	m_BanTimeout = GetPrivateProfileInt(section, _T("BanTImeout"), 0, g_ConfigFile);
	m_MinCodeLength = GetPrivateProfileInt(section, _T("MinCodeLength"), 4, g_ConfigFile);
	m_MaxCodeLength = GetPrivateProfileInt(section, _T("MaxCodeLength"), 4, g_ConfigFile);
	TCHAR temp[8190] = { 0 };
	if(GetPrivateProfileString(section, _T("AllowedCharacters"), 0, temp, sizeof(temp), g_ConfigFile))
	{
		wstringstream dataStream(temp);
		wstring chars;
		while(dataStream >> chars)
		{
			chars = Parser::Replace(chars, L'-', L' ');
			WCHAR wStart = 0, wEnd = 0;
			wstringstream charStream(chars);
			charStream >> wStart >> wEnd;
			if(wStart < wEnd)
			{
				m_AllowedCharacters.push_back(pair<WCHAR, WCHAR>(wStart, wEnd));
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Invalid allowed characters pair(%C-%C)[%d-%d]", __FUNCTION__, wStart, wEnd, (UINT)wStart, (UINT)wEnd);
			}
		}
	}
	m_BanHTML[0] = 0;
	GetPrivateProfileString(section, _T("BanHTML"), 0, m_BanHTML, 0x4000, g_ConfigFile);
}

bool CCharacterLock::IsBanned(UINT charId, HWID &hwid)
{
	guard;

	bool ret = false;

	UINT currentTime = time(0);
	m_BannedLock.Enter();

	for(vector<CharacterHWIDBan>::iterator it = m_BannedHWID.begin(); it!= m_BannedHWID.end(); it++)
	{
		if(it->charId == charId && it->hwid.IsEqual(hwid))
		{
			if(it->timeout > currentTime)
			{
				ret = true;
				break;
			}else
			{
				m_BannedHWID.erase(it);
				break;
			}
		}
	}

	m_BannedLock.Leave();

	unguard;
	return ret;
}

void CCharacterLock::BanHWID(UINT charId, HWID& hwid)
{
	guard;
	if(m_BanTimeout > 0)
	{
		CharacterHWIDBan ban;
		ban.charId = charId;
		ban.hwid = hwid;
		ban.timeout = time(0) + m_BanTimeout;
		m_BannedLock.Enter();
		m_BannedHWID.push_back(ban);
		m_BannedLock.Leave();
		g_Logger.Add(L"Banned HWID[%s] for char_id[%d] - reached character lock attempt limit[%d]!", hwid.ToString().c_str(), charId, m_AttemptLimit);
	}

	unguard;
}

void CCharacterLock::OnDBLoad(UINT charId, LPBYTE lpHash)
{
	guard;

	if(m_Enabled)
	{
		if(User *pUser = g_UserDB.GetUserByDBID(charId)->SafeCastUser())
		{
			if(IsZeroMemory(lpHash, 32))
			{
				if(m_Force)
				{
					//force user to create a lock
					pUser->pED->isLocked = true;
					//2503	1	a,Your character has been locked!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pUser->pSocket->SendSystemMessage(2503);
					//2504	1	a,Please enter 4 digits to create new PIN:\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pUser->pSocket->SendSystemMessage(2504);
				}
			}else
			{
				memcpy(pUser->pED->lockHash, lpHash, 32);
				pUser->pED->isLocked = true;
				//2503	1	a,Your character has been locked!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2503);
				//2505	1	a,Please enter the PIN code to unlock your character:\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2505);
			}
		}
	}

	unguard;
}

bool CCharacterLock::OnSay(User *pUser, std::wstring text)
{
	guard;
	if(m_Enabled)
	{
		if(text.size() > 0 && text.size() < 32)
		{
			if(IsZeroMemory(pUser->pED->lockHash, 32))
			{
				if(!IsZeroMemory(pUser->pED->tempLockHash, 32))
				{
					//confirm the new pin
					//generate MD5 hash
					string hash = md5(WStringToString(text));
					if(hash.size() == 32)
					{
						if(!memcmp(hash.c_str(), pUser->pED->tempLockHash, 32))
						{
							//correct confirmation save data
							memcpy(pUser->pED->lockHash, hash.c_str(), 32);
							ZeroMemory(pUser->pED->tempLockHash, 32);
							pUser->pED->isLocked = false;
							//2506	1	a,Your character has been unlocked!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pUser->pSocket->SendSystemMessage(2506);
							g_DB.RequestSaveCharacterLock(pUser->nDBID, pUser->pED->lockHash, pUser->pSocket->pED->longHWID.hash);
							pUser->ActionFailed();
						}else
						{
							ZeroMemory(pUser->pED->tempLockHash, 32);
							//2507	1	a,PIN code doesn't match!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pUser->pSocket->SendSystemMessage(2507);
							//2504	1	a,Please enter 4 digits to create new PIN:\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pUser->pSocket->SendSystemMessage(2504);
						}
					}
				}else
				{
					//new pin

					//check allowed characters
					if(m_AllowedCharacters.size() > 0)
					{
						bool allowed = false;
						for(UINT n=0;n<text.size();n++)
						{
							allowed = false;
							for(UINT l=0;l<m_AllowedCharacters.size();l++)
							{
								if(text[n] >= m_AllowedCharacters[l].first && text[n] <= m_AllowedCharacters[l].second)
								{
									allowed = true;
									break;
								}
							}
							if(!allowed)
							{
								break;
							}
						}
						if(!allowed)
						{
							//2508	1	a,Invalid character!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pUser->pSocket->SendSystemMessage(2508);
							unguard;
							return true;
						}
					}

					//check length
					if(text.size() < m_MinCodeLength || text.size() > m_MaxCodeLength)
					{
						//2509	1	a,Invalid PIN length!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->pSocket->SendSystemMessage(2509);
						unguard;
						return true;
					}

					//generate MD5 hash
					string hash = md5(WStringToString(text));
					if(hash.size() == 32)
					{
						memcpy(pUser->pED->tempLockHash, hash.c_str(), 32);
						//2510	1	a,Please confirm your new PIN code:\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->pSocket->SendSystemMessage(2510);
						unguard;
						return true;
					}
				}
			}else
			{
				//entering the pin
				if(m_AttemptLimit == 0 || pUser->pED->lockAttempts < m_AttemptLimit)
				{
					//generate MD5 hash
					string hash = md5(WStringToString(text));
					if(hash.size() == 32)
					{
				//		g_Log.Add(CLog::Blue, "[%s] vs [%s]", hash.c_str(), pUser->pED->lockHash);
						if(!memcmp(hash.c_str(), pUser->pED->lockHash, 32))
						{
							pUser->pED->isLocked = false;
							//2506	1	a,Your character has been unlocked!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pUser->pSocket->SendSystemMessage(2506);
						}else
						{
							//2507	1	a,PIN code doesn't match!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pUser->pSocket->SendSystemMessage(2507);
							//2505	1	a,Please enter the PIN code to unlock your character:\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pUser->pSocket->SendSystemMessage(2505);
							pUser->pED->lockAttempts++;
						}
					}
				}else
				{
					if(pUser->pSD->pExData->kickTick == 0)
					{
						//2511	1	a,You've reached PIN code attempt limit!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->pSocket->SendSystemMessage(2511);
						pUser->pSD->pExData->kickTick = 2;
						BanHWID(pUser->nDBID, pUser->pSocket->pED->longHWID);
					}
				}
			}
		}
	}
	unguard;
	return false;
}