#include "stdafx.h"
#include "NetworkHandler.h"
#include "Packet.h"
#include "MemoryProtector.h"
#include "HWID.h"
#include "NWindow.h"
#include "Crypt.h"
#include "L2Con.h"
#include "AntiBot.h"
#include "JobHandler.h"

CNetworkHandler g_NetworkHandler;
extern HMODULE g_Engine;
extern BYTE g_MD5Checksum[32];
PVOID g_UNetworkHandler_OnTick = 0;
PVOID g_GameEngine_OnTick = 0;
PVOID g_UNetworkHandler_Handle = 0;
PVOID g_UNetworkHandler_OnConnect = 0;

#pragma optimize("", off)
void CNetworkHandler::Init()
{
	VIRTUALIZER_START;
	UINT baseAddress = reinterpret_cast<UINT>(g_Engine);
	Memory::WriteDWORD((baseAddress + 0x5B228C), (DWORD)CNetworkHandler::OnTick);	//?Tick@UNetworkHandler@@UAEXM@Z
	g_UNetworkHandler_OnTick = (PVOID)(baseAddress + 0x1218B0);

	Memory::WriteJump((baseAddress + 0x13D8B), CNetworkHandler::OnGameTick);	//?Tick@UGameEngine@@UAEXM@Z
	g_GameEngine_OnTick = (PVOID)(baseAddress + 0x297290);

	for(UINT n=0;n<256;n++)
	{
		m_CustomHandler[n] = CPHDummyPacket;
	}

	m_Initialized = FALSE;
	m_RequestQuitTime = 0;
	m_RequestQuit = FALSE;
	m_PingExTime = time(0);
	m_RequestQuitModuleName[0] = 0;

	InitAntiBot();

	VIRTUALIZER_END;
}

void CNetworkHandler::Bind(UINT id, PacketHandler func)
{
	if(id < 256)
	{
		m_CustomHandler[id] = func;
	}
}

void CNetworkHandler::SeparatedTick(void *args)
{
	VIRTUALIZER_START;

	bool quit = false;
	while(!quit)
	{
		time_t currentTime = time(0);
		if( (currentTime % 14) == 0 )
		{
			g_MemoryProtector.ValidateRunningApps();
		}else if((currentTime % 23) == 0 )
		{
			g_MemoryProtector.ValidateRunningAppsEx();
		}else if((currentTime % 10) == 0)
		{
			g_MemoryProtector.ValidateModules();
		}
		g_NetworkHandler.m_SeparatedTick = GetTickCount();
		Sleep(999);
	}

	_endthread();
	VIRTUALIZER_END;
}

int CNetworkHandler::OnTick(PVOID pInstance, float value)
{
	VIRTUALIZER_START;
	UINT currentTick = GetTickCount();
	if(currentTick > g_NetworkHandler.m_TickTimeout)
	{
		g_NetworkHandler.m_TickTimeout = currentTick + 900;
		if(g_NetworkHandler.m_Initialized == FALSE)
		{
			g_NetworkHandler.m_Initialized = TRUE;
			g_NetworkHandler.m_SeparatedTick = 0;
			UINT baseAddress = reinterpret_cast<UINT>(g_Engine);
			g_UNetworkHandler_Handle = (PVOID)(baseAddress + 0x12B7D0);
			g_UNetworkHandler_OnConnect = (PVOID)(baseAddress + 0x11C70);
			Memory::WriteJump((baseAddress + 0xE318), CNetworkHandler::HandlePacket);	//?AddNetworkQueue@UNetworkHandler@@UAEHPAUNetworkPacket@@@Z
			Memory::WriteCall((baseAddress + 0x122090), CNetworkHandler::OnConnect);
			g_NetworkHandler.Bind(1, CPHPingExPacket);
			g_NetworkHandler.Bind(2, CPHGetHWIDPacket);
			g_NetworkHandler.Bind(4, CPHOpenWebsitePacket);
			g_NetworkHandler.Bind(6, CPHRequestL2NetworkVoteTime);
			g_NetworkHandler.Bind(10, CPHPingPacket);
			g_NetworkHandler.Bind(11, CPHJobPacket);
			g_NetworkHandler.Bind(12, CPHMD5Packet);

			g_NWindow.Init();
			g_MemoryProtector.InitCliExt();
			g_MemoryProtector.ValidateRunningApps();
			
			WCHAR l2conINI[] = {L'L', L'2', L'C', L'o', L'n', L'.', L'i', L'n', L'i', 0 };
			g_L2Con.Init(l2conINI, g_AuthPort);
			g_NetworkHandler.m_SeparatedTickHandle = (HANDLE)_beginthread(CNetworkHandler::SeparatedTick, 0, 0);
		}else
		{
			g_MemoryProtector.ValidateMemory();
			g_JobHandler.TimerExpired();
		}
	}
	
	VIRTUALIZER_END;
	typedef int (__thiscall*f)(PVOID, float);
	return f(g_UNetworkHandler_OnTick)(pInstance, value);
}

