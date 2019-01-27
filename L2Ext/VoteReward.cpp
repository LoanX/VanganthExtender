#include "StdAfx.h"
#include "VoteReward.h"
#include "DB.h"

CVoteReward g_VoteReward;

CVoteReward::CVoteReward() : m_Enabled(FALSE), m_TerminateThread(false), m_ThreadHandle(0)
{
}

CVoteReward::~CVoteReward()
{
	if(m_ThreadHandle)
	{
		m_TerminateThread = true;
	}
}

void CVoteReward::Init()
{
	LoadData();
	if(m_Enabled)
	{
		g_Log.Add(CLog::Blue, "[%s] Feature is enabled, loaded[%d] reward(s).", __FUNCTION__, m_Rewards.size());
		m_ThreadHandle = (HANDLE)_beginthread(CVoteReward::JobThread, 0, 0);
	}
}

/*
[VoteReward]
Enabled=0
;Item pair list, format: item_id;item_amount, separated with space
RewardList=57;10000
;Points required to get 1x reward
RequiredPoints=1
HttpRequestUrl=http://localhost/get_reward.php
PendingJobLimit=50
*/

INT32 GetPointsHelper(string shost, string spath)
{
	INT32 points = 0;
	if(shost.size() > 0 && spath.size() > 0)
	{
		hostent *remoteHost = gethostbyname(shost.c_str());
		if(remoteHost)
		{
			hostent host = *remoteHost;
			in_addr addr;
			stringstream webStream;
			if(remoteHost)
			{
				if(host.h_length)
				{
					addr.s_addr = (*(u_long *)host.h_addr_list[0]);
					SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
					if(s != INVALID_SOCKET)
					{
						SOCKADDR_IN sockAddr;
						memset(&sockAddr,0, sizeof(sockAddr));
						sockAddr.sin_family = AF_INET;
						sockAddr.sin_port = htons(80);
						sockAddr.sin_addr = *((LPIN_ADDR)*host.h_addr_list);

						if(!connect(s,(SOCKADDR *)&sockAddr,sizeof(SOCKADDR_IN)))
						{
							char buffer[8192];
							memset(buffer, 0, sizeof(buffer));
							int len = 0;

							string request = "GET " + spath + " HTTP/1.0\r\n";
							request += "Host: " + shost + "\r\n";
							request += "Agent: Other\r\n";
							request += "Accept: */*\r\n";
							request += "Connection: close\r\n\r\n";

							char sendBuffer[8192];
							memset(sendBuffer, 0, sizeof(sendBuffer));
							strcpy_s(sendBuffer, 8192, request.c_str());
							send(s, sendBuffer, static_cast<int>(request.size()+1), 0);
							int bytes = 0;
							try
							{
								while((bytes = recv(s, buffer, 8190, 0)) > 0 )
								{
									len += bytes;

									webStream << buffer;

									if(buffer[bytes-1] == '\0')
										break;

									memset(buffer, 0, sizeof(buffer));
								}
							}catch(...)
							{
								g_Logger.Add(L"Exception handled in %s !", __FUNCTIONW__);
							}
							string strWeb = webStream.str();
							size_t headerEnd = strWeb.find("\r\n\r\n");
							if(headerEnd != string::npos)
							{
								headerEnd+=4;
								strWeb = strWeb.substr(headerEnd);
								UINT userId = 0;
								webStream.str(strWeb);
								webStream >> userId >> points;
								g_Logger.Add(L"Loaded vote points[%d] host[%S] path[%S]", points, shost.c_str(), spath.c_str());
							}
						}
						closesocket(s);
					}
				}
			}
		}
	}
	return points;
};

void CVoteReward::JobThread(LPVOID params)
{
	CVoteReward& m_This = g_VoteReward;
	string host, path;
	size_t pos = m_This.m_HttpRequestUrl.find("/");
	if(pos!=string::npos)
	{
		host = m_This.m_HttpRequestUrl.substr(0, pos);
		path = m_This.m_HttpRequestUrl.substr(pos);
	}

	g_Logger.Add(L"[%s] Starting with params host[%S] path[%S]", __FUNCTIONW__, host.c_str(), path.c_str());
	while(m_This.m_TerminateThread == false)
	{
		vector<VoteJob> jobs;
		m_This.m_Lock.Enter();
		jobs = m_This.m_PendingJobs;
		m_This.m_PendingJobs.clear();
		m_This.m_Lock.Leave();

		for(UINT n=0;n<jobs.size();n++)
		{
			UINT startTime = time(0);
			stringstream pathStream;
			pathStream << path << "?serverId=" << m_This.m_ServerId << "&secretKey=" << m_This.m_SecretKey << "&accountName=" << base64_encode((const unsigned char*)jobs[n].accountName.c_str(), (UINT)(jobs[n].accountName.size()));
			INT32 points = GetPointsHelper(host, pathStream.str());

			if(points > 0)
			{
				if(User *pUser = g_UserDB.GetUserByDBID(jobs[n].userId))
				{
					pUser->pED->voteRewardPoints += points;
					pUser->pED->voteRewardMsgRequest = TRUE;
				}
			}
			UINT endTime = time(0);
			g_Logger.Add(L"Refreshing vote points[%d] for account[%S] user[%d], start[%d] diffTime[%d], host[%S] path[%S]", points, jobs[n].accountName.c_str(), jobs[n].userId, startTime, endTime - startTime, host.c_str(), pathStream.str().c_str());
		}

		Sleep(1000);
	}
	_endthread();
}

