#include "PacketStat.h"
#include "Utils.h"
#include "CLog.h"

CPacketStat::CPacketStat()
{

}

CPacketStat::~CPacketStat()
{
}

void CPacketStat::Init()
{
	
}

void CPacketStat::SetPacket(UINT id, UINT time)
{
	UINT index = GetThreadIndex();
	if(index < 16)
	{
		if(id < 256)
		{
			PacketStatistic& ps = m_Packets[index][id];
			ps.executeCount++;
			if(ps.executeLongestTime < time)
			{
				ps.executeLongestTime = time;
			}
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Packet id overflow[%d]!", __FUNCTION__, id);
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Thread index overflow[%d]!", __FUNCTION__);
	}
}