#include "stdafx.h"
#include "LoginDb.h"
#include "HookManager.h"

CLoginDb g_LoginDb;

void CLoginDb::Init()
{
	g_HookManager.WriteMemoryDWORD(0xBEFBF0, static_cast<UINT>(reinterpret_cast<UINT64>(CLoginDb::OnAuthRequestAboutToPlay)));
	g_HookManager.WriteJump(0x53E6CA, LoginFix);
}

void CLoginDb::SetLoginData(UINT accountId, wstring accountName)
{
	LoginData ld;
	ld.accountName = accountName;
	ld.timestamp = time(0) + 5;
	ld.used = FALSE;

	m_Lock.Enter();
	m_Accounts[accountId] = ld;
	m_Lock.Leave();
}

UINT CLoginDb::CanLogin(UINT accountId, wstring accountName)
{
	guard;
	UINT ret = 1;

	m_Lock.Enter();
	map<UINT, LoginData>::iterator it = m_Accounts.find(accountId);
	if(it!=m_Accounts.end())
	{
		LoginData& ld = it->second;
		if(/*accountName == ld.accountName */ true)
		{
			if(ld.used == FALSE)
			{
				if(ld.timestamp > time(0))
				{
					ret = 0;
				}else
				{
					ret = 3;
				}
				ld.used = TRUE;
			}else
			{
				ret = 2;
			}
		}
	}
	m_Lock.Leave();

	unguard;
	return ret;
}

bool CLoginDb::OnAuthRequestAboutToPlay(PVOID pSocket, const unsigned char* packet)
{
	guard;
	bool ret = false;

	//dsddddcddd
	UINT p1, p3, p4, p5, p6, p8, p9, p10;
	CHAR p2[25] = { 0 };
	BYTE p7 = 0;
	Disassemble(packet, "dsddddcddd", &p1, sizeof(p2), p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10);

//	g_Log.Add(LogBlue, "[%s] p1[%d] p2[%s] p3[%d] p4[%d] p5[%d] p6[%d] p7[%d] p8[%d] p9[%d] p10[%d]", __FUNCTION__, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);

	wstring accountName;
	for(UINT n=0;n<25;n++)
	{
		if(p2[n] != 0)
		{
			accountName.push_back((WCHAR)p2[n]);
		}else
		{
			break;
		}
	}

	g_LoginDb.SetLoginData(p1, accountName);


	typedef bool (*f)(PVOID, const unsigned char*);
	ret = f(0x58DF50L)(pSocket, packet);

	unguard;
	return ret;
}