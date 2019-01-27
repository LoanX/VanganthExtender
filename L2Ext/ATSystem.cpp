#include "StdAfx.h"
void ATSystem::Initialize()
{
	g_HookManager.WriteJump( 0x8F0B12, ATHook, 2);
	g_HookManager.WriteJump( 0x7A6330, ATHook2, 0);
}