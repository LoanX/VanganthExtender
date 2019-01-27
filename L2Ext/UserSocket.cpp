#include "StdAfx.h"
#include "UserSocket.h"
#include "SocketDB.h"

void CUserSocket::ExtInit()
{
	BYTE bytesUserSocketConstructor[5] = {0xE8, 0x20, 0xFC, 0xFF, 0xFF};
	g_HookManager.WriteCall(0x8903BB, CUserSocket::OnConstructor, 0, bytesUserSocketConstructor);
	BYTE bytesUserSocketDestructor[4] = {0x00, 0x6F, 0x87, 0x00};
	g_HookManager.WriteMemoryDWORD(0xA8EE90, (DWORD)CUserSocket::OnDestructor, bytesUserSocketDestructor );

	//.text:000000000089036A mov     edx, 470h
	g_HookManager.WriteMemoryDWORD(0x89036B, sizeof(CUserSocket));
	//.text:00000000008903A0 mov     dword ptr [rbp+rcx*4+6C52320h], 470h
	g_HookManager.WriteMemoryDWORD(0x8903A7, sizeof(CUserSocket));

	//Check for amped
	//AmpeDx64.dll:000000000016DE7D mov     ss:dword_6C52320[rbp+rcx*4], 478h
	//16DE84 
	//.text:000000000089036F jmp     loc_16DE39
	//E9 C5 DA 8D FF
	//Org L2Server .text:000000000089036F lea     rcx, cs:1557E00h
	//48 8D 0D 8A 7A CC 00
	BYTE OrgBytes[5] = { 0x48, 0x8D, 0x0D, 0x8A, 0x7A };
	DWORD ampedAddr = (DWORD)GetModuleHandle(_T("AmpeDx64.dll"));
	if(memcmp(OrgBytes, (LPBYTE)0x89036F, 5))
	{
		g_HookManager.WriteMemoryDWORD((ampedAddr + 0xDE84), sizeof(CUserSocket));
	}
}

CUserSocket* CUserSocket::OnConstructor(CUserSocket* pSocket, SOCKET s)
{
	typedef CUserSocket* (*f)(CUserSocket*, SOCKET s);
	CUserSocket *pNewSocket = f(0x88FFE0L)(pSocket, s);

	if(pNewSocket)
	{
		sockaddr_in addr;
		INT32 addrLen = sizeof(addr);
		getpeername(s, (sockaddr*)&addr, &addrLen);
		CSocketData socketData;
		CIPAddress ip(addr);
		socketData.SetIp(ip);
		g_SocketDB.AddSocket(pSocket, socketData);
		SocketExtData *pData = new SocketExtData;
		if(pData)
		{
			pData->itemActionTimestamp = 0;
			pData->ioReadCount = 0;
			pData->ioReadTimeout = 0;
			pData->onReadTimeout = 0;
			pData->abd.count = 0;
			pData->abd.done = FALSE;
			pData->abd.firstPacket = TRUE;
			pData->abd.requestTimeout = 0;
			pData->abd.startIndex = g_Random.RandInt(724);
			pData->abd.confirmationCode1 = GetTickCount();
			pData->abd.confirmationCode2 = 0;
		//	pData->abd.hardwareId = 0;
			memset(pData->inKeyEx, 0, 4);
			pData->invalidWriteTime = 0;
			pData->packetCounter = 1;
			pData->shortHWID = 0;
			pNewSocket->pED = pData;
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot allocate SocketExtData!", __FUNCTION__);
		}
	}

	return pNewSocket;
}

CUserSocket *CUserSocket::OnDestructor(CUserSocket *pSocket, DWORD Data)
{
	g_SocketDB.DeleteSocket(pSocket);
	if(pSocket->pED)
	{
		delete pSocket->pED;
		pSocket->pED = 0;
	}
	typedef CUserSocket*(*f)(CUserSocket*, DWORD);
	return f(0x00876F00L)(pSocket, Data);
}

void CUserSocket::SendSystemMessage(int nMsgID)
{
	if(this)
	{
		typedef void (*f)(CSocket*, int nMsgID);
		f(0x00410920L)(this, nMsgID);
	}
}

void CUserSocket::SendSystemMessage(PWCHAR wszMsg)
{
	if(this)
	{
		Send("cdddS", 0x64, 614, 1, 0, wszMsg);
	}
}

void CUserSocket::SendSystemMessage(const wchar_t* wszMsg)
{
	if(this)
	{
		Send("cdddS", 0x64, 614, 1, 0, wszMsg);
	}
}

void CUserSocket::SendSystemMessage_S(int nMsgID, PWCHAR wsParam)
{
	if(this)
	{
		Send("cdddS", 0x64, nMsgID, 1, 0, wsParam);
	}
}

void CUserSocket::SendSystemMessage(CSystemMessage* pMsg)
{
	if(this && pMsg)
	{
		Send("b", pMsg->GetSize(), pMsg->GetBuff());
	}
}

void CUserSocket::ShowHTML(const WCHAR *wHtml, int classId)
{
	Send("cdSd", 0xF, 0, wHtml, classId);
}

User *CUserSocket::GetUser()
{
	if(this)
	{
		try
		{
			if(pUser->ValidUser())
				return pUser;
		}
		catch(...)
		{
			return NULL;
		}
	}
	return NULL;
}

void CUserSocket::Close()
{
	if(this)
	{
		typedef void (*f)(CUserSocket*);
		f(0x85DEF0L)(this);
	}
}