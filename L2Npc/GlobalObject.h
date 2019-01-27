#pragma once
#include "Utils.h"

class CGlobalObject
{
public:
	static void Initialize()
	{
		WriteHook(JMP, 0x4C69A2, (DWORD)CGlobalObject::Rand, 0);
	}
	static int Rand(CGlobalObject *pObject, int Number)
	{
		typedef int (*_f)(CGlobalObject*, int);
		_f f = (_f) 0x0051F730;

		//g_Log.Add(CLog::Blue, "[%s] Called!", __FUNCTION__);
		if(Number < 0)
		{
			switch(Number)
			{
			case -666:
				return g_Era.GetVariable();
				break;
			default:
				g_Era.SetVariable((-Number));
				g_Log.Add(CLog::Blue, "Custom Global Variable has been set to [%d]. ", (-Number));
				return (-Number);
				break;
			}
			return 0;
		}else
			return f(pObject, Number);
	}
};