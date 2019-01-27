#pragma once

class CMoveAction
{
/* 00 */	PVOID _vtable;
/* 08 */	UINT creatureId;
/* 0C */	UINT _align0C;
/* 10 */	FVector destination;

public:
	CCreature* GetCreature()
	{
		typedef CCreature*(*f)(PUINT);
		return f(0x07F07F0L)(&creatureId);
	}
	inline FVector GetDestination() { return destination; };

};

class CMoveActionEx
{
/* 00 */	PVOID _vtable;
/* 08 */	UINT creatureId;
/* 0C */	UINT _align0C;
/* 10 */	UINT targetId;

public:
	CCreature* GetCreature()
	{
		typedef CCreature*(*f)(PUINT);
		return f(0x07F07F0L)(&creatureId);
	}
	CObject* GetTarget()
	{
		typedef CCreature*(*f)(PUINT);
		return f(0x7F0320L)(&targetId);
	}
};