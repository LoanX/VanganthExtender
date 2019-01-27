#pragma once
#include "MemoryObject.h"


//vt 6BBF68h
class CObject : public MemoryObject
{
public:
	/* 18 */ LPVOID _unkn18;	//6BC200h
	/* 20 */ LPVOID _unkn20;	//9D3B4B0h	for creature : 683FE8h - some kind of data filler
	/* 28 */ UINT _unkn28;
	/* 2C */ UINT _unkn2C;
	/* 30 */ UINT _unkn30;
	/* 34 */ UINT _unkn34;
};