#include "StdAfx.h"
#include "OnlineMultipler.h"

COnlineMultipler g_OnlineMultipler;

COnlineMultipler::COnlineMultipler(void)
{
	m_lpCurrentUsers = (LPINT)0x1557E0C;
	m_lpPlayingUsers = (LPINT)0x1557E10;
	m_lpMaxUsers = (LPINT)0x1557E1C;
	m_lpLetBuilder = (LPINT)0x1557E20;
}

COnlineMultipler::~COnlineMultipler(void)
{
}

int COnlineMultipler::GetCurrentUsers()
{
	return (*m_lpCurrentUsers);
}

void COnlineMultipler::SetCurrentUsers(int Value)
{
	(*m_lpCurrentUsers) = Value;
}

int COnlineMultipler::GetPlayingUsers()
{
	return (*m_lpPlayingUsers);
}

void COnlineMultipler::SetPlayingUsers(int Value)
{
	(*m_lpPlayingUsers) = Value;
}

int COnlineMultipler::GetMaxUsers()
{
	return (*m_lpMaxUsers);
}

void COnlineMultipler::SetMaxUsers(int Value)
{
	(*m_lpMaxUsers) = Value;
}

void COnlineMultipler::LoadINI()
{
	const TCHAR* section = _T("OnlineMultipler");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile);
	m_MaxMultipler = GetPrivateProfileDouble(section, _T("MaxMultipler"), 1.0, g_ConfigFile);
	m_MaxFixed = GetPrivateProfileDouble(section, _T("MaxFixed"), 0.0, g_ConfigFile);
	m_CurrentMultipler = GetPrivateProfileDouble(section, _T("CurrentMultipler"), 1.0, g_ConfigFile);
	m_CurrentFixed = GetPrivateProfileDouble(section, _T("CurrentFixed"), 0.0, g_ConfigFile);
	m_PlayingMultipler = GetPrivateProfileDouble(section, _T("PlayingMultipler"), 1.0, g_ConfigFile);
	m_PlayingFixed = GetPrivateProfileDouble(section, _T("PlayingFixed"), 0.0, g_ConfigFile);
	m_PrivateStoreMultipler = GetPrivateProfileDouble(section, _T("PrivateStoreMultipler"), 1.0, g_ConfigFile);
	m_PrivateStoreFixed = GetPrivateProfileDouble(section, _T("PrivateStoreFixed"), 0.0, g_ConfigFile);
}

void COnlineMultipler::Init()
{
	LoadINI();
	if(m_Enabled)
	{
		BYTE OrgBytes1[5] = {0xE8, 0x67, 0x1F, 0xFD, 0xFF};
		g_HookManager.WriteCall(0x549DD4, COnlineMultipler::AuthUserNum, 0, OrgBytes1);	//AuthRequestUserNum
		BYTE OrgBytes2[5] = {0xE8, 0xBF, 0xFD, 0xFF, 0xFF};
		g_HookManager.WriteCall(0x51BF7C, COnlineMultipler::AuthUserNum, 0, OrgBytes2);	//AuthSendServerVersion
		g_HookManager.WriteCall(0x878576, COnlineMultipler::SendRemoteWho);
		g_HookManager.WriteCall(0x8785EB, COnlineMultipler::SendRemoteWho);
	}
}

void COnlineMultipler::AuthUserNum(LPVOID lpInstance, int nCurrent, int nMax)
{
	double current = static_cast<double>(nCurrent);
	current += g_OnlineMultipler.m_CurrentFixed;
	current *= g_OnlineMultipler.m_CurrentMultipler;
	nCurrent = static_cast<int>(current);

	double max = static_cast<double>(nMax);
	max += g_OnlineMultipler.m_MaxFixed;
	max *= g_OnlineMultipler.m_MaxMultipler;
	nMax = static_cast<int>(max);
	
	typedef void (*f)(LPVOID, int, int);
	f(0x51BD40)(lpInstance, nCurrent, nMax);
}

void COnlineMultipler::SendRemoteWho(CSocket *pSocket, const char* format, BYTE opCode, int param1, int max, int current, int playing, int privateStore, PWCHAR wStr, int param7, int param8, int param9, int param10, int param11)
{
	double temp = static_cast<double>(current);
	temp += g_OnlineMultipler.m_CurrentFixed;
	temp *= g_OnlineMultipler.m_CurrentMultipler;
	current = static_cast<int>(temp);

	temp = static_cast<double>(max);
	temp += g_OnlineMultipler.m_MaxFixed;
	temp *= g_OnlineMultipler.m_MaxMultipler;
	max = static_cast<int>(temp);

	temp = static_cast<double>(playing);
	temp += g_OnlineMultipler.m_PlayingFixed;
	temp *= g_OnlineMultipler.m_PlayingMultipler;
	playing = static_cast<int>(temp);

	temp = static_cast<double>(privateStore);
	temp += g_OnlineMultipler.m_PrivateStoreFixed;
	temp *= g_OnlineMultipler.m_PrivateStoreMultipler;
	privateStore = static_cast<int>(temp);

	pSocket->Send(format, opCode, param1, max, current, playing, privateStore, wStr, param7, param8, param9, param10, param11);
}