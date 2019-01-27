#include "stdafx.h"
#include "VotePage.h"
#include "UserDb.h"

CVotePage::CVotePage() : m_enabled(FALSE), m_rewardIpCheck(FALSE), m_requiredPoints(50), m_rewardId(0), m_rewardCount(0), m_multiplyReward(FALSE), m_interval(300), m_basePoints(0), m_setBasePoints(TRUE), m_newPoints(0), m_checkTime(0)
{
	InitializeCriticalSection(&m_lock);
	static UINT id = 1;
	m_id = ++id;
};

CVotePage::~CVotePage()
{
	DeleteCriticalSection(&m_lock);
}

void CVotePage::Init(const TCHAR* section)
{
	m_enabled = false;
	m_setBasePoints = TRUE;
	LoadINI(section);
	_tcscpy_s(m_section, 8192, section);
}

void CVotePage::LoadCurrentPoints()
{
	TCHAR file[260];
	GetCurrentDirectory(MAX_PATH,file);
	lstrcat(file, TEXT("\\Vote.ini"));
	m_currentPoints = GetPrivateProfileInt(m_Section, _T("CurrentPoints"), 0, file);
}

void CVotePage::LoadINI(const TCHAR* section)
{
	TCHAR file[260];
	GetCurrentDirectory(MAX_PATH,file);
	lstrcat(file, TEXT("\\Vote.ini"));
	m_Section[0] = 0;
	wcscpy(m_Section, section);
	
	m_enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, file);
	m_requiredPoints = GetPrivateProfileInt(section, _T("RequiredPoints"), 50, file);
	m_interval = GetPrivateProfileInt(section, _T("CheckInterval"), 300, file);
	m_rewardId = GetPrivateProfileInt(section, _T("RewardId"), 57, file);
	m_rewardCount = GetPrivateProfileInt(section, _T("RewardCount"), 57, file);
	m_multiplyReward = GetPrivateProfileInt(section, _T("MultiplyRewardCountByVotes"), 57, file);
	m_rewardIpCheck = GetPrivateProfileInt(section, _T("RewardIpCheck"), 0, file);
	m_currentPoints = GetPrivateProfileInt(section, _T("CurrentPoints"), 0, file);
	TCHAR temp[8192];
	if(GetPrivateProfileString(section, _T("UrlHost"), 0, temp, 8190, file))
	{
		wstring wTmp(temp);
		m_urlHost.append(wTmp.begin(), wTmp.end());
	}
	if(GetPrivateProfileString(section, _T("UrlPath"), 0, temp, 8190, file))
	{
		wstring wTmp(temp);
		m_urlPath.append(wTmp.begin(), wTmp.end());
	}
	if(GetPrivateProfileString(section, _T("StartPointString"), 0, temp, 8190, file))
	{
		wstring wTmp(temp);
		m_startPointString.append(wTmp.begin(), wTmp.end());
	}
	if(GetPrivateProfileString(section, _T("EndPointString"), 0, temp, 8190, file))
	{
		wstring wTmp(temp);
		m_endPointString.append(wTmp.begin(), wTmp.end());
	}
	if(GetPrivateProfileString(section, _T("IntervalAnnounce"), 0, temp, 8190, file))
	{
		m_intervalAnnounce = temp;
	}
	if(GetPrivateProfileString(section, _T("IntervalAnnounceEx"), 0, temp, 8190, file))
	{
		m_intervalAnnounceEx = temp;
	}
	if(GetPrivateProfileString(section, _T("RewardAnnounce"), 0, temp, 8190, file))
	{
		m_rewardAnnounce = temp;
	}
}

void CVotePage::HandleNewPoints(INT32 newPoints)
{
	Lock();
	if(newPoints >= 0)
	{
		if(m_setBasePoints || newPoints < m_basePoints)
		{
			m_setBasePoints = FALSE;
			m_basePoints = newPoints;
		}

		m_newPoints = newPoints;
	}
	Unlock();
}

