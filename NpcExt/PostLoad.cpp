#include "stdafx.h"
#include "PostLoad.h"
#include "EraSystem.h"
#include "ScriptAccessible.h"
#include "LotteryManager.h"

void CPostLoad::Init()
{
	WriteCall(0x5CA90B, CPostLoad::LoadAfterAi, 0);
}

void CPostLoad::LoadAfterAi(const char *format)
{
	typedef void (*f)(const char*);
	f(0x582E30L)(format);
	
	CLotteryManager::Get().Init();
	g_EraSystem.Init();
}