UINT g_GameTickTimeout = 0;

int CNetworkHandler::OnGameTick(PVOID pInstance, float value)
{
	VIRTUALIZER_START;
	UINT currentTick = GetTickCount();
	if(currentTick > g_GameTickTimeout)
	{
		g_GameTickTimeout = currentTick + 999;		
	}
	
	VIRTUALIZER_END;
	typedef int (__thiscall*f)(PVOID, float);
	return f(g_GameEngine_OnTick)(pInstance, value);
}

void __thiscall CNetworkHandler::OnConnect(PVOID pInstance, const char* sAddress, UINT port)
{
	if(g_L2Con.IsInitialized())
	{
		UtB addr;
		addr.full = g_L2Con.GetWorkingAddress();
		port = g_L2Con.GetWorkingPort();

		stringstream ipStream;
		ipStream << (UINT)addr.part[0] << "." << (UINT)addr.part[1] << "." << (UINT)addr.part[2] << "." << (UINT)addr.part[3];

		typedef void (__thiscall*f)(PVOID, const char*, UINT);
		((f)g_UNetworkHandler_OnConnect)(pInstance, ipStream.str().c_str(), port);
	}else
	{
		typedef void (__thiscall*f)(PVOID, const char*, UINT);
		((f)g_UNetworkHandler_OnConnect)(pInstance, sAddress, port);
	}
}

