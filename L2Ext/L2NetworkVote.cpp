#include "stdafx.h"
#include "L2NetworkVote.h"
#include "DB.h"
#include "HtmlCache.h"

CL2NetworkVote g_L2NetworkVote;

void CL2NetworkVote::Init()
{
	m_Enabled = false;
	memset(m_ApiKey, 0, sizeof(m_ApiKey));
	memset(m_Host, 0, sizeof(m_Host));
	memset(m_Path, 0, sizeof(m_Path));
	memset(m_TemplateLink, 0, sizeof(m_TemplateLink));
	LoadINI();
	if(m_Enabled)
	{
		g_Log.Add(CLog::Blue, "[%s] Feature is enabled - ApiKey[%s].", __FUNCTION__, m_ApiKey);
	}
}

void CL2NetworkVote::LoadINI()
{
/*
[L2NetworkVote]
Enabled=0
;0 - uses charId, 1 - uses character name, 2 - uses accountId, 3 - uses account name
IdentityType=0
;if set to 1 it will auto check for a reward when player enters world
CheckOnEnterWorld=0
;if set to 1 it will enable in game command ".votereward" to check a vote and get a reward.
EnableCommand=0
;Vote reward item id
RewardId=57
RewardAmount=10000
*/
	const TCHAR* section = _T("L2NetworkVote");

	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile) ? true : false;
	m_CheckOnEnterWorld = GetPrivateProfileInt(section, _T("CheckOnEnterWorld"), 0, g_ConfigFile) ? true : false;
	m_EnableCommand = GetPrivateProfileInt(section, _T("EnableCommand"), 0, g_ConfigFile) ? true : false;
	m_HardwareIdCheck = GetPrivateProfileInt(section, _T("HardwareIdCheck"), 0, g_ConfigFile) ? true : false;
	m_IdentityType = GetPrivateProfileInt(section, _T("IdentityType"), 0, g_ConfigFile);
	m_VoteReward.itemId = GetPrivateProfileInt(section, _T("RewardId"), 0, g_ConfigFile);
	m_VoteReward.amount = GetPrivateProfileInt(section, _T("RewardAmount"), 0, g_ConfigFile);
	m_VoteTimeout = GetPrivateProfileInt(section, _T("VoteTimeout"), 0, g_ConfigFile);
	GetPrivateProfileString(section, _T("TemplateLink"), 0, m_TemplateLink, 260, g_ConfigFile);
	TCHAR temp[8190] = { 0 };
	if(GetPrivateProfileString(section, _T("ApiKey"), 0, temp, 8190, g_ConfigFile))
	{
		for(UINT n=0;n<32;n++)
		{
			m_ApiKey[n] = (BYTE)temp[n];
		}
	}
	memset(temp, 0, sizeof(temp));
	if(GetPrivateProfileString(section, _T("Host"), 0, temp, 8190, g_ConfigFile))
	{
		for(UINT n=0;n<255;n++)
		{
			m_Host[n] = (BYTE)temp[n];
			if(temp[n] == 0)
			{
				break;
			}
		}
	}
	memset(temp, 0, sizeof(temp));
	if(GetPrivateProfileString(section, _T("Path"), 0, temp, 8190, g_ConfigFile))
	{
		for(UINT n=0;n<255;n++)
		{
			m_Path[n] = (BYTE)temp[n];
			if(temp[n] == 0)
			{
				break;
			}
		}
	}
}

void CL2NetworkVote::OnShowVote(User *pUser)
{
	guard;
	if(m_Enabled)
	{
		const WCHAR* wHtml = g_HtmlCache.Get(L"showvote.htm");
		if(wHtml)
		{
			WCHAR identity[33] = { 0 };	//aka player name

			switch(m_IdentityType)
			{
			case 0:
				{
					//charId
					_itow_s(pUser->nDBID, identity, 33, 10);
					break;
				}
			case 1:
				{
					//charName
					for(UINT n=0;n<16;n++)
					{
						if(pUser->pSD->wszName[n] != 0)
						{
							identity[n] = pUser->pSD->wszName[n];
						}else
						{
							break;
						}
					}
					break;
				}
			case 2:
				{
					_itow_s(pUser->pSocket->accountId, identity, 33, 10);
					break;
				}
			case 3:
				{
					//charName
					for(UINT n=0;n<16;n++)
					{
						if(pUser->pSD->wszName[n] != 0)
						{
							identity[n] = pUser->pSD->wszName[n];
						}else
						{
							break;
						}
					}
					break;
				}
			}

			wstring link = Utils::ReplaceString(m_TemplateLink, L"<?identity?>", identity, true);
			wstring html = Utils::ReplaceString(wHtml, L"<?vote_link?>", link, true);
			pUser->ShowHTML(L"showvote.htm", html.c_str(), 0);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find showvote.htm!", __FUNCTION__);
		}
	}

	unguard;
}