void CVoteReward::LoadData()
{
	const TCHAR* sectionName = _T("VoteReward");
	m_Enabled = GetPrivateProfileInt(sectionName, _T("Enabled"), 0, g_ConfigFile);
	TCHAR temp[8190] = { 0 };

	m_RequiredPoints = GetPrivateProfileInt(sectionName, _T("RequiredVotePoints"), 0, g_ConfigFile);
	m_PendingJobLimit = GetPrivateProfileInt(sectionName, _T("PendingJobLimit"), 50, g_ConfigFile);
	m_ServerId = GetPrivateProfileInt(sectionName, _T("ServerId"), 0, g_ConfigFile);
	if(GetPrivateProfileString(sectionName, _T("HttpRequestUrl"), 0, temp, 8190, g_ConfigFile))
	{
		m_HttpRequestUrl = WStringToString(temp);
		if(m_HttpRequestUrl.find("http://") == 0)
		{
			m_HttpRequestUrl = m_HttpRequestUrl.substr(7);
		}
	}
	if(GetPrivateProfileString(sectionName, _T("SecretKey"), 0, temp, 8190, g_ConfigFile))
	{
		m_SecretKey = WStringToString(temp);
	}
	if(GetPrivateProfileString(sectionName, _T("RewardList"), 0, temp, 8190, g_ConfigFile))
	{
		wstringstream itemStream(Utils::ReplaceString(temp, L";", L" ", true));
		VoteRewardItem vi;
		while(itemStream >> vi.itemId >> vi.amount)
		{
			m_Rewards.push_back(vi);
		}
	}
}

void CVoteReward::OnLoad(User *pUser)
{
	if(m_Enabled)
	{
		//2448	1	Vote points: $s1.	0	79	9B	B0	FF			0	0	0	0	0		none
		//2449	1	The Vote Reward System is currently disabled.	0	79	9B	B0	FF			0	0	0	0	0		none
		CSystemMessage msg(2448);
		msg.AddNumber(pUser->pED->voteRewardPoints);
		pUser->SendSystemMessage(&msg);
	}
}

void CVoteReward::Exchange(User *pUser)
{
	if(pUser->ValidUser())
	{
		if(pUser->pED->voteRewardPoints > m_RequiredPoints && m_RequiredPoints > 0)
		{
			int amount = pUser->pED->voteRewardPoints / m_RequiredPoints;
			int usedPoints = (amount*m_RequiredPoints);
			pUser->pED->voteRewardPoints -= usedPoints;

			g_DB.RequestSaveVotePoints(pUser->pSocket->accountId, pUser->nDBID, pUser->pED->voteRewardPoints);

			if(pUser->IsNowTrade())
			{
				pUser->TradeCancel();
			}
			for(UINT n=0;n<m_Rewards.size();n++)
			{
				pUser->AddItemToInventory(m_Rewards[n].itemId, (amount*m_Rewards[n].amount));
			}
			g_Logger.Add(L"User[%s][%d] gets a vote reward, amount[%d], points left[%d].", pUser->pSD->wszName, pUser->nDBID, amount, pUser->pED->voteRewardPoints);
		}else
		{
			//2450	1	You've got not enough Vote Points to perform this action.	0	79	9B	B0	FF			0	0	0	0	0		none
			pUser->pSocket->SendSystemMessage(2450);
		}
	}
}

void CVoteReward::HandleCommand(User *pUser, wstring wParams)
{
	
	try
	{
		if(pUser->ValidUser())
		{
			wstringstream sstr;
			sstr << wParams;
			wstring cmd;
			sstr >> cmd;

			if(cmd == L"help" || cmd.size() == 0)
			{
				pUser->SendSystemMessage(L"Welcome to Vote Reward System.");
				pUser->SendSystemMessage(L".vote help - shows available vote commands.");
				pUser->SendSystemMessage(L".vote points - shows available vote points.");
				pUser->SendSystemMessage(L".vote exchange - exchanges vote points into a reward items.");
				pUser->SendSystemMessage(L".vote refresh - reloads vote points.");
			}else if ( cmd == L"points" )
			{
				//2448	1	Vote points: $s1.	0	79	9B	B0	FF			0	0	0	0	0		none
				CSystemMessage msg(2448);
				msg.AddNumber(pUser->pED->voteRewardPoints);
				pUser->SendSystemMessage(&msg);
			}else if ( cmd == L"exchange" )
			{
				Exchange(pUser);
			}else if ( cmd == L"refresh" )
			{
				if((pUser->pED->voteRewardTick + 300) < time(0))
				{
					if(m_HttpRequestUrl.size() > 0)
					{
						m_Lock.Enter();
						if(m_PendingJobLimit > 0 && m_PendingJobs.size() < m_PendingJobLimit)
						{
							bool isPending = false;
							for(UINT n=0;n<m_PendingJobs.size();n++)
							{
								if(m_PendingJobs[n].userId == pUser->nDBID)
								{
									isPending = true;
									break;
								}
							}
							if(isPending == false)
							{
								VoteJob vj;
								vj.userId = pUser->nDBID;
								vj.accountName = WStringToString(pUser->pSocket->accountName);
								m_PendingJobs.push_back(vj);
								pUser->pED->voteRewardTick = time(0);
							}
							m_Lock.Leave();
						}else
						{
							pUser->SendSystemMessage(L"Server is busy at the moment, please try later.");
						}
					}else
					{
						g_DB.RequestLoadVotePoints(pUser->pSocket->accountId, pUser->nDBID);
					}
				}else
				{
					pUser->SendSystemMessage(L"You can refresh your vote points once per 5 minutes.");
				}
			}
		}
	}catch(...)
	{
		EXCEPTION_LOG;
	}
}