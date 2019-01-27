#include "PacketHandler.h"
#include "CLog.h"

void UserObjectEvent_LogInvalidDelete(LPVOID pInstance, int nType, const wchar_t* format, const wchar_t* file, int Line, int UserId)
{
	return;
}

void User_AddReverseHate_NoActiveUser(LPVOID pInstance, int nType, const wchar_t* format, const wchar_t* file, int Line)
{
	return;
}

void CL2ServerPacketHandler::Init()
{
	WriteHook(CALL, 0x5D57CD, (DWORD)UserObjectEvent_LogInvalidDelete, 0);
	WriteHook(CALL, 0x6012A7, (DWORD)User_AddReverseHate_NoActiveUser, 0);
}