#include "StdAfx.h"
#include "LogControl.h"

/*
			System_black = 0,	//SYSTEM
			System_blue = 1,	//SYSTEM
			ErrorLog = 2,	//ERROR
			InLog = 6,		//IN
			DbLog = 7,
			AuditLog = 8,
			ChatLog = 9
*/
static bool bSaveSystemLog;
static bool bSaveErrorLog;
static bool bSaveInLog;
static bool bSaveDbLog;
static bool bSaveAuditLog;
static bool bSaveChatLog;

void CLogControl::Initialize()
{
	bSaveSystemLog = true;
	bSaveErrorLog = true;
	bSaveInLog = true;
	bSaveDbLog = true;
	bSaveAuditLog = true;
	bSaveChatLog = true;
	if(g_Config.IsSet(CConfig::LOG))
	{
		bSaveSystemLog = g_Config.LogInfo.IsSaveSystem();
		bSaveErrorLog = g_Config.LogInfo.IsSaveError();
		bSaveInLog = g_Config.LogInfo.IsSaveIn();
		bSaveDbLog = g_Config.LogInfo.IsSaveDb();
		bSaveAuditLog = g_Config.LogInfo.IsSaveAudit();
		bSaveChatLog = g_Config.LogInfo.IsSaveChat();
		//Unicode
		g_HookManager.WriteCall( 0x64D81F, HandleSystem, 1);
		g_HookManager.WriteCall( 0x64D9F2, HandleError, 1);
		g_HookManager.WriteCall( 0x64D997, HandleIn, 1);
		g_HookManager.WriteCall( 0x64D939, HandleDb, 1);
		g_HookManager.WriteCall( 0x64D8DB, HandleAudit, 1);
		g_HookManager.WriteCall( 0x64D87D, HandleChat, 1);
		//Ansi
		g_HookManager.WriteCall( 0x64D07A, HandleSystem, 1);
		g_HookManager.WriteCall( 0x64D22F, HandleError, 1);
		g_HookManager.WriteCall( 0x64D1DA, HandleIn, 1);
		g_HookManager.WriteCall( 0x64D182, HandleDb, 1);
		g_HookManager.WriteCall( 0x64D12A, HandleAudit, 1);
		g_HookManager.WriteCall( 0x64D0D2, HandleChat, 1);

		if(g_Config.LogInfo.IsBlockOrtReport())
		{
			g_HookManager.WriteMemoryDWORD(0xA09218, (DWORD)CLogControl::OrtReport);
			g_HookManager.WriteMemoryDWORD(0xA09988, (DWORD)CLogControl::OrtReport);
			g_HookManager.WriteMemoryDWORD(0xA0A0F8, (DWORD)CLogControl::OrtReport);
			g_HookManager.WriteMemoryDWORD(0xA0A888, (DWORD)CLogControl::OrtReport);
			g_HookManager.WriteMemoryDWORD(0xA0B028, (DWORD)CLogControl::OrtReport);
			g_HookManager.WriteMemoryDWORD(0xA0B798, (DWORD)CLogControl::OrtReport);
			//npc ort report
			g_HookManager.WriteMemoryDWORD(0x988FE8, (DWORD)CLogControl::OrtReport);
			g_HookManager.WriteMemoryDWORD(0xA1AC28, (DWORD)CLogControl::OrtReport);
			g_HookManager.WriteMemoryDWORD(0xA28A48, (DWORD)CLogControl::OrtReport);
			g_HookManager.NopMemory(0x6BB061, 5);
			g_HookManager.WriteMemoryDWORD(0xA4A778, (DWORD)CLogControl::OrtReport);
			g_HookManager.WriteMemoryDWORD(0xA4B0E8, (DWORD)CLogControl::OrtReport);
			g_HookManager.WriteMemoryDWORD(0xAA0A38, (DWORD)CLogControl::OrtReport);
			//npc ort report end
			g_HookManager.WriteMemoryDWORD(0xA87228, (DWORD)CLogControl::OrtReport);
		}
	}
}

void CLogControl::OrtReport(LPVOID pClass, char* sCaller, int nWorkType)
{

}

void CLogControl::HandleSystem(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	if(bSaveSystemLog)
	{
		WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
	}else
	{
		(*lpNumberOfBytesWritten) = 0;
		return;
	}
}

void CLogControl::HandleError(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	if(bSaveErrorLog)
	{
		WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
	}else
	{
		*lpNumberOfBytesWritten = 0;
		return;
	}
}

void CLogControl::HandleIn(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	if(bSaveInLog)
	{
		WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
	}else
	{
		*lpNumberOfBytesWritten = 0;
		return;
	}
}

void CLogControl::HandleDb(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	if(bSaveDbLog)
	{
		WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
	}else
	{
		*lpNumberOfBytesWritten = 0;
		return;
	}
}

void CLogControl::HandleAudit(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	if(bSaveAuditLog)
	{
		WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
	}else
	{
		*lpNumberOfBytesWritten = 0;
		return;
	}
}

void CLogControl::HandleChat(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	if(bSaveChatLog)
	{
		WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
	}else
	{
		*lpNumberOfBytesWritten = 0;
		return;
	}
}