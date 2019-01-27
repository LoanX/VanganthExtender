#pragma once
#pragma warning(disable: 4996)
#include "tchar.h"
#include "Config.h"
#include "CLog.h"
#include "CObject.h"
#include "NpcMarker.h"
#include "NpcServer.h"
#include "..//L2Ext//CreatureSharedData.h"
#include "..//L2Ext//CPledgeSharedData.h"
#include "CSharedFactory.h"
#include "GlobalObject.h"
#include "CNPC.h"
#include "Era.h"
#include "Timer.h"
#include "PostLoad.h"
#include "PacketHandler.h"

class L2NPC
{
public:
	static void Initialize();
};