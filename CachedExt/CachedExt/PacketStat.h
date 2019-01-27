#pragma once
#include <windows.h>
#include "Utils.h"

struct PacketStatistic
{
	UINT executeCount;
	UINT executeLongestTime;
	PacketStatistic() : executeCount(0), executeLongestTime(0) {};
};

class CPacketStat
{
	CSLock m_Lock;
	PacketStatistic m_Packets[16][256];
public:
	CPacketStat();
	~CPacketStat();
	void Init();
	void SetPacket(UINT id, UINT time);
};