void CL2NetworkVote::OnCheck(User *pUser)
{
	guard;

	if(m_Enabled)
	{
		//check timestamp
		L2NetworkUser& lu = pUser->pED->l2networkUser;
		if(lu.checkTimestamp < GetTickCount())
		{
			lu.checkTimestamp = GetTickCount() + 10000;

			//check vote timestamp
			time_t currentTime = time(0);

			if( (lu.lastVoteTime + m_VoteTimeout) < currentTime )
			{
				bool canVote = true;
				if(m_HardwareIdCheck)
				{
					HWID& hardwareId = pUser->pSocket->pED->longHWID;
					AUTO_LOCK(m_Lock);
					for(UINT n=0;n<m_HWIDReuse.size();n++)
					{
						VoteReuse& vr = m_HWIDReuse[n];
						if(vr.hardwareId.IsEqual(hardwareId))
						{
							if(vr.timeout > currentTime)
							{
								canVote = false;
								break;
							}
						}
					}
				}
				if(canVote)
				{
					CHAR identity[33] = { 0 };	//aka player name

					switch(m_IdentityType)
					{
					case 0:
						{
							//charId
							_itoa_s(pUser->nDBID, identity, 33, 10);
							break;
						}
					case 1:
						{
							//charName
							for(UINT n=0;n<16;n++)
							{
								if(pUser->pSD->wszName[n] != 0)
								{
									identity[n] = pUser->pSD->wszName[n];
								}else
								{
									break;
								}
							}
							break;
						}
					case 2:
						{
							_itoa_s(pUser->pSocket->accountId, identity, 33, 10);
							break;
						}
					case 3:
						{
							//charName
							for(UINT n=0;n<16;n++)
							{
								if(pUser->pSD->wszName[n] != 0)
								{
									identity[n] = pUser->pSD->wszName[n];
								}else
								{
									break;
								}
							}
							break;
						}
					}

			//		g_Log.Add(CLog::Blue, "[%s] identity[%s] host[%s] path[%s]", __FUNCTION__, identity, m_Host, m_Path);
					CHAR buff[8190];
					int len = Assemble(buff, 8190, "cdssss", 0x87, 0x06, m_ApiKey, identity, m_Host, m_Path);
					pUser->pSocket->Send("b", len, buff);
				}
			}
		}
	}

	unguard;
}

void CL2NetworkVote::OnClientReply(User *pUser, INT32 lastVoteTime)
{
	guard;

	L2NetworkUser& lu = pUser->pED->l2networkUser;
	//check vote timestamp
	time_t currentTime = time(0);

//	g_Log.Add(CLog::Blue, "[%s] lastVote[%d] currentTime[%d]", __FUNCTION__, lastVoteTime, currentTime);
	if(lu.loaded)
	{
		if(!pUser->IsNowTrade() && pUser->pSD->nStoreMode == 0)
		{
			if(lastVoteTime > 0)
			{
				if( lastVoteTime > (currentTime - 604800) )
				{
					//vote is younger than 1 week
					if( (lu.lastVoteTime + m_VoteTimeout) < currentTime )
					{
						if(lu.lastVoteTime != lastVoteTime)
						{
							lu.lastVoteTime = lastVoteTime;
							lu.lastRewardTime = static_cast<UINT>(currentTime);

							//give reward
							if(m_VoteReward.itemId > 0 && m_VoteReward.amount > 0)
							{
								pUser->AddItemToInventory(m_VoteReward.itemId, m_VoteReward.amount);
							}

							g_Logger.Add(L"User[%s] received vote reward[%d][%d] voteTime[%d] currentTime[%d].", pUser->pSD->wszName, m_VoteReward.itemId, m_VoteReward.amount, lastVoteTime, currentTime);

							//save in db
							g_DB.RequestSaveL2NetworkVote(lu.identity, lu.lastVoteTime, lu.lastRewardTime);
							if(m_HardwareIdCheck)
							{
								bool added = false;
								HWID& hardwareId = pUser->pSocket->pED->longHWID;
								AUTO_LOCK(m_Lock);
								for(UINT n=0;n<m_HWIDReuse.size();n++)
								{
									VoteReuse& vr = m_HWIDReuse[n];
									if(vr.hardwareId.IsEqual(hardwareId))
									{
										vr.timeout = currentTime + m_VoteTimeout;
										added = true;
									}
								}
								if(!added)
								{
									VoteReuse vr;
									vr.hardwareId = hardwareId;
									vr.timeout = currentTime + m_VoteTimeout;
									m_HWIDReuse.push_back(vr);
								}
							}
						}
					}
				}
			}
		}else
		{
			pUser->SendSystemMessage(L"You cannot receive vote reward while trading!");
		}
	}

	unguard;
}