#pragma once
#include "MemoryObject.h"
#include "AsynchronousExecution.h"

//size 1210h
class CCreatureController : public MemoryObject
{
public:
/* 0018 */ LPVOID _unkn0018;
/* 0020 */ int objectId;
/* 0024 */ int _unkn24[701];
/* 0B18 */ CAsynchronousExecution asynchronousExecution;

	void AsyncDoNothing();
};