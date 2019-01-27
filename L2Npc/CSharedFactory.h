#pragma once

class CSharedFactory
{
public:
	static void Initialize();
	static void ExtendCreatureSharedData();
	static void ExtendPledgeSharedData();
	static void ReallocateAbnormalTable();
	static void ReallocateQuests();
};