void CVotePage::TimerExpired()
{
	guard;
	if(m_enabled)
	{
		try
		{
			INT32 itemId = 0;
			INT32 itemCount = 0;
			if(m_newPoints >= 0)
			{
				Lock();

				if(m_newPoints > m_basePoints && m_setBasePoints == FALSE)
				{
					INT32 diff = m_newPoints - m_basePoints;
					if(diff >= m_requiredPoints)
					{
						diff /= m_requiredPoints;
						m_basePoints += m_requiredPoints * diff;
						//give out reward
						itemId = m_rewardId;
						itemCount = m_rewardCount;
						if(m_multiplyReward)
						{
							if(diff > 1)
							{
								itemCount *= diff;
							}
						}
						
					}
				}

				/* old calculation

				INT32 pointsDiff = m_newPoints - m_basePoints;

				if(pointsDiff >= m_requiredPoints)
				{
					//set new base points
					m_basePoints += m_requiredPoints;

					//give out reward
					itemId = m_rewardId;
					itemCount = m_rewardCount;
					if(m_multiplyReward)
					{
						itemCount *= pointsDiff;
					}
				}
				*/
				Unlock();

				if(itemId > 0 && itemCount > 0)
				{
					try
					{
						//announce
						tstring announce = m_rewardAnnounce;
						//; dynamic words : [currentPoints], [section], [missingPoints], [requiredPoints]
						TCHAR temp[16];
						_itot_s(m_newPoints, temp, 16, 10);
						announce = Utils::ReplaceString(announce, _T("[currentPoints]"), temp);
						INT32 missingPoints = m_requiredPoints - (m_newPoints - m_basePoints);
						_itot_s(missingPoints, temp, 16, 10);
						announce = Utils::ReplaceString(announce, _T("[missingPoints]"), temp);
						_itot_s(m_requiredPoints, temp, 16, 10);
						announce = Utils::ReplaceString(announce, _T("[requiredPoints]"), temp);
						announce = Utils::ReplaceString(announce, _T("[section]"), m_section);
						Utils::BroadcastToAllUser_Announce(announce.c_str());
						//give item to all users
						g_UserDB.GiveItemToAllUser(itemId, itemCount, m_rewardIpCheck);
					}catch(...)
					{
						EXCEPTION_LOG;
					}
				}

				time_t currentTime = time(NULL);
				if( m_checkTime < currentTime )
				{
					if(m_intervalAnnounce.size() > 0)
					{
						tstring announce = m_intervalAnnounce;
						//; dynamic words : [currentPoints], [section], [missingPoints], [requiredPoints]
						TCHAR temp[16];
						_itot_s(m_newPoints, temp, 16, 10);
						announce = Utils::ReplaceString(announce, _T("[currentPoints]"), temp);

						INT32 missingPoints = 0;
						if(m_newPoints > 0)
						{
							missingPoints = m_requiredPoints - (m_newPoints % m_requiredPoints);
							/* old calculation
							missingPoints = m_requiredPoints - (m_newPoints - m_basePoints);
							*/
						}
						if(missingPoints == 0)
						{
							missingPoints = m_requiredPoints;
						}
						_itot_s(missingPoints, temp, 16, 10);
						announce = Utils::ReplaceString(announce, _T("[missingPoints]"), temp);
						_itot_s(m_requiredPoints, temp, 16, 10);
						announce = Utils::ReplaceString(announce, _T("[requiredPoints]"), temp);
						announce = Utils::ReplaceString(announce, _T("[section]"), m_section);
						Utils::BroadcastToAllUser_Announce(announce.c_str());
					}
					if(m_intervalAnnounceEx.size() > 0)
					{
						tstring announce = m_intervalAnnounceEx;
						//; dynamic words : [currentPoints], [section], [missingPoints], [requiredPoints]
						TCHAR temp[16];
						_itot_s(m_newPoints, temp, 16, 10);
						announce = Utils::ReplaceString(announce, _T("[currentPoints]"), temp);
						INT32 missingPoints = 0;
						if(m_newPoints > 0)
						{
							missingPoints = m_requiredPoints - (m_newPoints % m_requiredPoints);
							/* old calculation
							missingPoints = m_requiredPoints - (m_newPoints - m_basePoints);
							*/
						}else
						{
							missingPoints = m_requiredPoints;
						}
						_itot_s(missingPoints, temp, 16, 10);
						announce = Utils::ReplaceString(announce, _T("[missingPoints]"), temp);
						_itot_s(m_requiredPoints, temp, 16, 10);
						announce = Utils::ReplaceString(announce, _T("[requiredPoints]"), temp);
						announce = Utils::ReplaceString(announce, _T("[section]"), m_section);
						Utils::BroadcastToAllUser_Announce(announce.c_str());
					}
					m_checkTime = currentTime + m_interval;
					try
					{
						_beginthread(CVotePage::CheckVotePoints, 0, this);
					}catch(...)
					{
						EXCEPTION_LOG;
					}
				}
			}
		}catch(...)
		{
			EXCEPTION_LOG;
		}
	}
	unguard;
}

