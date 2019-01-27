#include "StdAfx.h"

void AVESystem::Initialize()
{
	g_HookManager.WriteJump( 0x8F0E36, AVEHook, 2);
}