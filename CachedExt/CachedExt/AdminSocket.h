#pragma once
#include <windows.h>

//vTable 505714h
class CAdminSocket /* : public CSocket */
{
public:
	void Send(UINT len, PCHAR buff);
};