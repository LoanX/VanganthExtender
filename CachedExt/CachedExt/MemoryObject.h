#pragma once
#include "SPointer.h"

class MemoryObject
{
public:
	/* 000 */	virtual void Release(const char* file, int line, int nWorkType, bool flag) = 0;
};