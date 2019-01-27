#include "stdafx.h"
#include "L2Con.h"
#include "CryptFile.h"
#include "HWID.h"

/*
[IPprDomainforAuth]
GroupCount=3
;Url (separated with space) that return login ip/port in format ip:port, ex 127.0.0.1:2106 or login.lineage.ru:2106
IpPortUrlList=localhost/getIpPort.php 127.0.0.1/getIpPort.php

[Auth_0]
Address1=127.0.0.1
Address2=localhost

[Auth_1]
Address1=127.0.0.1
Address2=localhost

[Auth_2]
Address1=127.0.0.1
Address2=localhost

*/

CL2Con g_L2Con;

bool FileExists(const WCHAR *fileName)
{
    wifstream infile(fileName);
    return infile.good();
}

void CL2Con::Init(wstring path, UINT authPort)
{
	if(path.size())
	{
		if(FileExists(path.c_str()))
		{
			m_Initialized = TRUE;
			m_Loaded = FALSE;
			m_Address = 0;
			m_AuthPort = authPort;
			m_Template.clear();
			m_TemplateUrl.clear();
			LoadINI(path);
			GetLoginIpPort();
			if(m_Address == 0)
			{
				ResolveAddr();
			}

		//	wstringstream statusStream;
		//	UtB ip;
		//	ip.full = m_Address;
		//	statusStream << L"IP: " << (UINT)ip.part[0] << L"." << (UINT)ip.part[1] << L"." << (UINT)ip.part[2] << L"." << (UINT)ip.part[3] << L" " << L" port: " << m_AuthPort;
		//	MessageBox(0, statusStream.str().c_str(), L"STATUS", MB_OK);
			m_Loaded = TRUE;
		}
	}
}

void CL2Con::LoadINI(wstring path)
{
	VIRTUALIZER_START;

	CCryptFile cf;
	cf.Handle(path.c_str());
	const TCHAR* section = _T("IPorDomainForAuth");
	TCHAR config[260] = { 0 };
	GetCurrentDirectory(260, config);
	lstrcat(config, L"\\");
	lstrcat(config, cf.GetName());  
	INT32 groupCount = GetPrivateProfileInt(section, _T("GroupCount"), 0, config);
	for(int n = 0; n < groupCount; n++)
	{
		wstringstream sectionStream;
		sectionStream << L"Auth_" << n ;
		ConAddr ca;
		TCHAR temp[8190] = { 0 };
		if(GetPrivateProfileString(sectionStream.str().c_str(), _T("Address1"), 0, temp, 8190, config))
		{
			for(int m = 0;m < 260;m++)
			{
				if(temp[m] != 0 )
				{
					ca.addrStr1.push_back((CHAR)temp[m]);
				}else
				{
					break;
				}
			}
			ca.addr1 = 0;
		}
		if(GetPrivateProfileString(sectionStream.str().c_str(), _T("Address2"), 0, temp, 8190, config))
		{
			for(int m = 0;m < 260;m++)
			{
				if(temp[m] != 0 )
				{
					ca.addrStr2.push_back((CHAR)temp[m]);
				}else
				{
					break;
				}
			}
			ca.addr2 = 0;
		}
		m_Template.push_back(ca);
	}
	TCHAR tempUrl[8190] = { 0 };
	if(GetPrivateProfileString(section, _T("IpPortUrlList"), 0, tempUrl, 8190, config))
	{
		wstringstream urlStream(tempUrl);
		wstring url;
		while(urlStream >> url)
		{
			ConUrl cu;
			string str = WStringToString(url);
			if(str.find("http://") == 0)
			{
				str = str.substr(7);
			}
			size_t pos = str.find("/");
			if(pos!= string::npos)
			{
				cu.host = str.substr(0, pos);
				cu.path = str.substr(pos);
			}else
			{
				cu.host = str;
			}
			if(cu.host.size() > 0)
			{
				m_TemplateUrl.push_back(cu);
			}
		}
	}
	cf.Release();
	VIRTUALIZER_END;
}

struct ConnectData
{
	UINT addr;
	UINT port;
	bool valid;
	ConnectData() : addr(0), port(0), valid(0) {};
	ConnectData(UINT a, UINT p) : addr(a), port(p), valid(false) {};
};

