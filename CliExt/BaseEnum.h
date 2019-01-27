#pragma once

enum HookType
{
	HookNOP = 0x90,
	HookJump = 0xE9,
	HookCall = 0xE8
};
