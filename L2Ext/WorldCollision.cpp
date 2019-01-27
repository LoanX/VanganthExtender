#include "stdafx.h"
#include "WorldCollision.h"
#include "HookManager.h"

CWorldCollision g_WorldCollision;

CHLArray g_CHLArray;

void CWorldCollision::Init()
{
	LoadINI();
	if(m_enabled)
	{
		g_Log.Add(CLog::Blue, "Initializing World Collision Mod");
		if(!m_onlyDoorCollision)
		{
			LPINT lpUsed = (LPINT)0x19AD01EC;
			(*lpUsed) = 0;
			LPINT lpFree = (LPINT)0x19AD01F0;
			(*lpFree) = 5000;
			LPINT lpTotalFree = (LPINT)0x19AD01F4;
			(*lpTotalFree) = 5000;

			g_HookManager.WriteJump(0x8C511F, CWorldCollisionHash_EnterCreature1, 9);
			g_HookManager.WriteJump(0x8C559F, CWorldCollisionHash_EnterCreature2, 9);
			g_HookManager.WriteJump(0x8C3682, CWorldCollisionHash_RemoveCreature1, 2);
			g_HookManager.WriteCall(0x8C71D6, GetCollisionHashLink, 0);
			g_HookManager.WriteJump(0x8C73D3, CWorldCollisionHash_MoveCreature1, 2);
		}
	}
}

void CWorldCollision::LoadINI()
{
	const TCHAR* section = _T("WorldCollision");
	m_enabled = GetPrivateProfileInt(section, _T("Enabled"), FALSE, g_ConfigFile);
	m_onlyDoorCollision = GetPrivateProfileInt(section, _T("DoorOnly"), FALSE, g_ConfigFile);
}

CHLArray::CHLArray() : m_total(5000)
{
	memset(m_links, 0, sizeof(m_links));
}

CHLArray::~CHLArray()
{
}

CollisionHashLink* GetCollisionHashLink()
{
	return g_CHLArray.Get();
}

void FreeCollisionHashLink(CollisionHashLink *lpLink)
{
	g_CHLArray.Free(lpLink);
}

CollisionHashLink* CHLArray::Get()
{
//	g_Log.Add(CLog::Blue, "[%s] [%d]", __FUNCTION__, m_inUse);
	m_lock.Enter();
	for(UINT n=0;n<5000;n++)
	{
		if(m_links[n].inUse == false)
		{
			m_inUse++;
			m_links[n].inUse = true;
			m_lock.Leave();
			return &m_links[n];
		}
	}
	m_lock.Leave();

	g_Log.Add(CLog::Error, "[%s] Overflow!", __FUNCTION__);
	return 0;
}

void CHLArray::Free(CollisionHashLink *lpLink)
{
	m_lock.Enter();
	lpLink->pCreature = 0;
	lpLink->lpNext = 0;
	lpLink->inUse = false;
	m_inUse--;
	m_lock.Leave();
};