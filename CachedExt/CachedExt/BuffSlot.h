#pragma once
#include "CQuerySocket.h"

#define ABNORMAL_SLOT_COUNT 40

class CBuffSlot
{
public:
	static void Init();
	static bool __cdecl RequestSaveAbnormalStatus(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestLoadAbnormalStatus(CQuerySocket *pQuerySocket, const unsigned char* packet);
};
