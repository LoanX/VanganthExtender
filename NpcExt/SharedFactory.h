#pragma once
#include "SharedCreatureData.h"
#include "SharedPledgeData.h"

class CSharedFactory
{
public:
	static void Init();
	static void ExtendSharedCreatureData();
	static void ExtendSharedPledgeData();
	static void ReallocateAbnormalArray();
	static void ReallocateQuestArray();
};