#include "StdAfx.h"
#include "ItemDataParser.h"

void CItemDataParser::Init()
{
	//initialize
	g_HookManager.WriteCall(0x008E799C, FindInTable, 0);
	g_HookManager.WriteCall(0x008E91B7, FindInTable, 0);
}

INT64 CItemDataParser::FindInTable(ObjectFieldType field, PWCHAR wStr)
{
	typedef INT64 (*_f)(ObjectFieldType, PWCHAR);
	_f f = (_f) 0x008E76F0;

	INT64 n = f(field, wStr);
	if(n == -1)
	{
		if(_wcsicmp(wStr, L"infinity_soulshot") == 0)
			return EtcItemInfinitySoulshot;
		if(_wcsicmp(wStr, L"infinity_spiritshot") == 0)
			return EtcItemInfinitySpiritshot;
		if(_wcsicmp(wStr, L"infinity_blessed_spiritshot") == 0)
			return EtcItemInfinityBlessedSpiritshot;
		if(_wcsicmp(wStr, L"hairdeco") == 0)
			return 0x010000LL; //0x040000LL;
		if(_wcsicmp(wStr, L"hair2") == 0)
			return 0x010000LL; //0x040000LL;
		else if(_wcsicmp(wStr, L"hairall") == 0)
			return 0x010000LL; //0x080000LL;
		
	}

	return n;
}