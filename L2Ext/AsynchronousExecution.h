#pragma once
#include "ExecutionArgument.h"

class CAsynchronousExecution
{
/* 000 */ long _unkn000;
/* 004 */ long _unkn004;
/* 008 */ long queue;
/* 00C */ long _unkn00C;
/* 010 */ long status;
/* 014 */ LPVOID _unkn[200];
/* 654 */ int _unkn654;
/* 658 */ Native funcArr[20];
public:
	static const Native DoNothing = 0x006A47C0L;
	void AddExecutable(MemoryObject* Matrix, Native Func, CExecutionArgument* Arg)
	{
		typedef void (*f)(CAsynchronousExecution*, MemoryObject*, Native, CExecutionArgument*);
		f(0x444050L)(this, Matrix, Func, Arg);
	};
};