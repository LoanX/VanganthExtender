#pragma once

struct CollisionHashLink
{
	/* 00 */ CCreature *pCreature;
	/* 08 */ CollisionHashLink *lpNext;
	/* 10 */ bool inUse;
};

class CHLArray
{
	CSLock m_lock;
	CollisionHashLink m_links[5000];
	UINT m_inUse;
	UINT m_total;
public:
	CHLArray();
	~CHLArray();
	CollisionHashLink* Get();
	void Free(CollisionHashLink* lpLink);
};

extern "C"
{
	CollisionHashLink* GetCollisionHashLink();
	void FreeCollisionHashLink(CollisionHashLink* lpLink);
	void CWorldCollisionHash_EnterCreature1();
	void CWorldCollisionHash_EnterCreature2();
	void CWorldCollisionHash_RemoveCreature1();
	void CWorldCollisionHash_MoveCreature1();
};


struct MwmrQueue
{
	/* 00 */ CollisionHashLink _link;
	/* 10 */ CollisionHashLink* lpFirst;
	/* 18 */ UINT spinLock;
	/* 1C */ INT32 usedSlots;
	/* 20 */ INT32 freeSlots;
	/* 24 */ INT32 freeSlotsEx;
};


struct DoorCollision
{
	CDoor *pDoor;
	FVector range[4];
};

class CWorldCollision
{
	BOOL m_enabled;
	BOOL m_onlyDoorCollision;
	void LoadINI();
public:
	void Init();
};

extern CWorldCollision g_WorldCollision;