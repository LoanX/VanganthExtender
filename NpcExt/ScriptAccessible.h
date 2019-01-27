#pragma once
#include "GlobalObject.h"

static const UINT64 AI_TYPE_VOID = 0x20fa650;
static const UINT64 AI_TYPE_INT = 0x20FA648;
static const UINT64 AI_TYPE_SHARED_CREATURE_DATA = 0x2ebd7b0;
static const UINT64 AI_TYPE_NPC_MAKER_EX_OUT = 0x2EBC7F0;
static const UINT64 AI_TYPE_WCHAR_STRING = 0x20FA628;
static const UINT64 AI_TYPE_NPC = 0x2EBC7A0;
static const UINT64 AI_CODE_INFO_LIST = 0x2EBC7C0;

struct AiFunction
{
	const WCHAR* wName;
	LPVOID handler;	//function that passes params to main ai func
	UINT64 returnType;
	UINT64 paramCount;
	UINT64 param1Type;
	UINT64 param2Type;
	UINT64 param3Type;
	UINT64 param4Type;
	UINT64 param5Type;
	UINT64 param6Type;
	UINT64 param7Type;
	UINT64 param8Type;
	UINT64 param9Type;
	UINT64 param10Type;
	UINT64 param11Type;
	AiFunction() : wName(0), handler(0), returnType(0), paramCount(0), param1Type(0), param2Type(0), param3Type(0), param4Type(0), param5Type(0), param6Type(0), param7Type(0), param8Type(0), param9Type(0), param10Type(0), param11Type(0)
	{
	}
}; 

class CScriptAccessible
{
public:
	static void Init();
	static void ExtendGlobalObjectFunctionArray();
};