int __thiscall CNetworkHandler::HandlePacket(NHandler *pNetwork, NPacket *pPacket)
{
	VIRTUALIZER_START;
	int ret = 0;

	if(pPacket->id == 0x87)
	{
		//handle custom packets
		CInPacket pck(pPacket->len, pPacket->buff);
		UINT opCode = pck.ReadD();
		if(opCode < 256)
		{
			ret = g_NetworkHandler.m_CustomHandler[opCode](pNetwork, pck);
		}
	}else
	{
		if(pPacket->id == 0x00 && pPacket->len == 25)
		{
			//decrypt key in packet
			//cQddb
			LPBYTE lpKey = (LPBYTE)&pPacket->buff[1];
			LPBYTE lpKeyMap = (LPBYTE)&pPacket->buff[17];

			static const BYTE xorKey[32][8] = { 0xc2, 0xc7, 0xd4, 0xb, 0xe1, 0xc8, 0xb5, 0xb1, 0x11, 0xca, 0x3a, 0xc, 0xb9, 0x5d, 0xa8, 0xe0, 0x21, 0x71, 0x8f, 0xdc, 0xe2, 0xd4, 0xfc, 0xdf, 0x9b, 0x84, 0x2d, 0x3c, 0xc6, 0x8, 0x6, 0x4b, 0xcf, 0x7a, 0x92, 0x15, 0x72, 0x83, 0xf5, 0xba, 0x4e, 0xa4, 0xca, 0x53, 0xf4, 0x5f, 0x9c, 0xaa, 0x22, 0x86, 0x64, 0xed, 0x79, 0x1d, 0xa5, 0x82, 0xa2, 0x4c, 0x1c, 0xfc, 0xde, 0xa5, 0x9a, 0xc2, 0x0, 0x5b, 0x18, 0xf3, 0xc, 0x7c, 0x69, 0x5d, 0x40, 0xc, 0xca, 0x5, 0xcf, 0xf2, 0x99, 0x11, 0x12, 0x80, 0x75, 0x97, 0x79, 0xa3, 0x1d, 0x8, 0x19, 0x90, 0x44, 0xea, 0xee, 0xe0, 0xbc, 0x76, 0xe3, 0xe8, 0x16, 0xce, 0x85, 0x68, 0x31, 0x6f, 0x90, 0x39, 0xd6, 0x85, 0x5b, 0x22, 0xcf, 0xd0, 0xf9, 0x86, 0x88, 0xbd, 0x6e, 0x6, 0xd6, 0x50, 0x99, 0xb0, 0xd6, 0xab, 0x42, 0x25, 0x6a, 0xa4, 0xfb, 0xf3, 0x6a, 0x4c, 0x23, 0xc4, 0x17, 0xd5, 0xd4, 0xa5, 0xb8, 0xc0, 0x12, 0xb6, 0x10, 0xaf, 0xc8, 0xa, 0x8e, 0xce, 0x50, 0xa1, 0xf3, 0x42, 0xb2, 0x4d, 0x32, 0x6e, 0x85, 0x9f, 0x4b, 0x95, 0xa9, 0x8c, 0x1b, 0x9e, 0x8b, 0xd8, 0x8c, 0x7e, 0x90, 0x19, 0x5c, 0x2e, 0xd4, 0x48, 0xd3, 0x79, 0x58, 0xc0, 0xa1, 0xbf, 0x77, 0x9e, 0x38, 0xcb, 0xd5, 0x4d, 0xd3, 0xee, 0xcf, 0x50, 0xad, 0x9e, 0x39, 0x11, 0x50, 0x80, 0xd4, 0xaf, 0xaa, 0x59, 0x24, 0xa1, 0xd7, 0xc2, 0xf5, 0x38, 0x43, 0x7, 0x68, 0xaa, 0x3, 0x13, 0xab, 0xf1, 0x11, 0xe6, 0x5d, 0xe3, 0x69, 0x6c, 0xa0, 0xf8, 0x8c, 0x16, 0xde, 0x24, 0x63, 0x34, 0x71, 0x1d, 0x32, 0x64, 0xde, 0x96, 0x6b, 0xb, 0x21, 0xaa, 0x29, 0x32, 0x9d, 0x12, 0x19, 0xc9, 0x1c, 0x52, 0x93, 0x90, 0x8b, 0x8e, 0xc4, 0xaf, 0xde, 0x23, 0x83, 0x5e };
			BYTE keyMap[8] = { 0 };
			for(UINT n=0;n<8;n++)
			{
				lpKey[n] ^= xorKey[lpKeyMap[n]][n];
			}
		}

		typedef int (__thiscall *f)(NHandler*, NPacket*);
		ret = f(g_UNetworkHandler_Handle)(pNetwork, pPacket);
	}

	VIRTUALIZER_END;
	return ret;
}