bool CanConnect(UINT addrVal, UINT port)
{
	bool valid = false;
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(s != INVALID_SOCKET)
	{
		SOCKADDR_IN sockAddr;
		memset(&sockAddr,0, sizeof(sockAddr));
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(port);
		sockAddr.sin_addr.S_un.S_addr = addrVal;

		typedef int (WSAAPI * _fcon)(IN SOCKET s, IN const struct sockaddr FAR * name, IN int namelen);
		HMODULE hW = GetModuleHandle(L"Ws2_32.dll");
		if(hW)
		{
			_fcon fCon = (_fcon)GetProcAddress(hW, "connect");
			if(fCon)
			{
				int result = fCon(s,(SOCKADDR *)&sockAddr, sizeof(SOCKADDR_IN));
				if(result == 0)
				{
					valid = true;
				}else
				{
					valid = false;
				}
			}
		}		
		closesocket(s);
	}
	return valid;
}

struct ConnectDataEx
{
	string host;
	string path;
	UINT ip;
	UINT port;
	bool valid;
	ConnectDataEx() : ip(0), port(0), valid(0) {};
};

DWORD WINAPI GetIpPortThread(LPVOID lpConnectData)
{
	ConnectDataEx* lpCD = (ConnectDataEx*)lpConnectData;
	
	try
	{
		hostent *remoteHost = gethostbyname(lpCD->host.c_str());
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
							CHAR sHWID[33];
							memset(sHWID, 0, 33);
							memcpy(sHWID, g_HWID.GetHash(), 32);

							char buffer[8192];
							memset(buffer, 0, sizeof(buffer));
							int len = 0;

							string content = "module=ddos:getIP";
							content += "&";
							content += "hwid=";
							content += sHWID;
							char temp[16];
							_itoa_s(content.size(), temp, 16, 10);

							string request = "POST " + lpCD->path + " HTTP/1.1\r\n";
							request += "Host: " + lpCD->host + "\r\n";
							request += "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:23.0) Gecko/20100101 Firefox/23.0\r\n";
							request += "Accept-Language: pl-PL,pl;q=0.8,en-US;q=0.6,en;q=0.4\r\n";
							request += "Accept-Charset: ISO-8859-2,utf-8;q=0.7,*;q=0.7\r\n";
							request += "Cache-Control: max-age=0\r\n";
							request += "Content-Type: application/x-www-form-urlencoded\r\n";
							request += "Content-Length: ";
							request += temp;
							request += "\r\n";
							request += "Connection: close\r\n\r\n";
							request += content;


							char sendBuffer[8192];
							memset(sendBuffer, 0, sizeof(sendBuffer));
							strcpy_s(sendBuffer, 8192, request.c_str());
							send(s, sendBuffer, static_cast<int>(request.size()+1), 0);
							string header;
							int bytes = 0;
							try
							{
								while((bytes = recv(s, buffer, 8190, 0)) > 0 )
								{
									len += bytes;

									string code(buffer);
									size_t pos = code.find("\r\n\r\n");
									if(pos != string::npos)
									{
										header = code.substr(0, pos);
										pos += 4;
										if(header.find("Transfer-Encoding: chunked") != string::npos)
										{
											pos = code.find("\r\n", pos);
											pos += 2;
										}
										code = code.substr(pos);
									}
									
									webStream << code;

									if(buffer[bytes-1] == '\0')
										break;

									memset(buffer, 0, sizeof(buffer));
								}
							}catch(...)
							{
							//	EXCEPTION_LOG;
							}

							string strWeb = webStream.str();
							if(strWeb.size() > 4)
							{
								UtB tempIp;
								tempIp.full = 0;
								size_t pos = strWeb.find(":");
								while(pos != string::npos)
								{
									strWeb[pos] = ' ';
									pos = strWeb.find(":");
								}

								stringstream ipStream(strWeb);
								UINT temp;
								ipStream >> temp;
								if(temp == 1406895978)
								{

									string loginIp;
									ipStream >> loginIp >> temp;

									hostent *remoteIp = gethostbyname(loginIp.c_str());
									if(remoteIp)
									{
										hostent hostIp = *remoteIp;
										if(remoteIp)
										{
											if(hostIp.h_length)
											{
												tempIp.full = (*(u_long *)hostIp.h_addr_list[0]);
											}
										}
									}

									lpCD->ip = tempIp.full;									
									lpCD->port = temp;
									if(lpCD->ip > 0 && lpCD->port > 0)
									{
										lpCD->valid = true;
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
	//	EXCEPTION_LOG;
	}


	return 0;
};


bool GetLoginIpPortHelper(string host, string path, UINT& ip, UINT& port )
{
	DWORD threadId = 0;
	ConnectDataEx cde;
	cde.host = host;
	cde.path = path;
	
	HANDLE hThread = CreateThread(0, 0, GetIpPortThread, &cde, 0, &threadId);
	if(WAIT_TIMEOUT == WaitForSingleObject(hThread, 4000))
	{
		TerminateThread(hThread, 0);
	}
	CloseHandle(hThread);
	if(cde.valid)
	{
		ip = cde.ip;
		port = cde.port;
	}
	return cde.valid;
};

void CL2Con::GetLoginIpPort()
{
	m_Lock.Enter();
	for(UINT n=0;n<m_TemplateUrl.size();n++)
	{
		ConUrl& cu = m_TemplateUrl[n];
		UINT port = 0, ip = 0;
		GetLoginIpPortHelper(cu.host, cu.path, ip, port);
		if(ip > 0 && port > 0)
		{
			m_Address = ip;
			m_AuthPort = port;
			break;
		}
	}
	
	m_Lock.Leave();
}

DWORD WINAPI ValidateAddressThread(LPVOID lpConnectData)
{
	ConnectData* lpCD = (ConnectData*)lpConnectData;
	lpCD->valid = CanConnect(lpCD->addr, lpCD->port);
	return 0;
};


bool ValidateAddress(UINT addrVal, UINT port)
{
	DWORD threadId = 0;
	ConnectData connectData(addrVal, port);
	HANDLE hThread = CreateThread(0, 0, ValidateAddressThread, &connectData, 0, &threadId);
	if(WAIT_TIMEOUT == WaitForSingleObject(hThread, 3000))
	{
		TerminateThread(hThread, 0);
	}
	CloseHandle(hThread);
	return connectData.valid;
};

UINT CL2Con::ResolveAddrHelper(string addrStr, bool& valid)
{
	UINT addrVal = inet_addr(addrStr.c_str());
	if(addrVal == INADDR_NONE)
	{
		if(hostent* host = gethostbyname(addrStr.c_str()))
		{
			if(host->h_length)
			{
				in_addr addr;
				addr.s_addr = (*(u_long *)host->h_addr_list[0]);
				addrVal = addr.s_addr;
			}
		}
	}
	valid = ValidateAddress(addrVal, m_AuthPort);
	
	return addrVal;
}

void CL2Con::ResolveAddr()
{
	m_Lock.Enter();
	m_TestCount = 0;
	m_Dice = rand() % m_Template.size();
//	wstringstream statusStream;
	while((m_Address == 0 || m_Address == INADDR_NONE) && m_TestCount < m_Template.size())
	{
		//searching for other address
		m_Dice = m_Dice % m_Template.size();
		ConAddr& ca = m_Template[m_Dice];
		ca.addr1 = ResolveAddrHelper(ca.addrStr1, ca.valid1);
		UtB ip;
		ip.full = ca.addr1;
	//	statusStream << L"IPGroup: " << m_Dice << L" Address1: " << ca.addrStr1.c_str() << L" ResolvedAs: " << (UINT)ip.part[0] << L"." << (UINT)ip.part[1] << L"." << (UINT)ip.part[2] << L"." << (UINT)ip.part[3] << L" is online:" << ca.valid1 << endl;
		if(ca.valid1)
		{
			m_Address = ca.addr1;
		}
		else
		{
			ca.addr2 = ResolveAddrHelper(ca.addrStr2, ca.valid2);
			ip.full = ca.addr2;
		//	statusStream << L"IPGroup: " << m_Dice << L" Address2: " << ca.addrStr1.c_str() << L" ResolvedAs: " << (UINT)ip.part[0] << L"." << (UINT)ip.part[1] << L"." << (UINT)ip.part[2] << L"." << (UINT)ip.part[3] << L" is online:" << ca.valid1 << endl;
			if(ca.valid2)
			{
				m_Address = ca.addr2;
			}
		}
		m_TestCount++;
		m_Dice++;
	}
//	MessageBox(0, statusStream.str().c_str(), L"STATUS", MB_OK);
	
	m_Lock.Leave();
}

UINT CL2Con::GetWorkingAddress()
{
	if(!m_Loaded)
	{
		UINT waitCount = 0;
		while(waitCount < 600 && !m_Loaded)
		{
			Sleep(100);
		}
	}
	return m_Address;
}

UINT CL2Con::GetWorkingPort()
{
	if(!m_Loaded)
	{
		UINT waitCount = 0;
		while(waitCount < 300 && !m_Loaded)
		{
			Sleep(100);
		}
	}
	return m_AuthPort;
}