void CVotePage::CheckVotePoints(LPVOID param)
{
	if(param)
	{
		try
		{
			CVotePage *lpVotePage = (CVotePage*)param;
			//start Vote.exe
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			ZeroMemory( &si, sizeof(si) );
			si.cb = sizeof(si);
			ZeroMemory( &pi, sizeof(pi) );
			WCHAR dir[260] = { 0 };
			GetCurrentDirectory(260, dir);

			WCHAR cmdLine[260];
			wcscpy_s(cmdLine, 260, dir);
			lstrcat(cmdLine, L"\\VoteCatcher.exe");
			// Start the child process. 
			if( CreateProcess( NULL,   // No module name (use command line)
				cmdLine,        // Command line
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				FALSE,          // Set handle inheritance to FALSE
				0,              // No creation flags
				NULL,           // Use parent's environment block
				dir,           // Use parent's starting directory 
				&si,            // Pointer to STARTUPINFO structure
				&pi )           // Pointer to PROCESS_INFORMATION structure
			) 
			{
				g_Log.Add(CLog::Blue, "[%S] Start: [%S]", L"VotePage", cmdLine);
				// Close process and thread handles. 
				CloseHandle( pi.hProcess );
				CloseHandle( pi.hThread );
			}
			lpVotePage->LoadCurrentPoints();
			lpVotePage->HandleNewPoints(lpVotePage->m_currentPoints);
			lpVotePage->Unlock();
		}catch(...)
		{
			EXCEPTION_LOG;
		}
	}
	_endthread();
}

INT32 CVotePage::GetVotePoints()
{
	INT32 points = -1;
	try
	{
		hostent *remoteHost = gethostbyname(m_urlHost.c_str());
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
							int len = 0;

							string request = "GET " + m_urlPath + " HTTP/1.0\r\n";
							request += "Host: " + m_urlHost + "\r\n";
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
								EXCEPTION_LOG;
							}

							string strWeb = webStream.str();
							ofstream file("html.htm");
							file << strWeb << endl;
							file.close();
							size_t pos = strWeb.find(m_startPointString);
							if(pos != string::npos)
							{
								pos += m_startPointString.size();
								if(m_startPointString.find(">") == string::npos)
								{
									pos = strWeb.find(">", pos);
									if(pos != string::npos)
									{
										pos++;
									}
								}
								if(pos != string::npos)
								{
									size_t endPos = strWeb.find("</", pos);
									if(endPos != string::npos)
									{
										string strPoints = strWeb.substr(pos, (endPos - pos));
										points = atoi(strPoints.c_str());
									}
								}
							}
						}
						closesocket(s);
					}
				}
			}
		}
	}catch(...)
	{
		EXCEPTION_LOG;
	}

	return points;
}