string WINAPI CNetworkHandler::SendPostRequest(string sHost, string sPath, string params)
{
	VIRTUALIZER_START;
	string reply = "";
	try
	{
		hostent *remoteHost = gethostbyname(sHost.c_str());
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

							string content = params;
							char temp[16];
							_itoa_s((INT32)content.size(), temp, sizeof(temp), 10);

							char _post[] = { 'P', 'O', 'S', 'T', ' ', 0 };
							char _http[] = { ' ', 'H', 'T', 'T', 'P', '/', '1', '.', '1', '\r', '\n', 0 };
							char _host[] = {'H', 'o', 's', 't', ':', ' ', 0};
							char _rn[] = { '\r', '\n', 0 };
							char _userAgent[] = { 'U', 's', 'e', 'r', '-', 'A', 'g', 'e', 'n', 't', ':', ' ',
								'M', 'o', 'z', 'i', 'l', 'l', 'a', '/', '5', '.', '0', ' ',
								'(', 'W', 'i', 'n', 'd', 'o', 'w', 's', ' ', 'N', 'T', ' ', '6', '.', '1', ';',
								' ', 'W', 'O', 'W', '6', '4', ';', ' ', 'r', 'v', ':', '2', '3', '.', '0', ')',
								' ', 'G', 'e', 'c', 'k', 'o', '/', '2', '0', '1', '0', '0', '1', '0', '1', ' ',
								'F', 'i', 'r', 'e', 'f', 'o', 'x', '/', '2', '3', '.', '0', 0 };
							char _acceptLanguage[] = { 'A', 'c', 'c', 'e', 'p', 't', '-', 'L', 'a', 'n', 'g', 'u', 'a', 'g', 'e', ':', ' ', 'p', 'l', '-', 'P', 'L', ',', 'p', 'l', ';', 'q', '=', '0', '.', '8', ',', 'e', 'n', '-', 'U', 'S', ';', 'q', '=', '0', '.', '6', ',', 'e', 'n', ';', 'q', '=', '0', '.', '4', 0 };
							char _acceptCharset[] = { 'A', 'c', 'c', 'e', 'p', 't', '-', 'C', 'h', 'a', 'r', 's', 'e', 't', ':', ' ', 'I', 'S', 'O', '-', '8', '8', '5', '9', '-', '2', ',', 'u', 't', 'f', '-', '8', ';', 'q', '=', '0', '.', '7', ',', '*', ';', 'q', '=', '0', '.', '7', 0 };
							char _cacheControl[] = { 'C', 'a', 'c', 'h', 'e', '-', 'C', 'o', 'n', 't', 'r', 'o', 'l', ':', ' ', 'm', 'a', 'x', '-', 'a', 'g', 'e', '=', '0', 0 };
							char _contentType[] = { 'C', 'o', 'n', 't', 'e', 'n', 't', '-', 'T', 'y', 'p', 'e', ':', ' ', 'a', 'p', 'p', 'l', 'i', 'c', 'a', 't', 'i', 'o', 'n', '/', 'x', '-', 'w', 'w', 'w', '-', 'f', 'o', 'r', 'm', '-', 'u', 'r', 'l', 'e', 'n', 'c', 'o', 'd', 'e', 'd', 0 };
							char _contentLength[] = { 'C', 'o', 'n', 't', 'e', 'n', 't', '-', 'L', 'e', 'n', 'g', 't', 'h', ':', ' ', 0 };
							char _connectionClose[] = { 'C', 'o', 'n', 'n', 'e', 'c', 't', 'i', 'o', 'n', ':', ' ', 'c', 'l', 'o', 's', 'e', 0 };
							char _transferEncoding[] = { 'T', 'r', 'a', 'n', 's', 'f', 'e', 'r', '-', 'E', 'n', 'c', 'o', 'd', 'i', 'n', 'g', ':', ' ', 'c', 'h', 'u', 'n', 'k', 'e', 'd', 0 };
							
							string request = _post + sPath + _http;
							request += _host + sHost + _rn;
							request += _userAgent + string(_rn);
							request += _acceptLanguage + string(_rn);
							request += _acceptCharset + string(_rn);
							request += _cacheControl + string(_rn);
							request += _contentType + string(_rn);
							request += _contentLength;
							request += temp;
							request += _rn;
							request += _connectionClose + string(_rn) + string(_rn);
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
									size_t pos = code.find(string(_rn) + string(_rn));
									if(pos != string::npos)
									{
										header = code.substr(0, pos);
										pos += 4;
										if(header.find(_transferEncoding) != string::npos)
										{
											pos = code.find(_rn, pos);
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

							reply = webStream.str();
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

	VIRTUALIZER_END;

	return reply;
};

int CPHDummyPacket(NHandler *pNetwork, CInPacket& packet)
{
//	MessageBox(0, L"DummyPacket", L"CustomPacket", MB_OK);
	return 0;
}

int CPHOpenWebsitePacket(NHandler *pNetwork, CInPacket& packet)
{
	VIRTUALIZER_START;

	wstring url = packet.ReadS();

	ShellExecute(0, 0, url.c_str(), 0, 0 , SW_SHOW );

	VIRTUALIZER_END;
	return 0;
}


int CPHPingExPacket(NHandler *pNetwork, CInPacket& packet)
{
	VIRTUALIZER_START;

	g_MemoryProtector.ValidateMemoryEx();

	UINT tickId = packet.ReadD();
	COutPacket pckA;
	pckA.WriteC(0x5);	//custom packet out opcode
	pckA.WriteD(1);	//PingEx opcode
	COutPacket pckB;
	pckB.WriteD(GetTickCount());
	pckB.WriteD(tickId);
	
	pckB.WriteD(g_NetworkHandler.GetTickTimeout());
	pckB.WriteD(g_NetworkHandler.GetSeparatedTick());
	pckB.WriteD(g_MemoryProtector.GetScanTick());

	pckB.WriteD(g_NetworkHandler.GetRequestQuit());
	pckB.WriteS(g_NetworkHandler.GetRequestQuitModuleName());
	pckA.WriteH(static_cast<USHORT>(pckB.GetLen()));
	CryptXor((LPBYTE)pckB.GetBuff(), pckB.GetLen());
	pckA.WriteB(pckB.GetBuff(), pckB.GetLen());

	pNetwork->pSocket->Send(pckA.GetLen(), pckA.GetBuff());

	VIRTUALIZER_END;
	return 0;
}

int CPHGetHWIDPacket(NHandler *pNetwork, CInPacket& packet)
{
	VIRTUALIZER_START;

	UINT hwid = g_HWID.Get();
	LPBYTE lpHWID = g_HWID.GetHash();

	COutPacket pck;
	pck.WriteC(0x5);
	pck.WriteD(2);	//HWID opcode
	pck.WriteD(hwid);
	for(UINT n=0;n<32;n++)
	{
		pck.WriteC(lpHWID[n]);
	}
	pck.WriteD(g_HWID.GetCheckSum());
	pNetwork->pSocket->Send(pck.GetLen(), pck.GetBuff());


	VIRTUALIZER_END;
	return 0;
}

int CPHMD5Packet(NHandler *pNetwork, CInPacket& packet)
{
	VIRTUALIZER_START;

	bool done = false;
	UINT actionId = packet.ReadD();
	
	COutPacket pck;
	pck.WriteC(0x05);
	pck.WriteD(12);	//md5 packet
	pck.WriteD(actionId);
	pck.WriteB((PCHAR)g_MD5Checksum, 32);

	pNetwork->pSocket->Send(pck.GetLen(), pck.GetBuff());

	VIRTUALIZER_END;
	return 0;
}


int CPHJobPacket(NHandler *pNetwork, CInPacket& packet)
{
	VIRTUALIZER_START;

	bool done = false;
	UINT jobId = 0;
	UINT jobType = packet.ReadD();
	
	if(jobType == JobTypeAdrenalinBot)
	{
		wstring pipeName = packet.ReadS();
		CJob *pJob = new CJob();
		CJobParamAdrenalinBot *pParam = new CJobParamAdrenalinBot();
		pParam->SetPipeName(pipeName.c_str());
		pJob->SetParam(pParam);
		pJob->SetFunction(DetectAdrenalinBot);

		g_JobHandler.AddJob(pJob);
		jobId = reinterpret_cast<UINT>(pJob);
		done = true;
	}

	if(done)
	{
		COutPacket pck;
		pck.WriteC(0x05);
		pck.WriteD(11);
		pck.WriteD(GetTickCount());
		pck.WriteD(jobId);
		pck.WriteD(time(0));	//current time
		pck.WriteD(g_JobHandler.GetExpireTime());
		pck.WriteD(g_JobHandler.GetJobCount());


		pNetwork->pSocket->Send(pck.GetLen(), pck.GetBuff());
	}

	VIRTUALIZER_END;
	return 0;
}

int CPHPingPacket(NHandler *pNetwork, CInPacket& packet)
{
	VIRTUALIZER_START;

	UINT serverTick = packet.ReadD();

	COutPacket pck;
	pck.WriteC(0x5);
	pck.WriteD(10);
	pck.WriteD(serverTick);
	pNetwork->pSocket->Send(pck.GetLen(), pck.GetBuff());


	VIRTUALIZER_END;
	return 0;
}

int CPHRequestL2NetworkVoteTime(NHandler *pNetwork, CInPacket& packet)
{
	VIRTUALIZER_START;

	string apiKey = packet.Reads();
	string identity = packet.Reads();
	string host = packet.Reads();
	string path = packet.Reads();
	UINT type = 2;

	stringstream pr;
	pr << "apiKey=" << apiKey << "&type=" << type << "&player=" << identity;

	string reply = CNetworkHandler::SendPostRequest(host, path, pr.str());

	stringstream replyStream(reply);

	INT32 voteTime = 0;
	if(replyStream >> voteTime)
	{
		//reply back to the server
		COutPacket pck;
		pck.WriteC(0x05);
		pck.WriteD(0x06);
		pck.WriteD(voteTime);
		pNetwork->pSocket->Send(pck.GetLen(), pck.GetBuff());
	}

	VIRTUALIZER_END;
	return 0;
}

#pragma optimize("", on)